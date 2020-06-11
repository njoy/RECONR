#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/printV.hpp"
#include "RECONR/details/nextMin.hpp"
#include "RECONR/details/checkRanges.hpp"

nlohmann::json input{R"({
  "nendf": 20, "npend": 22,
  "tlabel": "Modern RECONR Testing",
  "sequence": [
    {
      "mat": 125, "ncards": 1, "ngrid": 3,
      "err": 0.1, "tempr": 0, "errmax": 0.1, "errint": 5E-7,
      "cards": [ "Material 125 processed with modern RECONR" ],
      "enode": [ 1.0, 2.0, 3.0 ]
    },
    {
      "mat": 2631, "ncards": 2, "ngrid": 0,
      "err": 0.1, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
      "cards": [ "Material 2631 processed with modern RECONR",
                  "For testing purposes only." ],
      "enode": [ ]
    }
  ]
})"_json};
nlohmann::json input2631{R"({
  "nendf": 21, "npend": 23,
  "tlabel": "Modern RECONR Testing",
  "sequence": [
    {
      "mat": 2631, "ncards": 2, "ngrid": 0,
      "err": 0.1, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
      "cards": [ "Material 2631 processed with modern RECONR",
                  "For testing purposes only." ],
      "enode": [ ]
    }
  ]
})"_json};
nlohmann::json input30{R"({
  "nendf": 30, "npend": 32,
  "tlabel": "Modern RECONR Testing",
  "sequence": [
    {
      "mat": 9437, "ncards": 1, "ngrid": 3,
      "err": 0.1, "tempr": 0, "errmax": 0.1, "errint": 5E-7,
      "cards": [ "Material 125 processed with modern RECONR" ],
      "enode": [ 1.0, 2.0, 3.0 ]
    }
  ]
})"_json};
nlohmann::json input40{R"({
  "nendf": 40, "npend": 42,
  "tlabel": "Modern RECONR Testing",
  "sequence": [
    {
      "mat": 9437, "ncards": 1, "ngrid": 3,
      "err": 0.1, "tempr": 0, "errmax": 0.1, "errint": 5E-7,
      "cards": [ "Material 125 processed with modern RECONR" ],
      "enode": [ 1.0, 2.0, 3.0 ]
    }
  ]
})"_json};
/*
    {
      "mat": 9228, "ncards": 1, "ngrid": 0,
      "err": 0.1, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
      "cards": [ "Material 9228 processed with modern RECONR" ],
      "enode": [ ]
    }
*/

std::vector< double> XSEnergies(){

  std::vector< double > MT1{
    1.0E-5, 
    details::nextMin( 2.0E-5 ), 2.0E-5,
    details::nextMin( 7.5E+5 ), 7.5E+5,
    details::nextMin( 1.9E+7 ), 1.9E+7,
    details::nextMin( 1.95E+7 ),1.95E+7,
    details::nextMin( 2.0E+7 ), 2.0E+7 };
  std::vector< double > MT16{
    1e-05,       1.25e-05,    1.5e-05,     2e-05,       
    2.5457e-05,  3.09139e-05, 4.18279e-05, 5.27418e-05, 6.36557e-05,  
    8.54836e-05, 0.000107311, 0.000150967, 0.000194623, 0.000281934,  
    0.000369246, 0.000718492, 0.00141698,  0.00281397,  0.00560794,   
    0.0111959,   0.0223717,   0.0447235,   0.089427,    0.178834,     
    0.357648,    0.715276,    1.43053,     2.86104,     5.72207,      
    11.4441,     22.8882,     45.7764,     91.5528,     183.105,      
    366.211,     732.422,     1464.84,     2929.69,     5859.38,      
    11718.8,     23437.5,     46875,       93750,       187500,       
    375000,      750000,      1.9e+07,     1.95e+07,    2e+07 };
  std::vector< double > MT18{ 
    1.0E+5,
    details::nextMin( 1.5E+5 ), 1.5E+5,
    details::nextMin( 7.5E+5 ), 7.5E+5,
    1.9E+7, 1.95E+7, 2.0E+7 };
  std::vector< double > MT102{
    1e-05,        1.0625e-05,   1.125e-05,    1.1875e-05,   1.25e-05,     
    1.3125e-05,   1.375e-05,    1.4375e-05,   1.5e-05,      1.5625e-05,   
    1.625e-05,    1.75e-05,     1.875e-05,    2e-05,
    6.36557e-05,  0.000107311,  0.000194623,  0.000369246,  0.000718492,  
    0.00141698,   0.00281397,   0.00560794,   0.0111959,    0.0223717,    
    0.0447235,    0.089427,     0.178834,     0.357648,     0.715276,     
    1.43053,      2.86104,      5.72207,      11.4441,      22.8882,      
    45.7764,      91.5528,      183.105,      366.211,      732.422,      
    1464.84,      2929.69,      5859.38,      11718.8,      23437.5,      
    46875,        93750,        187500,       375000,       750000,       
    1.9e+07,      1.95e+07,     2e+07 };

  return ranges::view::concat( MT1, MT16, MT18, MT102 ) | ranges::to_vector;

}

template< typename ERange, typename XRange >
void zeroXS( const ERange& energies, XRange& XS, 
             const double& min, const double& max ){

  for( const auto& [ E, xs ] : ranges::view::zip( energies, XS ) ){
    if( ( E < min ) or ( E > max ) ){
      xs = 0.0;
    }
  }
}

template< typename... Ranges >
std::vector< double > sumRanges( const Ranges&... ranges ){
    auto sum_tuple = [](auto&& tuple) { 
      return std::apply([](auto... v) { 
        return (v + ...); 
      }, tuple );
    };

    return ranges::view::zip( ranges... )
      | ranges::view::transform(sum_tuple)
      | ranges::to_vector;
}

// Linearize and reconstruct resonances
auto lin_recon( std::string formalism, double absTol, double relTol ){
  std::vector< double > userSupplied{ 1.0, 2.0, 3.0 };
  auto material = details::ENDFMaterial( formalism );
  auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

  njoy::RECONR::RECONR::linearizeXS( std::cout, r2d2, absTol, relTol);
  auto refGrid = njoy::RECONR::RECONR::unionizeEnergyGrid(
    std::cout, r2d2, userSupplied );
  njoy::RECONR::RECONR::reconstructResonances( 
    std::cout, refGrid, r2d2, relTol, absTol );
  auto energies = njoy::RECONR::RECONR::unionizeEnergyGrid(
    std::cout, r2d2 );

  return std::make_pair( energies, r2d2 );
}

SCENARIO( "Testing creation of RECONR class" ){
  GIVEN( "a JSON object, and extra arguments" ){

    auto args = nlohmann::json::object();

    WHEN( "a RECONR object is called" ){
      CHECK_NOTHROW( njoy::RECONR::RECONR()( input, std::cout, args ) );

    } // THEN
  } // GIVEN
  GIVEN( "a JSON object, and extra arguments" ){

    auto args = nlohmann::json::object();

    WHEN( "a RECONR object is called" ){
      CHECK_NOTHROW( njoy::RECONR::RECONR()( input2631, std::cout, args ) );

    } // THEN
  } // GIVEN
  // GIVEN( "an ENDF file with just one resonance" ){

  //   auto args = nlohmann::json::object();

  //   WHEN( "a RECONR object is called" ){
  //     CHECK_NOTHROW( njoy::RECONR::RECONR()( input40, std::cout, args ) );
  //   } // THEN
  // } // GIVEN
  // GIVEN( "an ENDF file with just three resonances" ){

  //   auto args = nlohmann::json::object();

  //   WHEN( "a RECONR object is called" ){
  //     CHECK_NOTHROW( njoy::RECONR::RECONR()( input30, std::cout, args ) );
  //   } // THEN
  // } // GIVEN
} // SCENARIO

/*
SCENARIO( "Testing single resonance" ){
  const std::string single{R"(
Retrieved by E4-Web: 2020/06/10,18:34:17                             1 0  0    0
 9.423900+4 2.369986+2          1          1          0          59437 1451    1
 0.000000+0 1.000000+0          0          0          0          69437 1451    2
 1.000000+0 2.000000+7          0          0         10          89437 1451    3
 0.000000+0 0.000000+0          0          0          7          69437 1451    4
 94-Pu-239 LANL       EVAL-DEC17             LANL, ORNL, et al.   9437 1451    5
 NDS 148, 1 (2018)    DIST-FEB18                       20170914   9437 1451    6
----ENDF/B-VIII.0     MATERIAL 9437                               9437 1451    7
-----INCIDENT NEUTRON DATA                                        9437 1451    8
------ENDF-6 FORMAT                                               9437 1451    9
 ************************ C O N T E N T S *********************** 9437 1451  562
 **************** Program DICTIN (VERSION 2015-2) ****************9437 1451  563
                                1        451        569          59437 1451  564
                                2        151          6          39437 1451  565
                                3          1          4          19437 1451  566
                                3          2          4          59437 1451  567
                                3         18          4          49437 1451  568
                                3        102          4          19437 1451  569
                                                                  9437 1  099999
                                                                  9437 0  0    0
 9.423900+4 2.369986+2          0          0          1          09437 2151    1
 9.423900+4 1.000000+0          0          1          1          09437 2151    2
 1.000000-5 2.500000+3          1          3          0          19437 2151    3
 5.000000-1 9.410000-1          1          0          1          49437 2151    4
 2.369986+2 9.410000-1          0          0          6          19437 2151    5
 1.541700+1 0.000000+0 2.056203-3 4.054259-2-1.093928-6 7.550000-19437 2151    6
                                                                  9437 2  099999
                                                                  9437 0  0    0
 9.423900+4 2.369986+2          0          0          0          09437 3  1    1
 0.000000+0 0.000000+0          0          0          1          29437 3  1    2
          2          2                                            9437 3  1    3
 1.000000-5 0.000000+0 2.000000+7 0.000000+0                      9437 3  1  142
                                                                  9437 3  099999
 9.423900+4 2.369986+2          0          0          0          09437 3  2    1
 0.000000+0 0.000000+0          0          0          1          29437 3  2    2
          2          2                                            9437 3  2    3
 1.000000-5 0.000000+0 2.000000+7 0.000000+0                      9437 3  2  142
                                                                  9437 3  099999
 9.423900+4 2.369986+2          0          0          0          09437 3 18    1
 1.985785+8 1.985785+8          0          0          1          29437 3 18    2
          2          2                                            9437 3 18    3
 1.000000-5 0.000000+0 2.000000+7 0.000000+0                      9437 3 18   61
                                                                  9437 3  099999
 9.423900+4 2.369986+2          0          2          0          09437 3102    1
 6.534314+6 6.534314+6          0          0          1          29437 3102    2
          2          2                                            9437 3102    3
 1.000000-5 0.000000+0 2.000000+7 0.000000+0                      9437 3102    4
                                                                  9437 3  099999
                                                                  9437 0  0    0
                                                                     0 0  0    0
                                                                    -1 0  0    0
  )"};
  auto begin = single.begin();
  auto start = begin;
  auto end = single.end();
  long lineNumber = 0;

  njoy::ENDFtk::HeadRecord head( begin, end, lineNumber );
  njoy::ENDFtk::syntaxTree::Tape< std::string >::Material_t material(
    head, start, begin, end, lineNumber );
  auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
  njoy::RECONR::RECONR::linearizeXS( std::cout, r2d2, 1E-10, 0.1 );

  std::vector< double > user;
  auto njoyGrid = njoy::RECONR::RECONR::unionizeEnergyGrid( 
      std::cout, r2d2, user);

  njoy::Log::info( "njoyGrid: {}", njoyGrid | ranges::view::all );

  std::vector< double > grid{ 1E-5, 1.5417E1, 2.5E3, 2E7 };
  njoy::RECONR::RECONR::reconstructResonances( 
    std::cout, grid, r2d2, 0.1, 1E-10 );
  
} // SCENARIO
*/

/*
SCENARIO( "Getting evaluated data" ){
  WHEN( "Getting an existant ENDF Tape" ){
    auto evaluatedData = njoy::RECONR::RECONR::getEvaluated(
      std::cout, input[ "nendf" ] );

    auto tape = std::get< 0 >( evaluatedData );

    GIVEN( "MAT numbers in the tape" ){
      const std::vector< int > validMATs{ 125, 2631, 9228 };
      for( auto MAT : validMATs ){
        THEN( "we can get Material: " + std::to_string( MAT ) ){
          CHECK( tape.hasMAT( MAT ) );
          CHECK_NOTHROW( tape.MAT( MAT ) );
        } // THEN
      }
    } // GIVEN
  } // WHEN
  WHEN( "Getting an non-existant ENDF Tape" ){
    THEN( "an exception is thrown" ){
      CHECK_THROWS( njoy::RECONR::RECONR::getEvaluated( std::cout, 45 ) );
    } // THEN

  } // WHEN
} // SCENARIO
SCENARIO( "Testing the linearization of collected cross sections" ){
  GIVEN( "an ResonanceReconstructionDataDelivery object" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    WHEN( "the cross sections are linearized" ){
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      njoy::RECONR::RECONR::linearizeXS( 
        std::cout, r2d2, absTolerance, relTolerance );

      auto rXSs = r2d2.reactions();
      auto lReactions = r2d2.linearReactions();

      auto keys = ranges::view::keys( rXSs );
      CHECK( ranges::equal( keys, ranges::view::keys( lReactions ) ) );
      
      int MT;
      THEN( "we can check MT=1" ){
        MT = 1;
        auto reaction = lReactions.at( MT );
        std::vector< double > refE{
          1.0E-5, 
          details::nextMin( 2.0E-5 ), 2.0E-5,
          details::nextMin( 7.5E+5 ), 7.5E+5,
          details::nextMin( 1.9E+7 ), 1.9E+7,
          details::nextMin( 1.95E+7 ),1.95E+7,
          details::nextMin( 2.0E+7 ), 2.0E+7 };
        std::vector< double > refB{
          1.0         , 1.0         , 
          1.182897E1  , 1.182897E1  , 
          3.347392E-5 , 3.347392E-5 , 
          2.751761E-5 , 2.751761E-5 , 
          2.731301E-5 , 2.731301E-5 , 
          2.710792E-5 };

        auto lXS = reaction.crossSections();
        auto energies = lXS.x() | ranges::to_vector;
        auto barns = lXS.y() | ranges::to_vector;
        CHECK( ranges::distance( refE ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refB ) == ranges::distance( barns ) );
        for( const auto& [r, e ] : ranges::view::zip( refE, energies ) ){
          CHECK( r == Approx( e ).epsilon( 5E-6 ) );
        }
        for( const auto& [r, b ] : ranges::view::zip( refB, barns ) ){
          CHECK( r == Approx( b ).epsilon( 5E-6 ) );
        }
      }
      THEN( "we can check MT=16" ){
        MT = 16;
        auto reaction = lReactions.at( MT );
				std::vector< double > refE{
					1e-05,       1.25e-05,    1.5e-05,     2e-05,       
					2.5457e-05,  3.09139e-05, 4.18279e-05, 5.27418e-05, 6.36557e-05,  
					8.54836e-05, 0.000107311, 0.000150967, 0.000194623, 0.000281934,  
					0.000369246, 0.000718492, 0.00141698,  0.00281397,  0.00560794,   
					0.0111959,   0.0223717,   0.0447235,   0.089427,    0.178834,     
					0.357648,    0.715276,    1.43053,     2.86104,     5.72207,      
					11.4441,     22.8882,     45.7764,     91.5528,     183.105,      
					366.211,     732.422,     1464.84,     2929.69,     5859.38,      
					11718.8,     23437.5,     46875,       93750,       187500,       
					375000,      750000,      1.9e+07,     1.95e+07,    2e+07 };
				std::vector< double > refB{
					16,          14.5175,     13.4088,        11.829,
					10.4225,     9.41266,     8.0318,         7.11182,     6.4435,       
					5.51996,     4.89908,     4.09575,        3.5847,      2.95121,      
					2.56166,     1.80645,     1.26493,        0.882532,    0.614596,     
					0.427607,    0.29737,     0.206751,       0.143729,    0.0999125,    
					0.0694513,   0.0482764,   0.0335573,      0.0233258,   0.0162138,    
					0.0112703,   0.00783398,  0.00544542,     0.00378512,  0.00263104,   
					0.00182884,  0.00127123,  0.000883634,    0.000614215, 0.000426942,  
					0.000296768, 0.000206284, 0.000143388,    9.96694e-05, 6.92804e-05,  
					4.81569e-05, 3.34739e-05, 2.75176e-05,    2.7313e-05,  2.71079e-05 };

        auto lXS = reaction.crossSections();
        auto energies = lXS.x() | ranges::to_vector;
        auto barns = lXS.y() | ranges::to_vector;
        CHECK( ranges::distance( refE ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refB ) == ranges::distance( barns ) );
        for( const auto& [r, e ] : ranges::view::zip( refE, energies ) ){
          CHECK( r == Approx( e ).epsilon( 5E-6 ) );
        }
        for( const auto& [r, b ] : ranges::view::zip( refB, barns ) ){
          CHECK( r == Approx( b ).epsilon( 5E-6 ) );
        }
      }
      THEN( "we can check MT=18" ){
        MT = 18;
        auto reaction = lReactions.at( MT );
        std::vector< double > refE{ 
          1.0E+5,
          details::nextMin( 1.5E+5 ), 1.5E+5,
          details::nextMin( 7.5E+5 ), 7.5E+5,
          1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refB{ 
          1.8E+1      , 1.8E+1      , 
          1.182897E+1 , 1.182897E+1 , 
          3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto lXS = reaction.crossSections();
        auto energies = lXS.x() | ranges::to_vector;
        auto barns = lXS.y() | ranges::to_vector;
        CHECK( ranges::distance( refE ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refB ) == ranges::distance( barns ) );
        for( const auto& [r, e ] : ranges::view::zip( refE, energies ) ){
          CHECK( r == Approx( e ).epsilon( 5E-6 ) );
        }
        for( const auto& [r, b ] : ranges::view::zip( refB, barns ) ){
          CHECK( r == Approx( b ).epsilon( 5E-6 ) );
        }
      }
      THEN( "we can check MT=102" ){
        MT = 102;
        auto reaction = lReactions.at( MT );
				std::vector< double > refE{
					1e-05,        1.0625e-05,   1.125e-05,    1.1875e-05,   1.25e-05,     
					1.3125e-05,   1.375e-05,    1.4375e-05,   1.5e-05,      1.5625e-05,   
					1.625e-05,    1.75e-05,     1.875e-05,    2e-05,
					6.36557e-05,  0.000107311,  0.000194623,  0.000369246,  0.000718492,  
					0.00141698,   0.00281397,   0.00560794,   0.0111959,    0.0223717,    
					0.0447235,    0.089427,     0.178834,     0.357648,     0.715276,     
					1.43053,      2.86104,      5.72207,      11.4441,      22.8882,      
					45.7764,      91.5528,      183.105,      366.211,      732.422,      
					1464.84,      2929.69,      5859.38,      11718.8,      23437.5,      
					46875,        93750,        187500,       375000,       750000,       
					1.9e+07,      1.95e+07,     2e+07 };
				std::vector< double > refB {
					102,          94.1134,      86.6777,     79.6441,     72.9714,         
					66.6243,      60.5726,      54.7899,     49.2533,     43.9428,         
					38.8406,      29.2,         20.2247,     11.829,
					11.2665,      11.0128,      10.7235,     10.4124,     10.089,          
					9.75904,      9.42573,      9.0907,      8.75481,     8.41849,         
					8.08195,      7.7453,       7.4086,      7.07187,     6.73513,         
					6.39838,      6.06163,      5.72488,     5.38812,     5.05137,         
					4.71461,      4.37786,      4.0411,      3.70435,     3.36759,         
					3.03083,      2.69408,      2.35732,     2.02057,     1.68381,         
					1.34706,      1.0103,       0.673545,    0.336789,    3.34739e-05,     
					2.75176e-05,  2.7313e-05,  2.71079e-05 };

        auto lXS = reaction.crossSections();
        auto energies = lXS.x() | ranges::to_vector;
        auto barns = lXS.y() | ranges::to_vector;
        CHECK( ranges::distance( refE ) == ranges::distance( energies ) );
        CHECK( ranges::distance( refB ) == ranges::distance( barns ) );
        for( const auto& [r, e ] : ranges::view::zip( refE, energies ) ){
          CHECK( r == Approx( e ).epsilon( 5E-6 ) );
        }
        for( const auto& [r, b ] : ranges::view::zip( refB, barns ) ){
          CHECK( r == Approx( b ).epsilon( 5E-6 ) );
        }
      }
    } // THEN
  } // GIVEN
} // SCENARIO
SCENARIO( "Testing the resonance reconstruction" ){
  GIVEN( "a SpecialCase R2D2 object and reference grid" ){
    auto material = details::ENDFMaterial( "SpecialCase" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
    const std::vector< double > refGrid{
      0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5
    };

    WHEN( "resonances are reconstructed" ){
      njoy::RECONR::RECONR::reconstructResonances(
        std::cout, refGrid, r2d2, 1E-1, 1E-3
      );

      THEN( "the linearized reconstruction can be verified" ){
        auto reconstructed = r2d2.reconstructedResonances();

        auto elastic = reconstructed[ 2 ].front();

        std::vector< double > refEnergies{ 1E-5, 1E5 };
        // 4*pi*2*2---2 is the scattering radius
        double sigma{ 50.265482456 };
        std::vector< double > refElastic( 2, sigma );
        details::checkRanges( refEnergies,  elastic.x() );
        details::checkRanges( refElastic,  elastic.y() );
      
      } // THEN
    } // WHEN
  } // GIVEN
  GIVEN( "an SLBW R2D2 object and reference grid" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
    const std::vector< double > refGrid{
      0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5
    };

    WHEN( "the resonances are reconstructed" ){
      njoy::RECONR::RECONR::reconstructResonances(
        std::cout, refGrid, r2d2, 1E-1, 1E-3 );

      THEN( "the linearized reconstruction can be verified" ){
        auto reconstructed = r2d2.reconstructedResonances();

        std::vector< double > refEnergies{ 
          1e-05,    0.246525, 0.49304,  0.616297, 0.739554, 0.801183, 0.862812,  
          0.893626, 0.92444,  0.955255, 0.986069, 1.0253,   1.06453,  1.09455,   
          1.12458,  1.1546,   1.18463,  1.24468,  1.30472,  1.42482,  1.54492,   
          1.78511,  2.0253,   2.26549,  2.50568,  2.62578,  2.74588,  2.80592,   
          2.86597,  2.896,    2.92602,  2.95605,  2.98607,  3.0253,   3.06453,   
          3.08186,  3.09918,  3.13384,  3.16849,  3.20314,  3.27244,  3.34175,   
          3.48036,  3.61896,  3.89618,  4.1734,   4.72783,  5.28227,  6.39113,   
          7.5};
        std::vector< double > refElastic{ 
          0.12506,    0.142374,  0.239419,  0.376941,  0.731633,  1.15853,     
          2.12402,    3.132,     5.03349,   9.1272,    19.0871,   38.1473,     
          19.0819,    9.28208,   5.16956,   3.23402,   2.19952,   1.20137,     
          0.756675,   0.38362,   0.235867,  0.12328,   0.0861085, 0.0790813,   
          0.10576,    0.151698,  0.274666,  0.423839,  0.760705,  1.11065,     
          1.76604,    3.15938,   6.48055,   12.9402,   6.47874,   4.21545,     
          2.85963,    1.51044,   0.9191,    0.616025,  0.333129,  0.21033,     
          0.108652,   0.0682899, 0.0363148, 0.0237135, 0.0133756, 0.00901362,  
          0.00516115, 0.00343646 };
        std::vector< double > refCapture{ 
          6850.24, 49.8161, 58.9968, 82.7983, 146.211, 222.108, 391.906,   
          567.549, 896.41,  1598.51, 3289.5,  6446.72, 3165.14, 1518.74,   
          834.808, 515.696, 346.513, 185.012, 114.139, 55.8541, 33.4273,   
          17.0292, 12.114,  11.835,  17.1509, 25.4053, 46.9056, 72.6816,   
          130.496, 190.301, 301.974, 538.661, 1100.86, 2185.28, 1086.47,   
          704.507, 476.198, 249.594, 150.619, 100.063, 53.0942, 32.8511,   
          16.2643, 9.79166, 4.79638, 2.91043, 1.45495, 0.89217, 0.443622,  
          0.265993 };
        std::vector< double > refFission( 50, 0.0 );

        auto elastic = reconstructed[ 2 ].front();
        auto capture = reconstructed[ 102 ].front();
        auto fission = reconstructed[ 18 ].front();
        details::checkRanges( refEnergies, elastic.x() );
        details::checkRanges( refElastic, elastic.y() );
        details::checkRanges( refEnergies, capture.x() );
        details::checkRanges( refCapture, capture.y() );
        details::checkRanges( refEnergies, fission.x() );
        details::checkRanges( refFission, fission.y() );
        
      } // THEN
      
    } // WHEN
    
  } // GIVEN
  GIVEN( "an RM R2D2 object and reference grid" ){
    auto material = details::ENDFMaterial( "RM" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
      std::vector< double > refGrid { 
        0.98596, 1.06389, 1.1389, 1.21391, 2.02109, 2.0361, 2.05111, 
        2.74827, 2.7767, 2.80513, 3.14517, 3.1566, 3.16803, 3.60111, 
        3.6208, 3.64049, 4.8336, 4.8508, 4.868, 5.24932, 5.4497, 5.5
      };

    WHEN( "the resonances are reconstructed" ){
      njoy::RECONR::RECONR::reconstructResonances( 
          std::cout, refGrid, r2d2, 1E-1, 1E-3 );

      THEN( "the linearized reconstruction can be verified" ){
        auto reconstructed = r2d2.reconstructedResonances();

        std::vector< double > refEnergies{ 
          0.98596,  1.02493,  1.06389,  1.1389,   1.21391,  1.26436,  1.31481,  
          1.4157,   1.6175,   1.8193,   1.86974,  1.92019,  1.94542,  1.97064,  
          1.98325,  1.99587,  2.00848,  2.02109,  2.0361,   2.05111,  2.0729,   
          2.09468,  2.11647,  2.13826,  2.18183,  2.2254,   2.31255,  2.39969,  
          2.57398,  2.66113,  2.7047,   2.74827,  2.7767,   2.80513,  2.89014,  
          2.97515,  3.01765,  3.06016,  3.14517,  3.1566,   3.16803,  3.22216,  
          3.2763,   3.33044,  3.38457,  3.43871,  3.49284,  3.51991,  3.54697,  
          3.57404,  3.60111,  3.6208,   3.64049,  3.65913,  3.67777,  3.69642,  
          3.71506,  3.75234,  3.78963,  3.8642,   3.93877,  4.08791,  4.23705,  
          4.38618,  4.53532,  4.60989,  4.68446,  4.72175,  4.75903,  4.77767,  
          4.79632,  4.81496,  4.82428,  4.8336,   4.8508,   4.868,    4.87992,  
          4.89183,  4.90375,  4.91567,  4.9395,   4.96333,  5.01099,  5.05866,  
          5.15399,  5.24932,  5.4497,   5.5 };
        std::vector< double > refElastic{ 
          14.5826, 14.5327, 14.5131, 14.8464, 15.0201, 14.8999, 14.7925,
          14.6392, 14.4366, 14.2619, 14.2091, 14.1403, 14.0929, 14.0281,
          13.9863, 13.939, 13.9007, 13.9354, 14.2386, 14.5251, 14.5097,
          14.4214, 14.3556, 14.3078, 14.241, 14.1931, 14.1203, 14.0599,
          13.9471, 13.8813, 13.8362, 13.7759, 13.7421, 13.7296, 13.7054,
          13.6351, 13.5874, 13.539, 13.6968, 13.7725, 13.8435, 13.9331,
          13.8284, 13.7176, 13.6097, 13.4893, 13.3362, 13.2434, 13.156,
          13.1662, 13.6013, 14.2867, 14.7618, 14.821, 14.7118, 14.5741,
          14.4504, 14.2624, 14.1328, 13.9657, 13.8571, 13.7056, 13.5815,
          13.4505, 13.2714, 13.1313, 12.8992, 12.7039, 12.3859, 12.1396,
          11.7954, 11.3532, 11.1917, 11.3625, 14.4855, 16.9067, 16.5402,
          15.9963, 15.5563, 15.2253, 14.781, 14.5025, 14.1737, 13.9824,
          13.7587, 13.6287, 13.5656, 13.5755 };
        std::vector< double > refCapture{ 
          9.93351, 13.3482, 19.639, 36.16, 19.178, 10.8929, 7.09064,
          4.13228, 2.63351, 2.91457, 3.59415, 5.41459, 7.59086, 12.3436,
          17.0365, 25.1773, 39.9831, 65.0758, 88.8534, 64.4467, 27.7593,
          14.1011, 8.62062, 5.99604, 3.69314, 2.75031, 2.01499, 1.76788,
          1.8389, 2.35618, 3.04173, 4.17966, 4.6298, 4.32927, 3.24947,
          4.04808, 5.35192, 8.01053, 20.1129, 20.6294, 20.1278, 11.7661,
          6.9162, 5.37652, 5.41327, 6.87542, 11.1856, 15.9875, 25.0851,
          42.9939, 71.056, 79.9209, 64.9016, 44.9732, 30.3828, 21.1713,
          15.4037, 9.17284, 6.16688, 3.53898, 2.48523, 1.73358, 1.61699,
          1.90918, 3.03441, 4.5798, 8.60412, 13.5732, 25.2965, 38.1737,
          64.0024, 125.221, 188.316, 292.8, 499.675, 301.393, 171.423,
          103.625, 67.8095, 47.4078, 26.7679, 17.2455, 9.11788, 5.90234,
          3.57183, 2.98869, 3.11492, 3.01658 };
        std::vector< double > refFission{
          64.155, 73.7218, 89.705, 111.73, 49.7073, 30.5546, 23.3012,
          18.4774, 15.6896, 14.3587, 14.2416, 14.443, 14.8764, 15.9617,
          17.0766, 19.0255, 22.548, 28.3832, 33.3652, 26.8838, 18.0184,
          14.7916, 13.4737, 12.8033, 12.1121, 11.7105, 11.1305, 10.5992,
          9.04656, 7.30333, 6.11291, 6.59994, 9.8862, 14.2276, 19.2682,
          22.5998, 26.3267, 33.37, 59.2035, 58.8323, 56.0236, 32.6866,
          21.8327, 19.1471, 19.9451, 23.6452, 32.5672, 41.4005, 56.5703,
          82.8037, 114.893, 112.824, 79.6278, 48.2594, 28.5979, 17.619,
          11.493, 5.87904, 3.8114, 2.71883, 2.68868, 3.11531, 3.59612,
          4.12546, 4.87729, 5.48662, 6.5506, 7.53293, 9.40118, 11.1709,
          14.3614, 21.1263, 27.5253, 37.3192, 51.8651, 28.7732, 17.0467,
          11.5986, 9.04432, 7.7853, 6.84385, 6.71141, 7.2079, 8.12153,
          10.8557, 15.0913, 24.6041, 24.191 };

        auto elastic = reconstructed[ 2 ].front();
        auto capture = reconstructed[ 102 ].front();
        auto fission = reconstructed[ 18 ].front();
        details::checkRanges( refEnergies, elastic.x( ) );
        details::checkRanges( refElastic, elastic.y() );
        details::checkRanges( refEnergies, capture.x( ) );
        details::checkRanges( refCapture, capture.y() );
        details::checkRanges( refEnergies, fission.x( ) );
        details::checkRanges( refFission, fission.y() );
        
      } // THEN
      
    } // WHEN
    
  } // GIVEN
  
} // SCENARIO
SCENARIO( "Testing the summation of cross sections" ){
  double absTol{ 1E-6 };
  double relTol{ 1E-1 }; // This tolerance is large by design

  GIVEN( "an SLBW object" ){
    WHEN( "cross sections have been linearized and resonances reconstructed" ){
      auto [energies, r2d2] = lin_recon( "SLBW", absTol, relTol );
      auto sizeEnergies = ranges::distance( energies );

      auto reactions = njoy::RECONR::RECONR::summateReactions( 
          std::cout, r2d2, energies );
      int MT;
      THEN( "the reactions can be summed up and checked" ){
        std::vector< int > reference{ 
          1, 2, 3, 4, 16, 18, 51, 52, 102, 875, 876, 877 };

        auto keys = ranges::view::keys( reactions ) | ranges::to_vector;

        CHECK( reference == keys );
      } // THEN
      THEN( "MT = 1 can be tested" ){ 
        MT = 1;
        std::vector< double > refXS = sumRanges(
          reactions.at( 2 ).crossSections(), 
          reactions.at( 3 ).crossSections()
        );

        auto reaction = reactions.at( MT );
        CHECK( refXS == reaction.crossSections() );
      } // THEN
      THEN( "MT = 2 can be tested" ){ 
        MT = 2;
        std::vector< double > refXS{
          2.1250596224, 2.1250596069, 2.1250595915, 2.125059576,
          2.1250595605, 2.1250595451, 2.1250595297, 2.1250595142,
          2.1250594988, 2.1250594833, 2.1250594679, 2.1250594371,
          2.1250594062, 2.1250593754, 2.1250593754, 2.125059241,
          2.1250591067, 2.1250588385, 2.1250585706, 2.125058303,
          2.1250577684, 2.1250572344, 2.1250561682, 2.125055104,
          2.1250529807, 2.1250508639, 2.1250424569, 2.1250259186,
          2.1249939206, 2.1249341996, 2.124831779, 2.124694777,
          2.1246913672, 2.1257732654, 2.1325400578, 2.1705397832,
          2.2758449003, 2.3933663924, 2.6276993392, 2.999115101,
          3.8559270429, 4.7710888422, 6.546442872, 10.5523867734,
          14.5282269614, 21.0870853488, 28.9515293845, 40.1472608884,
          21.0819231841, 12.8937991537, 8.7179830724, 6.475047278,
          5.1699562167, 3.8153416418, 3.1720919024, 2.6065595044,
          2.3735241714, 2.2145840707, 2.1438845407, 2.0883434128,
          2.0784450931, 2.0916772364, 2.1145451168, 2.1642886589,
          2.2882255755, 2.4284567036, 2.719493436, 3.0557882446,
          3.6933602165, 5.0763163745, 8.4805527183, 11.145555755,
          14.9401749374, 8.4787428242, 5.8882709585, 4.4848883021,
          3.2230437589, 2.7175602154, 2.471153025, 2.2498545192,
          2.1567182569, 2.0810446197, 2.0513092395, 2.027716659,
          2.0183041169, 2.0104305066, 2.0070372823, 2.0047197672,
          2.0034364575, 2.001996977, 2.0013185985, 2.0008211181,
          2.0005620863, 2.0003117927, 2.0001982847, 2.0001372278,
          2.0000769303, 2.0000491588, 2.0000341092, 2.0000191704,
          2.0000122643, 2.0000085151, 2.0000047887, 2.0000030645,
          2.000002128, 2.0000011969, 2.000000766, 2.000000532,
          2.0000002992, 2.0000000748, 2.0000000187, 2.0000000047,
          2.0000000012, 2.0000000003, 2.0000000001, 2.0,
          2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0
        };

        auto reaction = reactions.at( MT );
        details::checkRanges( refXS, reaction.crossSections() );
      } // THEN
      THEN( "MT = 4 can be tested" ){ 
        MT = 4;
        std::vector< double > refXS = sumRanges( 
            reactions.at( 51 ).crossSections(), 
            reactions.at( 52 ).crossSections() );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( MT );
        CHECK( refXS == reaction.crossSections() );

      } // THEN
      THEN( "MT = 16 can be tested" ){ 
        MT = 16;
        std::vector< double > refXS( sizeEnergies, 1.5 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( MT );
        CHECK( refXS == reaction.crossSections() );
      } // THEN
      THEN( "MT = 18 can be tested" ){ 
        MT = 18;
        std::vector< double > refXS{ 
          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 18.0, 18.0,
          11.82897, 11.82897, 11.82897, 11.82897, 0.0000334739, 0.0000275176,
          0.0000275176, 0.000027313, 0.000027313, 0.0000271079, 0.0000271079
        };

        auto reaction = reactions.at( MT );
        details::checkRanges( refXS, reaction.crossSections() );

      } // THEN
      THEN( "MT = 51 can be tested" ){ 
        MT = 51;
        std::vector< double > refXS( sizeEnergies, 51.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( MT );
        details::checkRanges( refXS, reaction.crossSections() );
      } // THEN
      THEN( "MT = 52 can be tested" ){ 
        MT = 52;
        std::vector< double > refXS( sizeEnergies, 52.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( MT );
        details::checkRanges( refXS, reaction.crossSections() );
      } // THEN
    } // WHEN
  } // GIVEN
  
  GIVEN( "an RM object" ){
    WHEN( "cross sections have been linearized and resonances reconstructed" ){
      auto [energies, r2d2] = lin_recon( "RM", absTol, relTol );
      auto sizeEnergies = ranges::distance( energies );

      int MT;
      auto reactions = njoy::RECONR::RECONR::summateReactions( 
          std::cout, r2d2, energies );

      THEN( "the reactions can be summed up and checked" ){
        std::vector< int > reference{ 
          1, 2, 3, 4, 16, 18, 51, 52, 102, 875, 876, 877 };

        auto keys = ranges::view::keys( reactions ) | ranges::to_vector;

        CHECK( reference == keys );
      
      } // THEN
      THEN( "MT=1 can be checked" ){
        MT = 1;
        std::vector< double > refXS = sumRanges(
          reactions.at( 2 ).crossSections(), 
          reactions.at( 3 ).crossSections()
        );
      
        auto reaction = reactions.at( MT );
        CHECK( refXS == reaction.crossSections() );
      } // THEN
      THEN( "MT = 2 can be tested" ){
        MT = 2;
        std::vector< double > refXS{
          18.17, 18.17, 18.17, 18.17, 18.17, 18.17, 18.17, 18.17, 18.17,
          18.17, 18.17, 18.17, 18.1699, 18.1699, 18.1699, 18.1699,
          18.1699, 18.1699, 18.1698, 18.1698, 18.1697, 18.1696, 18.1694,
          18.1692, 18.1689, 18.1685, 18.1671, 18.1642, 18.1582, 18.146,
          18.1212, 18.0745, 17.9905, 17.8375, 17.5572, 17.4153, 17.4917,
          17.4205, 17.3396, 17.1909, 16.9417, 16.768, 16.6775, 16.5826,
          16.5634, 16.5131, 16.8464, 17.02, 16.8912, 16.7787, 16.6211,
          16.4759, 16.3538, 16.2227, 16.18, 16.1255, 16.0463, 15.99,
          15.924, 15.9353, 16.2386, 16.5252, 16.5097, 16.4214, 16.3556,
          16.3078, 16.241, 16.1931, 16.1203, 16.0599, 15.9471, 15.8813,
          15.8362, 15.7759, 15.7421, 15.7296, 15.7194, 15.6767, 15.608,
          15.5652, 15.5287, 15.6968, 15.7725, 15.8435, 15.9331, 15.8284,
          15.7176, 15.6097, 15.4893, 15.3362, 15.2434, 15.156, 15.1662,
          15.6013, 16.2867, 16.7619, 16.821, 16.7118, 16.5741, 16.4504,
          16.2624, 16.1328, 15.9657, 15.8571, 15.7056, 15.5815, 15.4505,
          15.2714, 15.1313, 14.8992, 14.7039, 14.3859, 14.1396, 13.7954,
          13.3533, 13.1917, 13.3624, 16.4855, 18.9067, 18.5401, 17.9962,
          17.5562, 17.2252, 16.7809, 16.5025, 16.1737, 15.9824, 15.7587,
          15.6287, 15.5656, 15.5755, 15.4726, 15.3773, 15.2573, 15.1074,
          14.969, 15.2812, 15.8318, 16.281, 16.3641, 16.1787, 16.0281,
          15.9196, 15.7764, 15.6834, 15.6154, 15.5173, 15.4454, 15.3386,
          15.1918, 15.087, 14.9393, 14.8372, 14.7029, 14.6177, 14.5585,
          14.4813, 14.4329, 14.3996, 14.3566, 14.3298, 14.3113, 14.2874,
          14.2723, 14.2618, 14.2478, 14.2386, 14.2318, 14.2219, 14.2146,
          14.2085, 14.1983, 14.1894, 14.1811, 14.1656, 14.1508, 14.1363,
          14.1079, 14.0799, 14.0521, 13.9969, 13.942, 13.8875, 13.7791,
          13.6715, 13.5648, 13.3538, 13.146, 12.9411, 12.5405, 12.4357,
          12.0246, 11.6265, 11.6265, 11.0529, 10.3742, 9.7362, 8.57452,
          7.55322, 6.6588, 5.20189, 5.20189, 4.3465, 3.67218, 2.75436,
          2.25859, 2.12296, 2.04264, 2.01942, 2.00567, 2.00198, 2.00086,
          2.00048, 2.00021, 2.00011, 2.00005, 2.00003, 2.00001, 2, 2, 2,
          2.00002, 2.00006, 2.00013, 2.00022, 2.00049, 2.00086, 2.0019,
          2.00511, 2.00973, 2.02277, 2.06115, 2.28706, 2.48342, 2.57194,
          2.46204, 2.40128, 2.38534, 2.36923, 2.33675, 2.27235, 2.21126,
          2.20399, 2.19681, 2.18275, 2.16911, 2.15592, 2.10805, 2.06873,
          2.05246, 2.03846, 2.02674, 2.01724, 2.01331, 2.00991, 2.00468,
          2.00143, 2.00052, 2.00023, 2.00013, 2.00006, 2.00004, 2.00002,
          2.00001, 2, 2, 2.00001, 2.00002, 2.00005, 2.00007, 2.00011,
          2.00015, 2.0002, 2.00045, 2.00127, 2.00247, 2.00404, 2.00597,
          2.01082, 2.02395, 2.05983, 2.14043, 2.17209, 2.17837, 2.18393,
          2.18457, 2.18489, 2.1852, 2.18551, 2.18582, 2.18612, 2.18643,
          2.18673, 2.18702, 2.18731, 2.1876, 2.18789, 2.18817, 2.18846,
          2.18873, 2.19277, 2.1985, 2.19875, 2.19888, 2.199, 2.19911,
          2.19923, 2.19934, 2.19944, 2.19955, 2.19965, 2.19975, 2.19984,
          2.20003, 2.2002, 2.20075, 2.20081, 2.20086, 2.20091, 2.20096,
          2.20101, 2.20105, 2.20109, 2.20112, 2.20129, 2.20126, 2.20124,
          2.20122, 2.2011, 2.20073, 2.20018, 2.19984, 2.19945, 2.19902,
          2.19856, 2.19843, 2.1983, 2.19818, 2.19804, 2.19791, 2.19777,
          2.19763, 2.19749, 2.16159, 2.10548, 2.10365, 2.10183, 2.1,
          2.09818, 2.09637, 2.09456, 2.09276, 2.09096, 2.08917, 2.08738,
          2.08561, 2.08384, 2.08296, 2.08208, 2.08121, 2.08033, 2.07946,
          2.0786, 2.07773, 2.07687, 2.05101, 2.03966, 2.039, 2.03833,
          2.03767, 2.03702, 2.03637, 2.03573, 2.03509, 2.03446, 2.03321,
          2.0326, 2.03199, 2.03138, 2.03078, 2.0296, 2.02092, 2.02043,
          2.01994, 2.01946, 2.01898, 2.01805, 2.01759, 2.01714, 2.01372,
          2.00804, 2.00389, 2.00126, 2.00049, 2.00024, 2.00019, 2.00015,
          2.00011, 2.00008, 2.00008, 2.00006, 2.00004, 2.00002, 2.00001,
          2, 2, 2, 2.00001, 2.00002, 2.00003, 2.00005, 2.00007, 2.00012,
          2.00019, 2.00063, 2.00071, 2.00078, 2.00094, 2.00103, 2.00112,
          2.00122, 2.00132, 2.00132, 2.00223, 2.00236, 2.00249, 2.00277,
          2.00292, 2.00306, 2.00337, 2.00627, 2.00627
        };

        auto reaction = reactions.at( MT );
        details::checkRanges( refXS, reaction.crossSections() );
      } // THEN
      THEN( "MT = 4 can be tested" ){
        MT = 4;
        std::vector< double > refXS = sumRanges( 
            reactions.at( 51 ).crossSections(), 
            reactions.at( 52 ).crossSections() );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( MT );
        CHECK( refXS == reaction.crossSections() );

      } // THEN
      THEN( "MT = 16 can be tested" ){ 
        MT = 16;
        std::vector< double > refXS = sumRanges(
          reactions.at( 875 ).crossSections(),
          reactions.at( 876 ).crossSections(),
          reactions.at( 877 ).crossSections() );

        auto reaction = reactions.at( MT );
        CHECK( refXS == reaction.crossSections() );
      } // THEN
      THEN( "MT = 18 can be tested" ){ 
        MT = 18;
        std::vector< double > refXS{
          30374.6, 29467.6, 28637.2, 27873.4, 27167.5, 26512.7, 25903.1,
          25333.6, 24800.2, 24299, 23827.1, 22960.3, 22181.6, 21477.1,
          21477.1, 19036.1, 17274, 14849.7, 13223.7, 12036.2, 10385.5,
          9268.37, 7812.74, 6879.64, 5713.81, 4990.91, 3572.62, 2536.74,
          1790.6, 1257.18, 878.778, 610.197, 414.027, 270.115, 180.791,
          196.675, 142.579, 113.388, 95.3084, 74.932, 57.362, 54.07,
          56.3374, 64.155, 67.0129, 89.704, 111.73, 49.7061, 29.7634,
          22.6738, 18.1326, 16.1068, 14.9333, 14.2522, 14.2715, 14.5498,
          15.5904, 16.9593, 19.9589, 28.3807, 33.3652, 26.8811, 18.0174,
          14.7912, 13.4735, 12.8033, 12.1121, 11.7105, 11.1305, 10.5992,
          9.04657, 7.30338, 6.11296, 6.59973, 9.8862, 14.228, 18.4102,
          20.4639, 24.4972, 28.8624, 36.4463, 59.2031, 58.8323, 56.0219,
          32.6852, 21.8323, 19.1471, 19.9452, 23.6453, 32.5675, 41.4007,
          56.5704, 82.8036, 114.893, 112.824, 79.627, 48.2589, 28.5977,
          17.6188, 11.4929, 5.87903, 3.8114, 2.71883, 2.68868, 3.11531,
          3.59612, 4.12546, 4.87728, 5.48661, 6.55057, 7.53287, 9.40104,
          11.1707, 14.3609, 21.1252, 27.5235, 37.3166, 51.8651, 28.7703,
          17.0452, 11.598, 9.04398, 7.78512, 6.84381, 6.71142, 7.20795,
          8.12161, 10.8558, 15.0915, 24.6041, 24.191, 17.0546, 17.1529,
          20.4584, 29.8851, 54.6829, 106.05, 116.119, 95.3901, 45.1235,
          23.1789, 14.0471, 9.618, 5.67319, 4.00574, 3.12971, 2.24788,
          1.80531, 1.34358, 0.912628, 0.681655, 0.42609, 0.289748,
          0.155498, 0.0947308, 0.0627027, 0.0323019, 0.0191446,
          0.0124288, 0.00624332, 0.00364297, 0.00234012, 0.00115976,
          0.000671128, 0.00042867, 0.000210887, 0.000121468,
          7.73329e-05, 3.78805e-05, 2.17585e-05, 1.38258e-05,
          6.75524e-06, 3.874e-06, 2.45891e-06, 1.1997e-06, 6.87402e-07,
          4.36048e-07, 2.12586e-07, 1.2175e-07, 7.72071e-08,
          3.76259e-08, 2.15436e-08, 1.36595e-08, 6.65544e-09,
          3.81026e-09, 2.41567e-09, 1.17689e-09, 6.73731e-10,
          4.27121e-10, 2.08078e-10, 18, 18, 18, 11.829, 11.829, 11.829,
          11.829, 11.829, 11.829, 11.829, 11.829, 3.34739e-05,
          3.34274e-05, 3.33809e-05, 3.32878e-05, 3.31947e-05,
          3.31482e-05, 3.31017e-05, 3.30784e-05, 3.30551e-05,
          3.30435e-05, 3.30377e-05, 3.30348e-05, 3.30319e-05,
          3.30304e-05, 3.30289e-05, 3.30282e-05, 3.30275e-05,
          3.30268e-05, 3.3026e-05, 3.30253e-05, 3.30246e-05,
          3.30231e-05, 3.30217e-05, 3.30202e-05, 3.30173e-05,
          3.30144e-05, 3.30086e-05, 3.29969e-05, 3.29853e-05,
          3.2962e-05, 3.29155e-05, 3.27294e-05, 3.25432e-05,
          3.23571e-05, 3.19848e-05, 3.18918e-05, 3.18685e-05,
          3.18452e-05, 3.17987e-05, 3.17056e-05, 3.16126e-05,
          3.16009e-05, 3.15893e-05, 3.1566e-05, 3.15428e-05,
          3.15195e-05, 3.14264e-05, 3.13334e-05, 3.12868e-05,
          3.12403e-05, 3.11938e-05, 3.11472e-05, 3.1124e-05,
          3.11007e-05, 3.10542e-05, 3.10076e-05, 3.09844e-05,
          3.09727e-05, 3.09669e-05, 3.09611e-05, 3.09582e-05,
          3.09553e-05, 3.09524e-05, 3.09495e-05, 3.09466e-05,
          3.09437e-05, 3.09407e-05, 3.09378e-05, 3.09349e-05,
          3.0932e-05, 3.09291e-05, 3.09262e-05, 3.09146e-05,
          3.08913e-05, 3.0868e-05, 3.08448e-05, 3.08215e-05, 3.0775e-05,
          3.06819e-05, 3.04958e-05, 3.01235e-05, 2.99374e-05,
          2.98908e-05, 2.98443e-05, 2.98385e-05, 2.98356e-05,
          2.98327e-05, 2.98298e-05, 2.98268e-05, 2.98239e-05,
          2.9821e-05, 2.98181e-05, 2.98152e-05, 2.98123e-05,
          2.98094e-05, 2.98065e-05, 2.98036e-05, 2.98007e-05,
          2.97978e-05, 2.97512e-05, 2.96582e-05, 2.96523e-05,
          2.96494e-05, 2.96465e-05, 2.96436e-05, 2.96407e-05,
          2.96378e-05, 2.96349e-05, 2.9632e-05, 2.96291e-05,
          2.96262e-05, 2.96233e-05, 2.96174e-05, 2.96116e-05,
          2.95884e-05, 2.95855e-05, 2.95825e-05, 2.95796e-05,
          2.95767e-05, 2.95738e-05, 2.95709e-05, 2.9568e-05,
          2.95651e-05, 2.95186e-05, 2.95127e-05, 2.95098e-05,
          2.95069e-05, 2.94953e-05, 2.9472e-05, 2.94488e-05,
          2.94371e-05, 2.94255e-05, 2.94139e-05, 2.94022e-05,
          2.93993e-05, 2.93964e-05, 2.93935e-05, 2.93906e-05,
          2.93877e-05, 2.93848e-05, 2.93819e-05, 2.9379e-05,
          2.90067e-05, 2.86344e-05, 2.86228e-05, 2.86112e-05,
          2.85995e-05, 2.85879e-05, 2.85763e-05, 2.85646e-05,
          2.8553e-05, 2.85414e-05, 2.85297e-05, 2.85181e-05,
          2.85065e-05, 2.84948e-05, 2.8489e-05, 2.84832e-05,
          2.84774e-05, 2.84716e-05, 2.84657e-05, 2.84599e-05,
          2.84541e-05, 2.84483e-05, 2.82621e-05, 2.81691e-05,
          2.81633e-05, 2.81574e-05, 2.81516e-05, 2.81458e-05, 2.814e-05,
          2.81342e-05, 2.81284e-05, 2.81225e-05, 2.81109e-05,
          2.81051e-05, 2.80993e-05, 2.80935e-05, 2.80876e-05,
          2.8076e-05, 2.79829e-05, 2.79771e-05, 2.79713e-05,
          2.79655e-05, 2.79597e-05, 2.7948e-05, 2.79422e-05,
          2.79364e-05, 2.78899e-05, 2.77968e-05, 2.77037e-05,
          2.76107e-05, 2.75641e-05, 2.75409e-05, 2.75351e-05,
          2.75292e-05, 2.75234e-05, 2.75176e-05, 2.75176e-05,
          2.75112e-05, 2.75048e-05, 2.74984e-05, 2.7492e-05,
          2.74856e-05, 2.74792e-05, 2.74729e-05, 2.74665e-05,
          2.74601e-05, 2.74537e-05, 2.74473e-05, 2.74409e-05,
          2.74281e-05, 2.74153e-05, 2.73642e-05, 2.73578e-05,
          2.73514e-05, 2.73386e-05, 2.73322e-05, 2.73258e-05,
          2.73194e-05, 2.7313e-05, 2.7313e-05, 2.72617e-05, 2.72553e-05,
          2.72489e-05, 2.72361e-05, 2.72297e-05, 2.72233e-05,
          2.72105e-05, 2.71079e-05, 2.71079e-05
        };

        auto reaction = reactions.at( MT );
        details::checkRanges( refXS, reaction.crossSections() );
      } // THEN
      THEN( "MT = 51 can be tested" ){ 
        MT = 51;
        std::vector< double > refXS( sizeEnergies, 51.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( MT );
        details::checkRanges( refXS, reaction.crossSections() );
      } // THEN
      THEN( "MT = 52 can be tested" ){ 
        MT = 52;
        std::vector< double > refXS( sizeEnergies, 52.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( MT );
        details::checkRanges( refXS, reaction.crossSections() );
      } // THEN
      THEN( "MT = 102 can be tested" ){ 
        MT = 102;
        std::vector< double > refXS{
          5915.61, 5734.13, 5567.77, 5414.54, 5272.77, 5141.1, 5018.37,
          4903.6, 4795.96, 4694.74, 4599.31, 4423.76, 4265.76, 4122.53,
          4122.53, 3655.26, 3317.95, 2853.82, 2542.48, 2315.08, 1999.02,
          1785.11, 1506.39, 1327.67, 1104.41, 965.906, 694.116, 495.31,
          351.321, 246.433, 168.797, 112.875, 75.5079, 51.2232, 40.6056,
          51.2827, 31.0043, 23.2985, 19.2438, 15.4109, 12.8153, 12.8634,
          13.8455, 16.5412, 17.5337, 26.2096, 42.6957, 25.6779, 16.9795,
          13.1785, 10.3098, 9.14646, 8.86177, 9.6696, 10.4949, 12.2795,
          17.0288, 22.8138, 35.3477, 71.3244, 95.1092, 70.6882, 34.002,
          20.3416, 14.8569, 12.2275, 9.91456, 8.96154, 8.20573, 7.93812,
          7.96811, 8.46485, 9.14011, 10.2678, 10.7113, 10.404, 9.45222,
          9.45907, 10.7438, 12.3269, 15.2668, 26.1406, 26.6563, 26.1529,
          17.7846, 12.9288, 11.3829, 11.4133, 12.8691, 17.1729, 21.9717,
          31.0661, 48.9715, 77.0302, 85.8931, 70.871, 50.9405, 36.3481,
          27.1344, 21.3648, 15.1295, 12.1192, 9.48253, 8.42001, 7.65081,
          7.51666, 7.7913, 8.89895, 10.4355, 14.451, 19.4155, 31.1338,
          44.0079, 69.8322, 131.042, 194.129, 298.6, 505.502, 307.189,
          177.23, 109.438, 73.6256, 53.2245, 32.5834, 23.059, 14.9262,
          11.7052, 9.36352, 8.7692, 8.87186, 8.7676, 8.26168, 8.56854,
          9.46867, 11.642, 17.1629, 28.6144, 30.9813, 26.5341, 15.5057,
          10.6506, 8.6218, 7.63494, 6.75268, 6.37684, 6.17685, 5.96972,
          5.85947, 5.7312, 5.57789, 5.46229, 5.3475, 5.24848, 5.06635,
          4.97619, 4.88892, 4.71766, 4.63223, 4.54741, 4.37846, 4.29402,
          4.20971, 4.04122, 3.95698, 3.87277, 3.70437, 3.62017, 3.53598,
          3.36759, 3.2834, 3.19921, 3.03083, 2.94665, 2.86246, 2.69408,
          2.60989, 2.5257, 2.35732, 2.27313, 2.18895, 2.02057, 1.93638,
          1.85219, 1.68381, 1.59962, 1.51543, 1.34706, 1.26287, 1.17868,
          1.0103, 0.98785, 0.898049, 0.808247, 0.808247, 0.673545,
          0.589356, 0.505167, 0.336789, 0.2526, 0.168411, 3.34739e-05,
          3.34739e-05, 3.34274e-05, 3.33809e-05, 3.32878e-05,
          3.31947e-05, 3.31482e-05, 3.31017e-05, 3.30784e-05,
          3.30551e-05, 3.30435e-05, 3.30377e-05, 3.30348e-05,
          3.30319e-05, 3.30304e-05, 3.30289e-05, 3.30282e-05,
          3.30275e-05, 3.30268e-05, 3.3026e-05, 3.30253e-05, 3.30246e-05,
          3.30231e-05, 3.30217e-05, 3.30202e-05, 3.30173e-05,
          3.30144e-05, 3.30086e-05, 3.29969e-05, 3.29853e-05, 3.2962e-05,
          3.29155e-05, 3.27294e-05, 3.25432e-05, 3.23571e-05,
          3.19848e-05, 3.18918e-05, 3.18685e-05, 3.18452e-05,
          3.17987e-05, 3.17056e-05, 3.16126e-05, 3.16009e-05,
          3.15893e-05, 3.1566e-05, 3.15428e-05, 3.15195e-05, 3.14264e-05,
          3.13334e-05, 3.12868e-05, 3.12403e-05, 3.11938e-05,
          3.11472e-05, 3.1124e-05, 3.11007e-05, 3.10542e-05, 3.10076e-05,
          3.09844e-05, 3.09727e-05, 3.09669e-05, 3.09611e-05,
          3.09582e-05, 3.09553e-05, 3.09524e-05, 3.09495e-05,
          3.09466e-05, 3.09437e-05, 3.09407e-05, 3.09378e-05,
          3.09349e-05, 3.0932e-05, 3.09291e-05, 3.09262e-05, 3.09146e-05,
          3.08913e-05, 3.0868e-05, 3.08448e-05, 3.08215e-05, 3.0775e-05,
          3.06819e-05, 3.04958e-05, 3.01235e-05, 2.99374e-05,
          2.98908e-05, 2.98443e-05, 2.98385e-05, 2.98356e-05,
          2.98327e-05, 2.98298e-05, 2.98268e-05, 2.98239e-05, 2.9821e-05,
          2.98181e-05, 2.98152e-05, 2.98123e-05, 2.98094e-05,
          2.98065e-05, 2.98036e-05, 2.98007e-05, 2.97978e-05,
          2.97512e-05, 2.96582e-05, 2.96523e-05, 2.96494e-05,
          2.96465e-05, 2.96436e-05, 2.96407e-05, 2.96378e-05,
          2.96349e-05, 2.9632e-05, 2.96291e-05, 2.96262e-05, 2.96233e-05,
          2.96174e-05, 2.96116e-05, 2.95884e-05, 2.95855e-05,
          2.95825e-05, 2.95796e-05, 2.95767e-05, 2.95738e-05,
          2.95709e-05, 2.9568e-05, 2.95651e-05, 2.95186e-05, 2.95127e-05,
          2.95098e-05, 2.95069e-05, 2.94953e-05, 2.9472e-05, 2.94488e-05,
          2.94371e-05, 2.94255e-05, 2.94139e-05, 2.94022e-05,
          2.93993e-05, 2.93964e-05, 2.93935e-05, 2.93906e-05,
          2.93877e-05, 2.93848e-05, 2.93819e-05, 2.9379e-05, 2.90067e-05,
          2.86344e-05, 2.86228e-05, 2.86112e-05, 2.85995e-05,
          2.85879e-05, 2.85763e-05, 2.85646e-05, 2.8553e-05, 2.85414e-05,
          2.85297e-05, 2.85181e-05, 2.85065e-05, 2.84948e-05, 2.8489e-05,
          2.84832e-05, 2.84774e-05, 2.84716e-05, 2.84657e-05,
          2.84599e-05, 2.84541e-05, 2.84483e-05, 2.82621e-05,
          2.81691e-05, 2.81633e-05, 2.81574e-05, 2.81516e-05,
          2.81458e-05, 2.814e-05, 2.81342e-05, 2.81284e-05, 2.81225e-05,
          2.81109e-05, 2.81051e-05, 2.80993e-05, 2.80935e-05,
          2.80876e-05, 2.8076e-05, 2.79829e-05, 2.79771e-05, 2.79713e-05,
          2.79655e-05, 2.79597e-05, 2.7948e-05, 2.79422e-05, 2.79364e-05,
          2.78899e-05, 2.77968e-05, 2.77037e-05, 2.76107e-05,
          2.75641e-05, 2.75409e-05, 2.75351e-05, 2.75292e-05,
          2.75234e-05, 2.75176e-05, 2.75176e-05, 2.75112e-05,
          2.75048e-05, 2.74984e-05, 2.7492e-05, 2.74856e-05, 2.74792e-05,
          2.74729e-05, 2.74665e-05, 2.74601e-05, 2.74537e-05,
          2.74473e-05, 2.74409e-05, 2.74281e-05, 2.74153e-05,
          2.73642e-05, 2.73578e-05, 2.73514e-05, 2.73386e-05,
          2.73322e-05, 2.73258e-05, 2.73194e-05, 2.7313e-05, 2.7313e-05,
          2.72617e-05, 2.72553e-05, 2.72489e-05, 2.72361e-05,
          2.72297e-05, 2.72233e-05, 2.72105e-05, 2.71079e-05,
          2.71079e-05
        };

        auto reaction = reactions.at( MT );
        details::checkRanges( refXS, reaction.crossSections() );
      } // THEN
    } // WHEN
  } // GIVEN
} // SCENARIO
SCENARIO( "Testing the unionization of the energy Grid" ){
  // These are the same regardless of the resonance formalism
  std::vector< double > userSupplied{ 1.0, 2.0, 3.0 };
  auto energies = XSEnergies();
  WHEN( "resonances have not been reconstructed" ){
    
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (SLBW) object" ){
      auto material = details::ENDFMaterial( "SLBW" );
      auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      njoy::RECONR::RECONR::linearizeXS( 
        std::cout, r2d2, absTolerance, relTolerance );
    
      THEN( "the energygrid can be unionized" ){
        std::vector< double > refGrid{
          0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5
        };
        refGrid |= ranges::action::push_back( energies );
        refGrid |= ranges::action::push_back( userSupplied );
        ranges::sort( refGrid );
    
        refGrid = ranges::view::unique( refGrid );
    
        auto trial = njoy::RECONR::RECONR::unionizeEnergyGrid( 
            std::cout, r2d2, userSupplied );
    
        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (RM) object" ){
      auto material = details::ENDFMaterial( "RM" );
      auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      njoy::RECONR::RECONR::linearizeXS(
        std::cout, r2d2, absTolerance, relTolerance );
    
      THEN( "the energygrid can be unionized" ){
        std::vector< double > refGrid { 
          0.98596, 1.06389, 1.1389, 1.21391, 2.02109, 2.0361, 2.05111, 
          2.74827, 2.7767, 2.80513, 3.14517, 3.1566, 3.16803, 3.60111, 
          3.6208, 3.64049, 4.8336, 4.8508, 4.868, 5.24932, 5.4497, 5.5
        };
        refGrid |= ranges::action::push_back( energies );
        refGrid |= ranges::action::push_back( userSupplied );
        ranges::sort( refGrid );
    
        refGrid = ranges::view::unique( refGrid );
    
        auto trial = njoy::RECONR::RECONR::unionizeEnergyGrid( 
            std::cout, r2d2, userSupplied );
    
        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (RML) object" ){
      auto material = details::ENDFMaterial( "RML" );
      auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      njoy::RECONR::RECONR::linearizeXS( 
        std::cout, r2d2, absTolerance, relTolerance );
    
      THEN( "the energygrid can be unionized" ){
        std::vector< double > refGrid { 
          -1.9E+6, -1.223300e+6, 7.788000e+3, 5.152000e+4, 5.359000e+4, 5.5E5 
        };
        refGrid |= ranges::action::push_back( energies );
        refGrid |= ranges::action::push_back( userSupplied );
        ranges::sort( refGrid );
    
        refGrid = ranges::view::unique( refGrid );
    
        auto trial = njoy::RECONR::RECONR::unionizeEnergyGrid( 
          std::cout, r2d2, userSupplied );
    
        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
  } // WHEN

  WHEN( "resonance have been reconstructed" ){
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (SLBW) object" ){
      auto material = details::ENDFMaterial( "SLBW" );
      auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      njoy::RECONR::RECONR::linearizeXS(
        std::cout, r2d2, absTolerance, relTolerance );
      auto refGrid = njoy::RECONR::RECONR::unionizeEnergyGrid( 
          std::cout, r2d2, userSupplied );
      njoy::RECONR::RECONR::reconstructResonances(
        std::cout, refGrid, r2d2, 1E-1, 1E-3 );
    
      THEN( "the reconstructed energygrid can be unionized" ){

        // refGrid is already verified above. Just need to add the
        // reconstructed/linearized cross sections
        for( const auto& [MT, V] : r2d2.reconstructedResonances() ){
          for( const auto& XS : V ){
            refGrid |= ranges::action::push_back( XS.x() );
          }
        }
        ranges::sort( refGrid );
        refGrid = ranges::view::unique( refGrid );

        auto trial = njoy::RECONR::RECONR::unionizeEnergyGrid( std::cout, r2d2 );

        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (RM) object" ){
      auto material = details::ENDFMaterial( "RM" );
      auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      njoy::RECONR::RECONR::linearizeXS( 
        std::cout, r2d2, absTolerance, relTolerance );
      auto refGrid = njoy::RECONR::RECONR::unionizeEnergyGrid( 
          std::cout, r2d2, userSupplied );
      njoy::RECONR::RECONR::reconstructResonances(
        std::cout, refGrid, r2d2, 1E-1, 1E-3 );
    
      THEN( "the reconstructed energygrid can be unionized" ){

        // refGrid is already verified above. Just need to add the
        // reconstructed/linearized cross sections
        for( const auto& [MT, V] : r2d2.reconstructedResonances() ){
          for( const auto& XS : V ){
            refGrid |= ranges::action::push_back( XS.x() );
          }
        }
        ranges::sort( refGrid );
        refGrid = ranges::view::unique( refGrid );

        auto trial = njoy::RECONR::RECONR::unionizeEnergyGrid( 
          std::cout, r2d2 );

        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
    
  } // WHEN
} // SCENARIO
*/
