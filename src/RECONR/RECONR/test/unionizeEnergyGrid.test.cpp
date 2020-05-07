#include "catch.hpp"

#include "RECONR.hpp"

#include "RECONR/details/simpleENDFTestString.hpp"
#include "RECONR/details/checkRanges.hpp"
#include "RECONR/details/printV.hpp"
#include "RECONR/details/nextMin.hpp"

std::vector< double> XSEnergies();

SCENARIO( "Testing the unionization of the energy Grid" ){
  // These are the same regardless of the resonance formalism
  std::vector< double > userSupplied{ 1.0, 2.0, 3.0 };
  auto energies = XSEnergies();

  GIVEN( "a linearized ResonanceReconstructionDataDelivery (SLBW) object" ){
    auto material = details::ENDFMaterial( "SLBW" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    double absTolerance{ 1E-6 };
    double relTolerance{ 1E-1 }; // This tolerance is large by design
    njoy::RECONR::RECONR::linearizeXS( r2d2, absTolerance, relTolerance );

    THEN( "the energygrid can be unionized" ){
      std::vector< double > refGrid{
        0.00001, 0.9860692, 1.0253, 1.0645308, 2.9860692, 3.0253, 3.0645308, 7.5
      };
      refGrid |= ranges::action::push_back( energies );
      refGrid |= ranges::action::push_back( userSupplied );
      std::sort( refGrid.begin(), refGrid.end() );

      refGrid = ranges::view::unique( refGrid );

      auto trial = njoy::RECONR::RECONR::unionizeEnergyGrid( 
          r2d2, userSupplied );

      details::checkRanges( refGrid, trial );
    } // THEN
  } // GIVEN
  GIVEN( "a linearized ResonanceReconstructionDataDelivery (RM) object" ){
    auto material = details::ENDFMaterial( "RM" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    double absTolerance{ 1E-6 };
    double relTolerance{ 1E-1 }; // This tolerance is large by design
    njoy::RECONR::RECONR::linearizeXS( r2d2, absTolerance, relTolerance );

    THEN( "the energygrid can be unionized" ){
      std::vector< double > refGrid { 
        0.98596, 1.06389, 1.1389, 1.21391, 2.02109, 2.0361, 2.05111, 
        2.74827, 2.7767, 2.80513, 3.14517, 3.1566, 3.16803, 3.60111, 
        3.6208, 3.64049, 4.8336, 4.8508, 4.868, 5.24932, 5.4497, 5.5
      };
      refGrid |= ranges::action::push_back( energies );
      refGrid |= ranges::action::push_back( userSupplied );
      std::sort( refGrid.begin(), refGrid.end() );

      refGrid = ranges::view::unique( refGrid );

      auto trial = njoy::RECONR::RECONR::unionizeEnergyGrid( 
          r2d2, userSupplied );

      details::checkRanges( refGrid, trial );
    } // THEN
  } // GIVEN
  GIVEN( "a linearized ResonanceReconstructionDataDelivery (RML) object" ){
    auto material = details::ENDFMaterial( "RML" );
    auto r2d2 = njoy::RECONR::R2D2::Factory( std::move( material ) )();

    double absTolerance{ 1E-6 };
    double relTolerance{ 1E-1 }; // This tolerance is large by design
    njoy::RECONR::RECONR::linearizeXS( r2d2, absTolerance, relTolerance );

    THEN( "the energygrid can be unionized" ){
      std::vector< double > refGrid { 
        -1.9E+6, -1.223300e+6, 7.788000e+3, 5.152000e+4, 5.359000e+4, 5.5E5 
      };
      refGrid |= ranges::action::push_back( energies );
      refGrid |= ranges::action::push_back( userSupplied );
      std::sort( refGrid.begin(), refGrid.end() );

      refGrid = ranges::view::unique( refGrid );

      auto trial = njoy::RECONR::RECONR::unionizeEnergyGrid( 
        r2d2, userSupplied );

      details::checkRanges( refGrid, trial );
    } // THEN
  } // GIVEN
} // SCENARIO

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

  // details::printV( "MT1", MT1 );
  // details::printV( "MT16", MT16 );
  // details::printV( "MT18", MT18 );
  // details::printV( "MT102", MT102 );


  return ranges::view::concat( MT1, MT16, MT18, MT102 ) | ranges::to_vector;

}
