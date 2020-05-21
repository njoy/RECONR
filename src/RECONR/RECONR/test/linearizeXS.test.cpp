#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/printV.hpp"
#include "RECONR/details/nextMin.hpp"

SCENARIO( "Testing the linearization of collected cross sections" ){
  GIVEN( "an ResonanceReconstructionDataDelivery object" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    WHEN( "the cross sections are linearized" ){
      double absTolerance{ 1E-6 };
      double relTolerance{ 1E-1 }; // This tolerance is large by design
      njoy::RECONR::RECONR::linearizeXS( r2d2, absTolerance, relTolerance );

      auto rXSs = r2d2.crossSections();
      auto lXSs = r2d2.linearCrossSections();

      auto keys = ranges::view::keys( rXSs );
      CHECK( ranges::equal( keys, ranges::view::keys( lXSs ) ) );
      
      THEN( "we can check MT=1" ){
        auto lxs1 = lXSs.at( 1 );
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

        auto energies = lxs1.x() | ranges::to_vector;
        auto barns = lxs1.y() | ranges::to_vector;
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
        auto lxs16 = lXSs.at( 16 );
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

        auto energies = lxs16.x() | ranges::to_vector;
        auto barns = lxs16.y() | ranges::to_vector;
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
        auto lxs18 = lXSs.at( 18 );
        std::vector< double > refE{ 
          1.0E+5,
          details::nextMin( 1.5E+5 ), 1.5E+5,
          details::nextMin( 7.5E+5 ), 7.5E+5,
          1.9E+7, 1.95E+7, 2.0E+7 };
        std::vector< double > refB{ 
          1.8E+1      , 1.8E+1      , 
          1.182897E+1 , 1.182897E+1 , 
          3.347392E-5, 2.751761E-5, 2.731301E-5, 2.710792E-5 };

        auto energies = lxs18.x() | ranges::to_vector;
        auto barns = lxs18.y() | ranges::to_vector;
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
        auto lxs102 = lXSs.at( 102 );
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

        auto energies = lxs102.x() | ranges::to_vector;
        auto barns = lxs102.y() | ranges::to_vector;
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
