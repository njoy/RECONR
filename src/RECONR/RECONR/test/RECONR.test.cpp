#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "range/v3/algorithm/equal.hpp"

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

njoy::RECONR::Logger logger{ std::cout, std::cerr };

template< typename K >
void printKeys( K& keys ){
  njoy::Log::info( "" );
  njoy::Log::info( "{:20s}", "ref" );
  for( const auto& key : keys ){
    Log::info( "\t{:20s}", key.symbol() );
  }
}

template< typename R, typename T >
void printKeys( R&& ref, T&& trial ){
  njoy::Log::info( "" );
  njoy::Log::info( "{:25s} {:25s}", "ref", "trial" );
  for( const auto& [r, t] : ranges::view::zip( ref, trial ) ){
    auto rMT = elementary::toEndfReactionNumber( r );
    auto tMT = elementary::toEndfReactionNumber( t );
    njoy::Log::info( "{:3} {:25s} {:3} {:25s}", rMT, r.symbol(), 
                                                tMT, t.symbol() );
  }
}
std::vector< double> XSEnergies(){

  std::vector< double > Qvalues{ 322.4648, 3.224648E6 };
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
auto lin_recon( std::string formalism, double absTol, double relTol, 
                int LSSF = 0 ){
  std::vector< double > userSupplied{ 1.0, 2.0, 3.0 };
  auto material = details::ENDFMaterial( formalism, LSSF );
  auto r2d2 = njoy::RECONR::R2D2::Factory()( logger, material );

  tRECONR::linearizeXS( logger, r2d2, absTol, relTol);
  auto refGrid = tRECONR::unionizeEnergyGrid( logger, r2d2, userSupplied );

  tRECONR::reconstructResonances( logger, refGrid, r2d2, relTol, absTol );

  auto energies = tRECONR::unionizeEnergyGrid(
    logger, refGrid, 
    r2d2.resolvedRange(), r2d2.unresolvedRange(),
    r2d2.reconstructedResonances() );

  tRECONR::reconstructCrossSections( logger, r2d2, energies );
  tRECONR::combineReconstructed( logger, r2d2, energies );

  return std::make_pair( energies, r2d2 );
}

void printReactions( const njoy::RECONR::R2D2& data ){
  for( const auto& [ ID, reaction ] : data.reactions() ){
    auto mt = elementary::toEndfReactionNumber( ID );
    details::printV( fmt::format( "{}: MT={}", ID.symbol(), mt ),
                     reaction.crossSections< njoy::RECONR::XSPair >().second );
  }
}
void printSummations( const njoy::RECONR::R2D2& data ){
  for( const auto& [ ID, reaction ] : data.summations() ){
    auto mt = elementary::toEndfReactionNumber( ID );
    details::printV( fmt::format( "{}: MT={}", ID.symbol(), mt ),
                     reaction.crossSections< njoy::RECONR::XSPair >().second );
  }
}


#include "RECONR/RECONR/test/constructor.hpp"
#include "RECONR/RECONR/test/combineReconstructed.hpp"
#include "RECONR/RECONR/test/evaluatedData.hpp"
#include "RECONR/RECONR/test/linearization.hpp"
#include "RECONR/RECONR/test/resonanceReconstruction.hpp"
#include "RECONR/RECONR/test/summateReactions.hpp"
#include "RECONR/RECONR/test/unionization.hpp"

