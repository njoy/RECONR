SCENARIO( "Filling the sparse energy grid" ){
  using namespace njoy::RECONR;

  GIVEN( "an 'energy grid'" ){
    std::vector< double > grid{ 1.0 };

    WHEN( "filling upper grid is within 1.19... of lower grid" ){
      TestReferenceGrid::fill ( 1.0, 1.1, grid );
      THEN( "only upper bound is added" ){
        std::vector< double > refGrid{1.0, 1.1};
        REQUIRE( refGrid == grid );
      }
    }

    WHEN( "filling is needed" ){
      THEN( "the appropriate values are appended" ){
        TestReferenceGrid::fill( 1.0, 2.0, grid );
        std::vector< double > refGrid = {{ 
          1.0, 1.1937766417, 1.4251026703, 1.7012542799 }};
        details::checkRanges( refGrid, grid );
      }
      THEN( "the appropriate values are appended" ){
        TestReferenceGrid::fill( 4, 15, grid );
        std::vector< double > refGrid = {{ 
          1.0, 4.7751065669, 5.7004106812, 6.8050171194, 8.1236704836,
          9.6978480683, 11.5770644989, 13.8204291784 }};

        details::checkRanges( refGrid, grid );
      }
    }

    WHEN( "upperBound < lowerBound" ){
      TestReferenceGrid::fill ( 1.0, 0.9, grid );
      THEN( "nothing is changed" ){
        std::vector< double > refGrid{ 1, 0.9 };
        REQUIRE( refGrid == grid );
      }
    }
  }
} // SCENARIO

