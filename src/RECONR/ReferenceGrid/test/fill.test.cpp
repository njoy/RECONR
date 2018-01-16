#include <iostream>

#include "catch.hpp"
#include "RECONR.hpp"

namespace RP = njoy::ENDFtk::resonanceParameters;

SCENARIO( "Filling the sparse energy grid" ){
  using namespace njoy::RECONR;

  GIVEN( "an 'energy grid'" ){
    std::vector< double > grid = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }};

    WHEN( "filling upper grid is within 1.19... of lower grid" ){
      ReferenceGrid::fill ( 1.0, 1.1, grid );
      THEN( "nothing changes" ){
        std::vector< double > refGrid = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }};
        REQUIRE( refGrid == grid );
      }
    }

    WHEN( "filling is needed" ){
      THEN( "the appropriate values are appended" ){
        ReferenceGrid::fill( 4.0, 5.0, grid );
        std::vector< double > refGrid = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                           4.775106566857746, 5.0
        }};
        for(size_t i=0; i<refGrid.size(); i++){
          REQUIRE( refGrid[i] == Approx( grid[i] ) );
        }
      }
      THEN( "the appropriate values are appended" ){
        ReferenceGrid::fill( 4, 15, grid );
        std::vector< double > refGrid = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                           4.7751065669, 5.7004106812,
                                           6.8050171194, 8.1236704836,
                                           9.6978480683, 11.5770644989,
                                           13.8204291784, 15.0
        }};

        for(size_t i=0; i<refGrid.size(); i++){
          REQUIRE( refGrid[i] == Approx( grid[i] ) );
        }
      }
    }

    WHEN( "upperBound < lowerBound" ){
      ReferenceGrid::fill ( 1.0, 0.9, grid );
      THEN( "nothing is changed" ){
        std::vector< double > refGrid = {{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }};
        REQUIRE( refGrid == grid );
      }
    }
  }
} // SCENARIO
