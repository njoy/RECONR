#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/printV.hpp"
#include "RECONR/details/nextMin.hpp"
#include "RECONR/details/checkRanges.hpp"

using namespace njoy;
using namespace njoy::elementary;

class tRECONR: protected njoy::RECONR::RECONR {

  using RECONR = njoy::RECONR::RECONR;

public:
  using RECONR::RECONR;
  using RECONR::operator();
  using RECONR::findR2D2;
  using RECONR::linearizeXS;
  using RECONR::reconstructResonances;
  using RECONR::unionizeEnergyGrid;
  using RECONR::summateReactions;
  using RECONR::summateProductions;
  using RECONR::truncateReactions;
  using RECONR::getEvaluated;
  using RECONR::reconstructCrossSections;
  using RECONR::combineReconstructed;
};

nlohmann::json input{R"({
  "nendf": 20, "npend": 22,
  "tlabel": "Modern RECONR Testing",
  "sequence": [
    {
      "mat": 125, "ncards": 1, "ngrid": 3,
      "err": 0.001, "tempr": 0, "errmax": 0.1, "errint": 5E-7,
      "cards": [ "Material 125 processed with modern RECONR" ],
      "enode": [ 1.0, 2.0, 3.0 ]
    },
    {
      "mat": 2631, "ncards": 2, "ngrid": 0,
      "err": 0.001, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
      "cards": [ "Material 2631 processed with modern RECONR",
                  "For testing purposes only." ],
      "enode": [ ]
    }
  ]
})"_json};
nlohmann::json Fe56Input{R"({
  "nendf": 20, "npend": 23,
  "tlabel": "Modern RECONR Testing",
  "sequence": [
    {
      "mat": 2631, "ncards": 2, "ngrid": 0,
      "err": 0.001, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
      "cards": [ "Material 2631 processed with modern RECONR",
                  "For testing purposes only." ],
      "enode": [ ]
    }
  ]
})"_json};
nlohmann::json inputWithU235{R"({
  "nendf": 20, "npend": 24,
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
    },
    {
      "mat": 9228, "ncards": 1, "ngrid": 0,
      "err": 0.1, "tempr": 0.0, "errmax": 2.1, "errint": 8E-7,
      "cards": [ "Material 9228 processed with modern RECONR" ],
      "enode": [ ]
    }
  ]
})"_json};

template< typename K >
void printKeys( K& keys ){
  njoy::Log::info( "" );
  njoy::Log::info( "{:20s}", "ref" );
  for( const auto& key : keys ){
    Log::info( "\t{:20s}", key.symbol() );
  }
}

template< typename R, typename T >
void printKeys( R& ref, T& trial ){
  njoy::Log::info( "" );
  njoy::Log::info( "{:20s}\t{:20s}", "ref", "trial" );
  for( const auto& [r, t] : ranges::view::zip( ref, trial ) ){
    njoy::Log::info( "{:20s}\t{:20s}", r.symbol(), t.symbol() );
  }
}
std::vector< double> XSEnergies(){

  std::vector< double > Qvalues{ 322.4648, 3.224648E6 };
  // std::vector< double > MT1{
  //   1.0E-5, 
  //   details::nextMin( 2.0E-5 ), 2.0E-5,
  //   details::nextMin( 7.5E+5 ), 7.5E+5,
  //   details::nextMin( 1.9E+7 ), 1.9E+7,
  //   details::nextMin( 1.95E+7 ),1.95E+7,
  //   details::nextMin( 2.0E+7 ), 2.0E+7 };
  // std::vector< double > MT16{
  //   1e-05,       1.25e-05,    1.5e-05,     2e-05,       
  //   2.5457e-05,  3.09139e-05, 4.18279e-05, 5.27418e-05, 6.36557e-05,  
  //   8.54836e-05, 0.000107311, 0.000150967, 0.000194623, 0.000281934,  
  //   0.000369246, 0.000718492, 0.00141698,  0.00281397,  0.00560794,   
  //   0.0111959,   0.0223717,   0.0447235,   0.089427,    0.178834,     
  //   0.357648,    0.715276,    1.43053,     2.86104,     5.72207,      
  //   11.4441,     22.8882,     45.7764,     91.5528,     183.105,      
  //   366.211,     732.422,     1464.84,     2929.69,     5859.38,      
  //   11718.8,     23437.5,     46875,       93750,       187500,       
  //   375000,      750000,      1.9e+07,     1.95e+07,    2e+07 };
  // std::vector< double > MT18{ 
  //   1.0E+5,
  //   details::nextMin( 1.5E+5 ), 1.5E+5,
  //   details::nextMin( 7.5E+5 ), 7.5E+5,
  //   1.9E+7, 1.95E+7, 2.0E+7 };
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
  std::vector< double > MF13MT3{
    1.00E-5, 1.00E-1, 1.09E+6, 1.09E+6, 2.00E+6, 5.00E+6,
    5.50E+6, 6.00E+6, 6.50E+6, 7.00E+6, 7.50E+6, 8.00E+6,
    1.42E+7, 2.00E+7,                      
  };
                                                                  
  std::vector< double > MF13MT18{ 1.0E-5, 3.E7 };

  return ranges::view::concat( Qvalues,
                               // MT1, MT16, MT18, 
                               MT102, 
                               MF13MT3, MF13MT18 ) 
    | ranges::to_vector;

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
  auto r2d2 = njoy::RECONR::R2D2::Factory()( material );

  tRECONR::linearizeXS( std::cout, r2d2, absTol, relTol);
  auto refGrid = tRECONR::unionizeEnergyGrid(
    std::cout, 
    r2d2.reactions(), 
    r2d2.photonProductions(), 
    r2d2.resonanceReferenceGrid(),
    userSupplied );

  tRECONR::reconstructResonances( 
    std::cout, refGrid, r2d2, relTol, absTol );

  auto energies = tRECONR::unionizeEnergyGrid(
    std::cout, refGrid, r2d2.reconstructedResonances() );

  tRECONR::reconstructCrossSections( std::cout, std::cout, r2d2, energies );
  tRECONR::combineReconstructed( std::cout, std::cout, r2d2, energies );

  return std::make_pair( energies, r2d2 );
}

/*
SCENARIO( "Testing creation of RECONR class" ){
  GIVEN( "a JSON object, and extra arguments" ){

    auto args = nlohmann::json::object();

    // WHEN( "a RECONR object is called" ){
    //   CHECK_NOTHROW( tRECONR()( input, 
    //                             std::cout, 
    //                             std::cerr, 
    //                             args ) );
    // } // WHEN
    // WHEN( "processing one ENDF Material in a Tape" ){
    //   CHECK_NOTHROW( tRECONR()( Fe56Input, 
    //                             std::cout, 
    //                             std::cerr, 
    //                             args ) );
    // } // WHEN
    // WHEN( "processing H-1, Fe-56, and U-235" ){
    //   CHECK_NOTHROW( tRECONR()( inputWithU235, 
    //                             std::cout, 
    //                             std::cerr, 
    //                             args ) );
    // } // WHEN
  } // GIVEN

  WHEN( "calling the modern RECONR interface" ){
    nlohmann::json modern{ R"({
      "input": "tape20",
      "output": "Fe56.modern.pendf",
      "tolerance": 0.001,
      "energy grid": []
    })"_json };

    CHECK_THROWS( tRECONR()( std::cout, std::cerr, modern ) );

  } // WHEN
} // SCENARIO
SCENARIO( "Getting evaluated data" ){
  WHEN( "Getting an existant ENDF Tape" ){
    auto evaluatedData = tRECONR::getEvaluated(
      std::cout, "tape20" );

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
      CHECK_THROWS( tRECONR::getEvaluated( std::cout, "tape45" ) );
    } // THEN

  } // WHEN
} // SCENARIO
SCENARIO( "Testing the linearization of collected cross sections" ){
  GIVEN( "an ResonanceReconstructionDataDelivery object" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( material );

    auto projectile = r2d2.projectile();
    auto target = r2d2.target();

    WHEN( "the cross sections are linearized" ){
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS( 
        std::cout, r2d2, absTolerance, relTolerance );

      auto reactions = r2d2.reactions();

      using PID = ParticleID;
      using NID = NucleusID;

      std::vector< njoy::RECONR::ReactionID > refKeys{ 
        ReactionID{ projectile, target, ReactionType{ 2 } },
        ReactionID{ projectile, target, ReactionType{ 18 } },
        ReactionID{ projectile, target, ReactionType{ 51 } },
        ReactionID{ projectile, target, ReactionType{ 52 } },
        ReactionID{ projectile, target, ReactionType{ 102 } },
        ReactionID{ projectile, target, ReactionType{ 875 } },
        ReactionID{ projectile, target, ReactionType{ 876 } },
        ReactionID{ projectile, target, ReactionType{ 877 } }
      };
      auto keys = ranges::view::keys( reactions ) | ranges::to_vector;
      std::sort( refKeys.begin(), refKeys.end() );
      std::sort( keys.begin(), keys.end() );
      CHECK( ranges::equal( refKeys, keys ) );

      auto skeys = keys | ranges::view::transform( []( auto&& k ){ return k.symbol(); } )
          | ranges::to_vector;
      auto sref = refKeys | ranges::view::transform( []( auto&& k ){ return k.symbol(); } )
          | ranges::to_vector;
      
      using Rxn_t = njoy::RECONR::interp::LinearTable;
      // njoy::RECONR::ReactionID MT;
      THEN( "we can check MT=18" ){
        ReactionID reactionID{ projectile, target, ReactionType{ 18 } };
        auto reaction = reactions.at( reactionID );
        std::vector< double > refE{ 
          1.0E+5,
          details::nextMin( 1.5E+5 ), 1.5E+5,
          details::nextMin( 7.5E+5 ), 7.5E+5,
          1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refB{ 
          1.8E+1      , 1.8E+1      , 
          1.182897E+1 , 1.182897E+1 , 
          3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto lXS = reaction.crossSections< Rxn_t >();
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
        ReactionID reactionID{ projectile, target, ReactionType{ 102 } };
        auto reaction = reactions.at( reactionID );
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

        auto lXS = reaction.crossSections< Rxn_t >();
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
SCENARIO( "Testing the unionization of the energy Grid" ){
  // These are the same regardless of the resonance formalism
  std::vector< double > userSupplied{ 1.0, 2.0, 3.0 };
  auto energies = XSEnergies();
  WHEN( "resonances have not been reconstructed" ){
    
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (SLBW) object" ){
      auto material = details::ENDFMaterial( "SLBW" );
      auto r2d2 = njoy::RECONR::R2D2::Factory()( material );
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS( std::cout, r2d2, absTolerance, relTolerance );
    
      THEN( "the energygrid can be unionized" ){
        std::vector< double > refGrid{
          1e-05,        1.0625e-05,   1.125e-05,    1.1875e-05,   1.25e-05,     
          1.3125e-05,   1.375e-05,    1.4375e-05,   1.5e-05,      1.5625e-05,   
          1.625e-05,    1.75e-05,     1.875e-05,    2e-05,        2e-05,        
          6.36557e-05,  0.000107311,  0.000194623,  0.000369246,  0.000718492,  
          0.00141698,   0.00281397,   0.00560794,   0.0111959,    0.0223717,    
          0.0447235,    0.089427,     0.1,          0.178834,     0.357648,     
          0.715276,     0.986069,     1,            1.0253,       1.06453,      
          1.43053,      2,            2.86104,      2.98607,      3,            
          3.0253,       3.06453,      5.72207,      7.5,          7.5,          
          11.4441,      22.8882,      45.7764,      91.5528,      183.105,      
          322.465,      366.211,      732.422,      1464.84,      2929.69,      
          5859.38,      11718.8,      23437.5,      46875,        93750,        
          100000,       150000,       150000,       187500,       375000,       
          750000,       750000,       1.09e+06,     2e+06,        3.22465e+06,  
          5e+06,        5.5e+06,      6e+06,        6.5e+06,      7e+06,        
          7.5e+06,      8e+06,        1.42e+07,     1.9e+07,      1.9e+07,      
          1.95e+07,     1.95e+07,     2e+07,        2e+07,        3e+07
        };
    
        auto trial = tRECONR::unionizeEnergyGrid( 
          std::cout, 
          r2d2.reactions(), 
          r2d2.photonProductions(), 
          r2d2.resonanceReferenceGrid(),
          userSupplied );
    
        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (RM) object" ){
      auto material = details::ENDFMaterial( "RM" );
      auto r2d2 = njoy::RECONR::R2D2::Factory()( material );
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS(
        std::cout, r2d2, absTolerance, relTolerance );
    
      THEN( "the energygrid can be unionized" ){
        std::vector< double > refGrid { 
          1e-05,        1.0625e-05,   1.125e-05,    1.1875e-05,   1.25e-05,     
          1.3125e-05,   1.375e-05,    1.4375e-05,   1.5e-05,      1.5625e-05,   
          1.625e-05,    1.75e-05,     1.875e-05,    2e-05,        2e-05,        
          6.36557e-05,  0.000107311,  0.000194623,  0.000369246,  0.000718492,  
          0.00141698,   0.00281397,   0.00560794,   0.0111959,    0.0223717,    
          0.0447235,    0.089427,     0.1,          0.178834,     0.357648,     
          0.715276,     0.98596,      1,            1.06389,      1.1389,       
          1.21391,      1.43053,      2,            2.02109,      2.0361,       
          2.05111,      2.74827,      2.7767,       2.80513,      2.86104,      
          3,            3.14517,      3.1566,       3.16803,      3.60111,      
          3.6208,       3.64049,      4.8336,       4.8508,       4.868,        
          5.24932,      5.4497,       5.5,          5.5,          5.72207,      
          11.4441,      22.8882,      45.7764,      91.5528,      183.105,      
          322.465,      366.211,      732.422,      1464.84,      2929.69,      
          5859.38,      11718.8,      23437.5,      46875,        93750,        
          100000,       150000,       150000,       187500,       375000,       
          750000,       750000,       1.09e+06,     2e+06,        3.22465e+06,  
          5e+06,        5.5e+06,      6e+06,        6.5e+06,      7e+06,        
          7.5e+06,      8e+06,        1.42e+07,     1.9e+07,      1.9e+07,      
          1.95e+07,     1.95e+07,     2e+07,        2e+07,        3e+07
        };
    
        auto trial = tRECONR::unionizeEnergyGrid( 
          std::cout, 
          r2d2.reactions(), 
          r2d2.photonProductions(), 
          r2d2.resonanceReferenceGrid(),
          userSupplied );
    
        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (RML) object" ){
      auto material = details::ENDFMaterial( "RML" );
      auto r2d2 = njoy::RECONR::R2D2::Factory()( material );
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS( 
        std::cout, r2d2, absTolerance, relTolerance );
    
      THEN( "the energygrid can be unionized" ){
        std::vector< double > refGrid { 
          1e-05,        1.0625e-05,   1.125e-05,    1.1875e-05,   1.25e-05,     
          1.3125e-05,   1.375e-05,    1.4375e-05,   1.5e-05,      1.5625e-05,   
          1.625e-05,    1.75e-05,     1.875e-05,    2e-05,        2e-05,        
          6.36557e-05,  0.000107311,  0.000194623,  0.000369246,  0.000718492,  
          0.00141698,   0.00281397,   0.00560794,   0.0111959,    0.0223717,    
          0.0447235,    0.089427,     0.1,          0.178834,     0.357648,     
          0.715276,     1,            1.43053,      2,            2.86104,      
          3,            5.72207,      11.4441,      22.8882,      45.7764,      
          91.5528,      183.105,      322.465,      366.211,      732.422,      
          1464.84,      2929.69,      5859.38,      7788,         11718.8,      
          23437.5,      46875,        51520,        53590,        93750,        
          100000,       150000,       150000,       187500,       375000,       
          550000,       550000,       750000,       750000,       1.09e+06,     
          2e+06,        3.22465e+06,  5e+06,        5.5e+06,      6e+06,        
          6.5e+06,      7e+06,        7.5e+06,      8e+06,        1.42e+07,     
          1.9e+07,      1.9e+07,      1.95e+07,     1.95e+07,     2e+07,        
          2e+07,        3e+07
        };
    
        auto trial = tRECONR::unionizeEnergyGrid( 
          std::cout, 
          r2d2.reactions(), 
          r2d2.photonProductions(), 
          r2d2.resonanceReferenceGrid(),
          userSupplied );
    
        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
  } // WHEN

  WHEN( "resonances have been reconstructed" ){
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (SLBW) object" ){
      auto material = details::ENDFMaterial( "SLBW" );
      auto r2d2 = njoy::RECONR::R2D2::Factory()( material );
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS(
        std::cout, r2d2, absTolerance, relTolerance );
      auto refGrid = tRECONR::unionizeEnergyGrid( 
          std::cout, 
          r2d2.reactions(), 
          r2d2.photonProductions(), 
          r2d2.resonanceReferenceGrid(),
          userSupplied );
      auto energies = refGrid;
      tRECONR::reconstructResonances(
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

        auto trial = tRECONR::unionizeEnergyGrid( 
          std::cout, 
          energies,
          r2d2.reconstructedResonances() );

        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
    GIVEN( "a linearized ResonanceReconstructionDataDelivery (RM) object" ){
      auto material = details::ENDFMaterial( "RM" );
      auto r2d2 = njoy::RECONR::R2D2::Factory()( material );
    
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      tRECONR::linearizeXS( 
        std::cout, r2d2, absTolerance, relTolerance );
      auto refGrid = tRECONR::unionizeEnergyGrid( 
          std::cout, 
          r2d2.reactions(), 
          r2d2.photonProductions(), 
          r2d2.resonanceReferenceGrid(),
          userSupplied );
      auto energies = refGrid;
      tRECONR::reconstructResonances(
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

        auto trial = tRECONR::unionizeEnergyGrid( 
          std::cout, 
          energies,
          r2d2.reconstructedResonances() );

        details::checkRanges( refGrid, trial );
      } // THEN
    } // GIVEN
  } // WHEN
} // SCENARIO
*/
/*
SCENARIO( "Testing the resonance reconstruction" ){
  GIVEN( "an SLBW R2D2 object and reference grid" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( material );

    auto proj = r2d2.projectile();
    auto target = r2d2.target();

    auto eID = ReactionID{ proj, target, ReactionType{ 2 } };
    auto fID = ReactionID{ proj, target, ReactionType{ 19 } };
    auto cID = ReactionID{ proj, target, ReactionType{ 102 } };

    const std::vector< double > refGrid{
      0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5
    };

    WHEN( "the resonances are reconstructed" ){
      tRECONR::reconstructResonances(
        std::cout, refGrid, r2d2, 1E-1, 1E-3 );

      THEN( "the linearized reconstruction can be verified" ){
        auto reconstructed = r2d2.reconstructedResonances();

        auto keys = ranges::view::keys( reconstructed );

        std::vector< double > refEnergies{ 
          1e-05, 2.50461e-05, 4.00921e-05, 7.01843e-05, 0.000130369, 0.000250737,
          0.000491474, 0.000972948, 0.0019359, 0.00386179, 0.00771359, 0.0154172,
          0.0308244, 0.0616387, 0.123267, 0.246525, 0.49304, 0.616297,
          0.739554, 0.801183, 0.862812, 0.893626, 0.92444, 0.955255,
          0.986069, 1.0253, 1.06453, 1.09455, 1.12458, 1.1546,
          1.18463, 1.24468, 1.30472, 1.42482, 1.54492, 1.66501,
          1.78511, 2.0253, 2.26549, 2.50568, 2.62578, 2.74588,
          2.80592, 2.86597, 2.896, 2.92602, 2.95605, 2.98607,
          3.0253, 3.06453, 3.08186, 3.09918, 3.13384, 3.16849,
          3.20314, 3.27244, 3.34175, 3.48036, 3.61896, 3.75757,
          3.89618, 4.1734, 4.72783, 5.28227, 6.39113, 7.5
        };
        std::vector< double > refElastic{
          0.12506,   0.125059,  0.125059,  0.125058,   0.125057,   0.125054,    
          0.125048,  0.125036,  0.125014,  0.124971,   0.124893,   0.124769,    
          0.124651,  0.124929,  0.127583,  0.142374,   0.239419,   0.376941,    
          0.731633,  1.15853,   2.12402,   3.132,      5.03349,    9.1272,      
          19.0871,   38.1473,   19.0819,   9.28208,    5.16956,    3.23402,     
          2.19952,   1.20137,   0.756675,  0.38362,    0.235867,   0.163379,    
          0.12328,   0.0861085, 0.0790813, 0.10576,    0.151698,   0.274666,    
          0.423839,  0.760705,  1.11065,   1.76604,    3.15938,    6.48055,     
          12.9402,   6.47874,   4.21545,   2.85963,    1.51044,    0.9191,      
          0.616025,  0.333129,  0.21033,   0.108652,   0.0682899,  0.0480401,   
          0.0363148, 0.0237135, 0.0133756, 0.00901362, 0.00516115, 0.00343646
        };
        std::vector< double > refCapture{ 
          6850.24,  4328.48,  3421.17,  2585.72,  1897.19,   1367.98,   
          977.059,  694.372,  492.186,  348.377,  246.372,   174.131,   
          123.081,  87.2921,  63.1015,  49.8161,  58.9968,   82.7983,   
          146.211,  222.108,  391.906,  567.549,  896.41,    1598.51,   
          3289.5,   6446.72,  3165.14,  1518.74,  834.808,   515.696,   
          346.513,  185.012,  114.139,  55.8541,  33.4273,   22.7445,   
          17.0292,  12.114,   11.835,   17.1509,  25.4053,   46.9056,   
          72.6816,  130.496,  190.301,  301.974,  538.661,   1100.86,   
          2185.28,  1086.47,  704.507,  476.198,  249.594,   150.619,   
          100.063,  53.0942,  32.8511,  16.2643,  9.79166,   6.60478,   
          4.79638,  2.91043,  1.45495,  0.89217,  0.443622,  0.265993
        };
        std::vector< double > refFission( 66, 0.0 );

        auto elastic = reconstructed.at( eID ).front();
        auto capture = reconstructed.at( cID ).front();
        auto fission = reconstructed.at( fID ).front();
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
    auto r2d2 = njoy::RECONR::R2D2::Factory()( material );

    auto proj = r2d2.projectile();
    auto target = r2d2.target();

    auto eID = ReactionID{ proj, target, ReactionType{ 2 } };
    auto fID = ReactionID{ proj, target, ReactionType{ 19 } };
    auto cID = ReactionID{ proj, target, ReactionType{ 102 } };

      std::vector< double > refGrid { 
        0.98596, 1.06389, 1.1389, 1.21391, 2.02109, 2.0361, 2.05111, 
        2.74827, 2.7767, 2.80513, 3.14517, 3.1566, 3.16803, 3.60111, 
        3.6208, 3.64049, 4.8336, 4.8508, 4.868, 5.24932, 5.4497, 5.5
      };

    WHEN( "the resonances are reconstructed" ){
      tRECONR::reconstructResonances( 
          std::cout, refGrid, r2d2, 1E-1, 1E-3 );

      THEN( "the linearized reconstruction can be verified" ){
        auto reconstructed = r2d2.reconstructedResonances();

        auto keys = ranges::view::keys( reconstructed );

        std::vector< double > refEnergies{ 
          0.98596,  1.02493,  1.06389,  1.1389,   1.21391,  
          1.26436,  1.31481,  1.4157,   1.6175,   1.8193,   
          1.86974,  1.92019,  1.94542,  1.97064,  1.98325,  
          1.99587,  2.00848,  2.02109,  2.0361,   2.05111,  
          2.0729,   2.09468,  2.11647,  2.13826,  2.18183,  
          2.2254,   2.31255,  2.39969,  2.57398,  2.66113,  
          2.7047,   2.74827,  2.7767,   2.80513,  2.89014,  
          2.97515,  3.01765,  3.06016,  3.14517,  3.1566,   
          3.16803,  3.22216,  3.2763,   3.33044,  3.38457,  
          3.43871,  3.49284,  3.51991,  3.54697,  3.57404,  
          3.60111,  3.6208,   3.64049,  3.65913,  3.67777,  
          3.69642,  3.71506,  3.75234,  3.78963,  3.8642,   
          3.93877,  4.08791,  4.23705,  4.38618,  4.53532,  
          4.60989,  4.68446,  4.72175,  4.75903,  4.77767,  
          4.79632,  4.81496,  4.82428,  4.8336,   4.8508,   
          4.868,    4.87992,  4.89183,  4.90375,  4.91567,  
          4.9395,   4.96333,  5.01099,  5.05866,  5.15399,  
          5.24932,  5.4497,   5.5
        };
        std::vector< double > refElastic{ 
          14.5826,  14.5327,  14.5131,  14.8464,  15.0201,  
          14.8999,  14.7925,  14.6392,  14.4366,  14.2619,  
          14.2091,  14.1403,  14.0929,  14.0281,  13.9863,  
          13.939,   13.9007,  13.9354,  14.2386,  14.5251,  
          14.5097,  14.4214,  14.3556,  14.3078,  14.241,   
          14.1931,  14.1203,  14.0599,  13.9471,  13.8813,  
          13.8362,  13.7759,  13.7421,  13.7296,  13.7054,  
          13.6351,  13.5874,  13.539,   13.6968,  13.7725,  
          13.8435,  13.9331,  13.8284,  13.7176,  13.6097,  
          13.4893,  13.3362,  13.2434,  13.156,   13.1662,  
          13.6013,  14.2867,  14.7618,  14.821,   14.7118,  
          14.5741,  14.4504,  14.2624,  14.1328,  13.9657,  
          13.8571,  13.7056,  13.5815,  13.4505,  13.2714,  
          13.1313,  12.8992,  12.7039,  12.3859,  12.1396,  
          11.7954,  11.3532,  11.1917,  11.3625,  14.4855,  
          16.9067,  16.5402,  15.9963,  15.5563,  15.2253,  
          14.781,   14.5025,  14.1737,  13.9824,  13.7587,  
          13.6287,  13.5656,  13.5755
        };
        std::vector< double > refCapture{ 
          64.155,   73.7218,  89.705,   111.73,   49.7073,  
          30.5546,  23.3012,  18.4774,  15.6896,  14.3587,  
          14.2416,  14.443,   14.8764,  15.9617,  17.0766,  
          19.0255,  22.548,   28.3832,  33.3652,  26.8838,  
          18.0184,  14.7916,  13.4737,  12.8033,  12.1121,  
          11.7105,  11.1305,  10.5992,  9.04656,  7.30333,  
          6.11291,  6.59994,  9.8862,   14.2276,  19.2682,  
          22.5998,  26.3267,  33.37,    59.2035,  58.8323,  
          56.0236,  32.6866,  21.8327,  19.1471,  19.9451,  
          23.6452,  32.5672,  41.4005,  56.5703,  82.8037,  
          114.893,  112.824,  79.6278,  48.2594,  28.5979,  
          17.619,   11.493,   5.87904,  3.8114,   2.71883,  
          2.68868,  3.11531,  3.59612,  4.12546,  4.87729,  
          5.48662,  6.5506,   7.53293,  9.40118,  11.1709,  
          14.3614,  21.1263,  27.5253,  37.3192,  51.8651,  
          28.7732,  17.0467,  11.5986,  9.04432,  7.7853,   
          6.84385,  6.71141,  7.2079,   8.12153,  10.8557,  
          15.0913,  24.6041,  24.191
        };
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
          10.8557, 15.0913, 24.6041, 24.191 
        };

        auto elastic = reconstructed.at( eID ).front();
        auto capture = reconstructed.at( fID ).front();
        // auto fission = reconstructed.at( cID ).front();
        details::checkRanges( refEnergies, elastic.x( ) );
        details::checkRanges( refElastic, elastic.y() );
        details::checkRanges( refEnergies, capture.x( ) );
        details::checkRanges( refCapture, capture.y() );
        // details::checkRanges( refEnergies, fission.x( ) );
        // details::checkRanges( refFission, fission.y() );
        
      } // THEN
      
    } // WHEN
    
  } // GIVEN
  GIVEN( "an RML R2D2 object and reference grid" ){
    auto material = details::ENDFMaterial( "RML" );
    auto r2d2 = njoy::RECONR::R2D2::Factory()( material );

    auto proj = r2d2.projectile();
    auto target = r2d2.target();

    auto eID = ReactionID{ proj, target, ReactionType{ 2 } };
    auto fID = ReactionID{ proj, target, ReactionType{ 19 } };
    auto cID = ReactionID{ proj, target, ReactionType{ 102 } };

    std::vector< double > grid{ 1e-05, 7788, 51520, 53590, 550000 };
    
    WHEN( "the resonances are reconstructed" ){
      tRECONR::reconstructResonances( 
          std::cout, grid, r2d2, 1E-1, 1E-3 );

      auto reconstructed = r2d2.reconstructedResonances();

      std::vector< njoy::elementary::ReactionID > refIDs{ eID, cID };
      auto IDs = ranges::view::keys( reconstructed ) | ranges::to_vector;
      std::sort( refIDs.begin(), refIDs.end() );
      std::sort( IDs.begin(), IDs.end() );

      REQUIRE( ranges::equal( refIDs, IDs ) );

      std::vector< double > refE{ 1e-05, 7788, 51520, 53590, 550000 };
      std::map< njoy::RECONR::ReactionID, std::vector< double > > refXS{
        { cID, 
          { 69.0745, 0.424143, 1.12865, 3.75695, 3.04552e-07 } },
        { eID,
          { 1.19111, 342.429, 57.3075, 49.6444, 3.8478 } }
      };

      for( const auto& id : IDs ){
        THEN( "the reconstructed cross section can be verified" +
              " for reaction: " + id.symbol() ){
          auto reaction = reconstructed.at( id ).front();

          details::checkRanges( refE, reaction.x() );
          details::checkRanges( refXS.at( id ), reaction.y() );
          
        } // THEN
      }
    } // WHEN
  } // GIVEN
  
} // SCENARIO
*/
SCENARIO( "Testing the summation of cross sections" ){
  double absTol{ 1E-6 };
  double relTol{ 1E-1 }; // This tolerance is large by design

  using RPair = njoy::RECONR::XSPair;
  using PPair = njoy::RECONR::PPair;

  GIVEN( "an SLBW object" ){
    auto [energies, r2d2] = lin_recon( "SLBW", absTol, relTol );
    auto sizeEnergies = ranges::distance( energies );

    WHEN( "cross sections have been summated" ){
      tRECONR::summateReactions( std::cout, std::cout, r2d2, energies );

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      auto reactions = r2d2.reactions();
      auto summations = r2d2.summations();

      THEN( "the reactions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > refReactions{ 
          ReactionID{ projectile, target, ReactionType{ 2 } },
          ReactionID{ projectile, target, ReactionType{ 18 } },
          ReactionID{ projectile, target, ReactionType{ 51 } },
          ReactionID{ projectile, target, ReactionType{ 52 } },
          ReactionID{ projectile, target, ReactionType{ 102 } },
          ReactionID{ projectile, target, ReactionType{ 875 } },
          ReactionID{ projectile, target, ReactionType{ 876 } },
          ReactionID{ projectile, target, ReactionType{ 877 } }
        };
        auto rxKeys = ranges::view::keys( reactions ) | ranges::to_vector;
        std::sort( refReactions.begin(), refReactions.end() );
        std::sort( rxKeys.begin(), rxKeys.end() );

        printKeys( refReactions, rxKeys );

        CHECK( ranges::equal( refReactions, rxKeys ) );

        std::vector< njoy::RECONR::ReactionID > refSummations{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 4 } },
          ReactionID{ projectile, target, ReactionType{ 16 } },
          ReactionID{ projectile, target, ReactionType{ 27 } },
          ReactionID{ projectile, target, ReactionType{ 101 } }
        };
        auto smKeys = ranges::view::keys( summations ) | ranges::to_vector;
        std::sort( refSummations.begin(), refSummations.end() );
        std::sort( smKeys.begin(), smKeys.end() );

        printKeys( refSummations, smKeys );

        CHECK( ranges::equal( refSummations, smKeys ) );

      } // THEN
      THEN( "MT = 1 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 1 } };

        std::vector< int > redundants = { 2, 4, 16, 27, 101 };
        auto mt2 = ReactionID{ projectile, target, ReactionType{ 2 } };
        auto mt4 = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt16 = ReactionID{ projectile, target, ReactionType{ 16 } };
        auto mt27 = ReactionID{ projectile, target, ReactionType{ 27 } };
        auto mt101 = ReactionID{ projectile, target, ReactionType{ 101 } };
        std::vector< double  > refXS = sumRanges(
          reactions.at(  mt2   ).template crossSections< RPair >().second,
          summations.at( mt4   ).template crossSections< RPair >().second,
          summations.at( mt16  ).template crossSections< RPair >().second,
          summations.at( mt27  ).template crossSections< RPair >().second,
          summations.at( mt101 ).template crossSections< RPair >().second
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 2 can be tested" ){ 
        auto capture = ReactionID{ projectile, target, ReactionType{ 2 } };
        std::vector< double > refXS{
          2.12506,  2.12506,  2.12506,  2.12506,  2.12506,  
          2.12506,  2.12506,  2.12506,  2.12506,  2.12506,  
          2.12506,  2.12506,  2.12506,  2.12506,  2.12506,  
          2.12506,  2.12506,  2.12506,  2.12506,  2.12506,  
          2.12506,  2.12506,  2.12506,  2.12506,  2.12505,  
          2.12505,  2.12504,  2.12503,  2.12499,  2.12493,  
          2.12483,  2.12469,  2.12469,  2.12577,  2.12625,  
          2.13254,  2.17054,  2.27584,  2.39337,  2.6277,   
          2.99912,  3.85593,  4.77109,  6.54644,  10.5524,  
          14.5282,  21.0871,  28.9515,  40.1473,  21.0819,  
          12.8938,  8.71798,  6.47505,  5.16996,  3.81534,  
          3.17209,  2.60656,  2.37352,  2.21458,  2.14388,  
          2.08834,  2.07845,  2.09168,  2.11455,  2.16429,  
          2.28823,  2.42846,  2.71949,  3.05579,  3.69336,  
          5.07632,  8.48055,  11.1456,  14.9402,  8.47874,  
          5.88827,  4.48489,  3.22304,  2.71756,  2.47115,  
          2.24985,  2.15672,  2.1089,   2.08104,  2.05131,  
          2.03639,  2.02772,  2.0183,   2.01043,  2.00704,  
          2.00472,  2.00344,  2.003436, 2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        0
        };

        auto reaction = reactions.at( capture );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 4 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt51 = ReactionID{ projectile, target, ReactionType{ 51 } };
        auto mt52 = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS = sumRanges( 
            reactions.at( mt51 ).template crossSections< RPair >().second, 
            reactions.at( mt52 ).template crossSections< RPair >().second );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 16 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 16 } };
        std::vector< double > refXS( sizeEnergies, 1.5 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 18 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 18 } };
        std::vector< double > refXS{
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            18,           18,           
          11.829,       11.829,       11.829,       11.829,       3.34739e-05,  
          3.3363e-05,   3.3066e-05,   3.26663e-05,  3.20868e-05,  3.19236e-05,  
          3.17605e-05,  3.15973e-05,  3.14341e-05,  3.12709e-05,  3.11077e-05,  
          2.90842e-05,  2.75176e-05,  2.75176e-05,  2.7313e-05,   2.7313e-05,   
          2.71079e-05,  2.71079e-05,  0
        };

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 51 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 51 } };
        std::vector< double > refXS( sizeEnergies, 51.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 52 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS( sizeEnergies, 52.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 102 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 102 } };
        std::vector< double > refXS{
          6952.24,      6739.82,      6545.15,      6365.86,      6200.01,      
          6046,         5902.48,      5768.28,      5642.45,      5524.13,      
          5412.61,      5207.49,      5022.93,      4855.67,      4855.67,      
          4305.16,      3907.76,      3360.98,      2994.22,      2726.35,      
          2354.07,      2102.12,      1773.88,      1563.45,      1300.63,      
          1137.67,      818.15,       585.098,      417.606,      298.116,      
          213.173,      152.912,      110.329,      80.7439,      77.0078,      
          61.8575,      56.5561,      71.9926,      92.5391,      134.368,      
          200.545,      351.623,      511.454,      819.044,      1506.89,      
          2184.75,      3296.11,      4618.73,      6453.3,       3171.71,      
          1794.73,      1098.14,      726.554,      511.711,      290.587,      
          186.802,      96.9413,      60.7023,      36.624,       26.2712,      
          18.6415,      17.7697,      20.6723,      24.8943,      33.7429,      
          55.3442,      79.5513,      129.501,      186.994,      295.659,      
          530.621,      1106.91,      1556.59,      2191.32,      1092.5,       
          655.408,      419.217,      207.478,      123.004,      81.9893,      
          45.3585,      30.0722,      22.2935,      17.8033,      13.0743,      
          10.7481,      9.4208,       8.01459,      6.873,        6.37959,      
          6.06902,      5.88624,      5.88624,      5.38812,      5.05137,      
          4.71461,      4.37786,      4.0411,       3.7848,       3.70435,      
          3.36759,      3.03083,      2.69408,      2.35732,      2.02057,      
          1.68381,      1.34706,      1.0103,       0.98785,      0.808247,     
          0.808247,     0.673545,     0.336789,     3.34739e-05,  3.34739e-05,  
          3.3363e-05,   3.3066e-05,   3.26663e-05,  3.20868e-05,  3.19236e-05,  
          3.17605e-05,  3.15973e-05,  3.14341e-05,  3.12709e-05,  3.11077e-05,  
          2.90842e-05,  2.75176e-05,  2.75176e-05,  2.7313e-05,   2.7313e-05,   
          2.71079e-05,  2.71079e-05,  0.0
        };
        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
    } // WHEN
    WHEN( "productions have been linearized and summarized" ){
      
      tRECONR::summateProductions( std::cout, std::cout, r2d2, energies );

      auto& productions = r2d2.photonProductions();

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      THEN( "the productions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > reference{
          ReactionID{ projectile, target, ReactionType{ 3 } },
          ReactionID{ projectile, target, ReactionType{ 18 } },
        };
        auto keys = ranges::view::keys( productions ) | ranges::to_vector;
        std::sort( reference.begin(), reference.end() );
        std::sort( keys.begin(), keys.end() );

        printKeys( reference, keys );

        CHECK( ranges::equal( reference, keys ) );
        THEN( "MT = 3 can be tested" ){
          auto ID = ReactionID{ projectile, target, ReactionType{ 3 } };
          std::vector< double > refP{
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.3, 2.70822, 3.3,
            4.3, 5.3, 6.3, 7.3, 8.3, 9.3, 10.3, 
            11.1276, 11.1276, 11.2138, 11.2138, 11.3, 11.3, 0.0
            };

          auto production = productions.at( ID ).productions< PPair >()
            | ranges::view::transform( []( auto&& p ){ return p.second; } )
            | ranges::to_vector;
          details::checkRanges( refP, production );
        } // THEN
        THEN( "MT = 18 can be tested" ){
          auto ID = ReactionID{ projectile, target, ReactionType{ 18 } };
          std::vector< double > refP{
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    10.18,    
            10.18,    10.18,    10.18,    10.18,    10.1801,  10.1802,  10.1804,  
            10.1808,  10.1816,  10.1831,  10.1833,  10.185,   10.185,   10.1863,  
            10.1925,  10.205,   10.205,   10.2163,  10.2467,  10.2875,  10.3467,  
            10.3633,  10.38,    10.3967,  10.4133,  10.43,    10.4467,  10.6533,  
            10.8133,  10.8133,  10.83,    10.83,    10.8467,  10.8467,  11.18
        };

          auto production = productions.at( ID ).productions< PPair >()
            | ranges::view::transform( []( auto&& p ){ return p.second; } )
            | ranges::to_vector;
          details::checkRanges( refP, production );
        } // THEN
      } // THEN
    } // WHEN
  } // GIVEN
  GIVEN( "an RM object" ){
    auto [energies, r2d2] = lin_recon( "RM", absTol, relTol );
    auto sizeEnergies = ranges::distance( energies );

    WHEN( "cross sections have been summated" ){
      tRECONR::summateReactions( std::cout, std::cout, r2d2, energies );

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      auto reactions = r2d2.reactions();
      auto summations = r2d2.summations();

      THEN( "the reactions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > refReactions{ 
          ReactionID{ projectile, target, ReactionType{ 2 } },
          ReactionID{ projectile, target, ReactionType{ 51 } },
          ReactionID{ projectile, target, ReactionType{ 52 } },
          ReactionID{ projectile, target, ReactionType{ 102 } },
          ReactionID{ projectile, target, ReactionType{ 875 } },
          ReactionID{ projectile, target, ReactionType{ 876 } },
          ReactionID{ projectile, target, ReactionType{ 877 } }
        };
        auto rxKeys = ranges::view::keys( reactions ) | ranges::to_vector;
        std::sort( refReactions.begin(), refReactions.end() );
        std::sort( rxKeys.begin(), rxKeys.end() );

        printKeys( refReactions, rxKeys );

        CHECK( ranges::equal( refReactions, rxKeys ) );

        std::vector< njoy::RECONR::ReactionID > refSummations{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 3 } },
          ReactionID{ projectile, target, ReactionType{ 4 } },
          ReactionID{ projectile, target, ReactionType{ 16 } },
          ReactionID{ projectile, target, ReactionType{ 18 } },
          ReactionID{ projectile, target, ReactionType{ 27 } },
          ReactionID{ projectile, target, ReactionType{ 101 } }
        };
        auto smKeys = ranges::view::keys( summations ) | ranges::to_vector;
        std::sort( refSummations.begin(), refSummations.end() );
        std::sort( smKeys.begin(), smKeys.end() );

        printKeys( refReactions, rxKeys );

        CHECK( ranges::equal( refSummations, rxKeys ) );

      } // THEN
      THEN( "MT=1 can be checked" ){
        auto ID = ReactionID{ projectile, target, ReactionType{ 1 } };
        auto capture = ReactionID{ projectile, target, ReactionType{ 2 } };
        auto nonelastic = ReactionID{ projectile, target, ReactionType{ 3 } };
        std::vector< double > refXS = sumRanges(
          reactions.at( capture ).template crossSections< RPair >().second, 
          summations.at( nonelastic ).template crossSections< RPair >().second
        );
      
        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 2 can be tested" ){
        auto capture = ReactionID{ projectile, target, ReactionType{ 2 } };
        std::vector< double > refXS{
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        16.5826,  16.5634,  
          16.5131,  16.8464,  17.02,    16.8912,  16.7787,  
          16.6211,  16.4759,  16.3538,  16.2227,  16.18,    
          16.1255,  16.0463,  15.99,    15.924,   15.9353,  
          16.2386,  16.5252,  16.5097,  16.4214,  16.3556,  
          16.3078,  16.241,   16.1931,  16.1203,  16.0599,  
          15.9471,  15.8813,  15.8362,  15.7759,  15.7421,  
          15.7296,  15.7194,  15.6767,  15.608,   15.5652,  
          15.5287,  15.6968,  15.7725,  15.8435,  15.9331,  
          15.8284,  15.7176,  15.6097,  15.4893,  15.3362,  
          15.2434,  15.156,   15.1662,  15.6013,  16.2867,  
          16.7619,  16.821,   16.7118,  16.5741,  16.4504,  
          16.2624,  16.1328,  15.9657,  15.8571,  15.7056,  
          15.5815,  15.4505,  15.2714,  15.1313,  14.8992,  
          14.7039,  14.3859,  14.1396,  13.7954,  13.3533,  
          13.1917,  13.3624,  16.4855,  18.9067,  18.5401,  
          17.9962,  17.5562,  17.2252,  16.7809,  16.5025,  
          16.1737,  15.9824,  15.7587,  15.6287,  15.5656,  
          15.5755,  15.5755,  2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        2,        2,        2,        
          2,        2,        0
        };

        auto reaction = reactions.at( capture );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 4 can be tested" ){
        auto ID = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt51 = ReactionID{ projectile, target, ReactionType{ 51 } };
        auto mt52 = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS = sumRanges(
          reactions.at( mt51 ).template crossSections< RPair >().second, 
          reactions.at( mt52 ).template crossSections< RPair >().second
        );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 16 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 16 } };
        auto mt875 = ReactionID{ projectile, target, ReactionType{ 875 } };
        auto mt876 = ReactionID{ projectile, target, ReactionType{ 876 } };
        auto mt877 = ReactionID{ projectile, target, ReactionType{ 877 } };
        std::vector< double > refXS = sumRanges(
          reactions.at( mt875 ).template crossSections< RPair >().second,
          reactions.at( mt876 ).template crossSections< RPair >().second,
          reactions.at( mt877 ).template crossSections< RPair >().second );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 18 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 18 } };
        std::vector< double > refXS{
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            64.155,       67.0129,      
          89.704,       111.73,       49.7061,      29.7634,      22.6738,      
          18.1326,      16.1068,      14.9333,      14.2522,      14.2715,      
          14.5498,      15.5904,      16.9593,      19.9589,      28.3807,      
          33.3652,      26.8811,      18.0174,      14.7912,      13.4735,      
          12.8033,      12.1121,      11.7105,      11.1305,      10.5992,      
          9.04657,      7.30338,      6.11296,      6.59973,      9.8862,       
          14.228,       18.4102,      20.4639,      24.4972,      28.8624,      
          36.4463,      59.2031,      58.8323,      56.0219,      32.6852,      
          21.8323,      19.1471,      19.9452,      23.6453,      32.5675,      
          41.4007,      56.5704,      82.8036,      114.893,      112.824,      
          79.627,       48.2589,      28.5977,      17.6188,      11.4929,      
          5.87903,      3.8114,       2.71883,      2.68868,      3.11531,      
          3.59612,      4.12546,      4.87728,      5.48661,      6.55057,      
          7.53287,      9.40104,      11.1707,      14.3609,      21.1252,      
          27.5235,      37.3166,      51.8651,      28.7703,      17.0452,      
          11.598,       9.04398,      7.78512,      6.84381,      6.71142,      
          7.20795,      8.12161,      10.8558,      15.0915,      24.6041,      
          24.191,       24.191,       0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            18,           18,           
          11.829,       11.829,       11.829,       11.829,       3.34739e-05,  
          3.3363e-05,   3.3066e-05,   3.26663e-05,  3.20868e-05,  3.19236e-05,  
          3.17605e-05,  3.15973e-05,  3.14341e-05,  3.12709e-05,  3.11077e-05,  
          2.90842e-05,  2.75176e-05,  2.75176e-05,  2.7313e-05,   2.7313e-05,   
          2.71079e-05,  2.71079e-05,  0
        };

        auto reaction = summations.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 51 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 51 } };
        std::vector< double > refXS( sizeEnergies, 51.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 52 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS( sizeEnergies, 52.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 102 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 102 } };
        std::vector< double > refXS{
          102,          94.1134,      86.6777,      79.6441,      72.9714,      
          66.6243,      60.5726,      54.7899,      49.2533,      43.9428,      
          38.8406,      29.2,         20.2247,      11.829,       11.829,       
          11.7587,      11.6884,      11.5477,      11.4071,      11.2665,      
          11.1396,      11.0128,      10.8681,      10.7235,      10.568,       
          10.4124,      10.089,       9.75904,      9.42573,      9.0907,       
          8.75481,      8.41849,      8.08195,      7.7453,       7.70549,      
          7.4086,       7.07187,      6.73513,      16.5412,      17.5337,      
          26.2096,      42.6957,      25.6779,      16.9795,      13.1785,      
          10.3098,      9.14646,      8.86177,      9.6696,       10.4949,      
          12.2795,      17.0288,      22.8138,      35.3477,      71.3244,      
          95.1092,      70.6882,      34.002,       20.3416,      14.8569,      
          12.2275,      9.91456,      8.96154,      8.20573,      7.93812,      
          7.96811,      8.46485,      9.14011,      10.2678,      10.7113,      
          10.404,       9.45222,      9.45907,      10.7438,      12.3269,      
          15.2668,      26.1406,      26.6563,      26.1529,      17.7846,      
          12.9288,      11.3829,      11.4133,      12.8691,      17.1729,      
          21.9717,      31.0661,      48.9715,      77.0302,      85.8931,      
          70.871,       50.9405,      36.3481,      27.1344,      21.3648,      
          15.1295,      12.1192,      9.48253,      8.42001,      7.65081,      
          7.51666,      7.7913,       8.89895,      10.4355,      14.451,       
          19.4155,      31.1338,      44.0079,      69.8322,      131.042,      
          194.129,      298.6,        505.502,      307.189,      177.23,       
          109.438,      73.6256,      53.2245,      32.5834,      23.059,       
          14.9262,      11.7052,      9.36352,      8.7692,       8.87186,      
          8.7676,       8.7676,       5.72488,      5.38812,      5.05137,      
          4.71461,      4.37786,      4.0411,       3.7848,       3.70435,      
          3.36759,      3.03083,      2.69408,      2.35732,      2.02057,      
          1.68381,      1.34706,      1.0103,       0.98785,      0.808247,     
          0.808247,     0.673545,     0.336789,     3.34739e-05,  3.34739e-05,  
          3.3363e-05,   3.3066e-05,   3.26663e-05,  3.20868e-05,  3.19236e-05,  
          3.17605e-05,  3.15973e-05,  3.14341e-05,  3.12709e-05,  3.11077e-05,  
          2.90842e-05,  2.75176e-05,  2.75176e-05,  2.7313e-05,   2.7313e-05,   
          2.71079e-05,  2.71079e-05,  0
        };
        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
    } // WHEN
  } // GIVEN
  GIVEN( "an RML object" ){
    auto [energies, r2d2] = lin_recon( "RML", absTol, relTol );
    auto sizeEnergies = ranges::distance( energies );

    WHEN( "cross sections have been summated" ){
      tRECONR::summateReactions( std::cout, std::cout, r2d2, energies );

      auto projectile = r2d2.projectile();
      auto target = r2d2.target();

      auto reactions = r2d2.reactions();
      auto summations = r2d2.summations();

      THEN( "the reactions can be summed up and checked" ){
        std::vector< njoy::RECONR::ReactionID > refReactions{ 
          ReactionID{ projectile, target, ReactionType{ 2 } },
          ReactionID{ projectile, target, ReactionType{ 51 } },
          ReactionID{ projectile, target, ReactionType{ 52 } },
          ReactionID{ projectile, target, ReactionType{ 102 } },
          ReactionID{ projectile, target, ReactionType{ 875 } },
          ReactionID{ projectile, target, ReactionType{ 876 } },
          ReactionID{ projectile, target, ReactionType{ 877 } }
        };
        auto rxKeys = ranges::view::keys( reactions ) | ranges::to_vector;
        std::sort( refReactions.begin(), refReactions.end() );
        std::sort( rxKeys.begin(), rxKeys.end() );

        printKeys( refReactions, rxKeys );

        CHECK( ranges::equal( refReactions, rxKeys ) );

        std::vector< njoy::RECONR::ReactionID > refSummations{ 
          ReactionID{ projectile, target, ReactionType{ 1 } },
          ReactionID{ projectile, target, ReactionType{ 3 } },
          ReactionID{ projectile, target, ReactionType{ 4 } },
          ReactionID{ projectile, target, ReactionType{ 16 } },
          ReactionID{ projectile, target, ReactionType{ 18 } },
          ReactionID{ projectile, target, ReactionType{ 27 } },
          ReactionID{ projectile, target, ReactionType{ 101 } }
        };
        auto smKeys = ranges::view::keys( summations ) | ranges::to_vector;
        std::sort( refSummations.begin(), refSummations.end() );
        std::sort( smKeys.begin(), smKeys.end() );

        printKeys( refReactions, rxKeys );

        CHECK( ranges::equal( refSummations, rxKeys ) );

      } // THEN
      THEN( "MT = 1 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 1 } };
        auto capture = ReactionID{ projectile, target, ReactionType{ 2 } };
        auto nonelastic = ReactionID{ projectile, target, ReactionType{ 3 } };
        std::vector< double > refXS = sumRanges(
          reactions.at( capture ).template crossSections< RPair >().second, 
          summations.at( nonelastic ).template crossSections< RPair >().second
        );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 2 can be tested" ){ 
        auto capture = ReactionID{ projectile, target, ReactionType{ 2 } };
        std::vector< double > refXS( sizeEnergies, 2.0 );
        refXS.back() = 0.0;

        auto reaction = reactions.at( capture );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 4 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 4 } };
        auto mt51 = ReactionID{ projectile, target, ReactionType{ 51 } };
        auto mt52 = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS = sumRanges( 
            reactions.at( mt51 ).template crossSections< RPair >().second, 
            reactions.at( mt52 ).template crossSections< RPair >().second );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 16 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 16 } };
        std::vector< double > refXS( sizeEnergies, 1.5 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = summations.at( ID );
        CHECK( refXS == reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 18 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 18 } };
        std::vector< double > refXS{
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            0,            0,            0,            
          0,            0,            18,           18,           11.829,       
          11.829,       11.829,       11.829,       11.829,       11.829,       
          3.34739e-05,  3.3363e-05,   3.3066e-05,   3.26663e-05,  3.20868e-05,  
          3.19236e-05,  3.17605e-05,  3.15973e-05,  3.14341e-05,  3.12709e-05,  
          3.11077e-05,  2.90842e-05,  2.75176e-05,  2.75176e-05,  2.7313e-05,   
          2.7313e-05,   2.71079e-05,  2.71079e-05,  0
        };

        auto reaction = summations.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );

      } // THEN
      THEN( "MT = 51 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 51 } };
        std::vector< double > refXS( sizeEnergies, 51.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 52 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 52 } };
        std::vector< double > refXS( sizeEnergies, 52.0 );
        zeroXS( energies, refXS, 1E5, 2E7 );

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );
      } // THEN
      THEN( "MT = 102 can be tested" ){ 
        auto ID = ReactionID{ projectile, target, ReactionType{ 102 } };
        std::vector< double > refXS{
          102,          94.1134,      86.6777,      79.6441,      72.9714,      
          66.6243,      60.5726,      54.7899,      49.2533,      43.9428,      
          38.8406,      29.2,         20.2247,      11.829,       11.829,       
          11.7587,      11.6884,      11.5477,      11.4071,      11.2665,      
          11.1396,      11.0128,      10.8681,      10.7235,      10.568,       
          10.4124,      10.089,       9.75904,      9.42573,      9.0907,       
          8.75481,      8.41849,      8.08195,      7.7453,       7.70549,      
          7.4086,       7.07187,      6.73513,      6.60108,      6.39838,      
          6.26433,      6.06163,      6.04528,      5.72488,      5.38812,      
          5.05137,      4.71461,      4.37786,      4.0411,       3.7848,       
          3.70435,      3.36759,      3.03083,      2.69408,      2.35732,      
          2.24648,      2.02057,      1.68381,      1.34706,      1.31369,      
          1.29881,      1.0103,       0.98785,      0.808247,     0.808247,     
          0.673545,     0.336789,     0.179636,     0.179636,     3.34739e-05,  
          3.34739e-05,  3.3363e-05,   3.3066e-05,   3.26663e-05,  3.20868e-05,  
          3.19236e-05,  3.17605e-05,  3.15973e-05,  3.14341e-05,  3.12709e-05,  
          3.11077e-05,  2.90842e-05,  2.75176e-05,  2.75176e-05,  2.7313e-05,   
          2.7313e-05,   2.71079e-05,  2.71079e-05,  0
        };

        auto reaction = reactions.at( ID );
        details::checkRanges( refXS, reaction.crossSections< RPair >().second );

      } // THEN
    } // WHEN
  } // GIVEN
} // SCENARIO
