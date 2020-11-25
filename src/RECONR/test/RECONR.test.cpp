#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "Log.hpp"

#include "RECONR/src/sigfig.hpp"

SCENARIO( "Testing sigfig" ){

  CHECK( 9E-7 == Approx( sigfig( 0.0, 7, -9 ) ).epsilon( 1E-7 ) );
  CHECK( 8E-7 == Approx( sigfig( 0.0, 7, -8 ) ).epsilon( 1E-7 ) );
  CHECK( 7E-7 == Approx( sigfig( 0.0, 7, -7 ) ).epsilon( 1E-7 ) );
  CHECK( 6E-7 == Approx( sigfig( 0.0, 7, -6 ) ).epsilon( 1E-7 ) );
  CHECK( 5E-7 == Approx( sigfig( 0.0, 7, -5 ) ).epsilon( 1E-7 ) );
  CHECK( 4E-7 == Approx( sigfig( 0.0, 7, -4 ) ).epsilon( 1E-7 ) );
  CHECK( 3E-7 == Approx( sigfig( 0.0, 7, -3 ) ).epsilon( 1E-7 ) );
  CHECK( 2E-7 == Approx( sigfig( 0.0, 7, -2 ) ).epsilon( 1E-7 ) );
  CHECK( 1E-7 == Approx( sigfig( 0.0, 7, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 0.0  == Approx( sigfig( 0.0, 7,  0 ) ).epsilon( 1E-7 ) );
  CHECK( 1E-7 == Approx( sigfig( 0.0, 7,  1 ) ).epsilon( 1E-7 ) );
  CHECK( 2E-7 == Approx( sigfig( 0.0, 7,  2 ) ).epsilon( 1E-7 ) );
  CHECK( 3E-7 == Approx( sigfig( 0.0, 7,  3 ) ).epsilon( 1E-7 ) );
  CHECK( 4E-7 == Approx( sigfig( 0.0, 7,  4 ) ).epsilon( 1E-7 ) );
  CHECK( 5E-7 == Approx( sigfig( 0.0, 7,  5 ) ).epsilon( 1E-7 ) );
  CHECK( 6E-7 == Approx( sigfig( 0.0, 7,  6 ) ).epsilon( 1E-7 ) );
  CHECK( 7E-7 == Approx( sigfig( 0.0, 7,  7 ) ).epsilon( 1E-7 ) );
  CHECK( 8E-7 == Approx( sigfig( 0.0, 7,  8 ) ).epsilon( 1E-7 ) );
  CHECK( 9E-7 == Approx( sigfig( 0.0, 7,  9 ) ).epsilon( 1E-7 ) );

  CHECK( 1234558 == Approx( sigfig( 1234567, 7, -9 ) ).epsilon( 1E-7 ) );
  CHECK( 1234559 == Approx( sigfig( 1234567, 7, -8 ) ).epsilon( 1E-7 ) );
  CHECK( 1234560 == Approx( sigfig( 1234567, 7, -7 ) ).epsilon( 1E-7 ) );
  CHECK( 1234561 == Approx( sigfig( 1234567, 7, -6 ) ).epsilon( 1E-7 ) );
  CHECK( 1234562 == Approx( sigfig( 1234567, 7, -5 ) ).epsilon( 1E-7 ) );
  CHECK( 1234563 == Approx( sigfig( 1234567, 7, -4 ) ).epsilon( 1E-7 ) );
  CHECK( 1234564 == Approx( sigfig( 1234567, 7, -3 ) ).epsilon( 1E-7 ) );
  CHECK( 1234565 == Approx( sigfig( 1234567, 7, -2 ) ).epsilon( 1E-7 ) );
  CHECK( 1234566 == Approx( sigfig( 1234567, 7, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 1234567 == Approx( sigfig( 1234567, 7,  0 ) ).epsilon( 1E-7 ) );
  CHECK( 1234568 == Approx( sigfig( 1234567, 7,  1 ) ).epsilon( 1E-7 ) );
  CHECK( 1234568 == Approx( sigfig( 1234567, 7     ) ).epsilon( 1E-7 ) );
  CHECK( 1234569 == Approx( sigfig( 1234567, 7,  2 ) ).epsilon( 1E-7 ) );
  CHECK( 1234570 == Approx( sigfig( 1234567, 7,  3 ) ).epsilon( 1E-7 ) );
  CHECK( 1234571 == Approx( sigfig( 1234567, 7,  4 ) ).epsilon( 1E-7 ) );
  CHECK( 1234572 == Approx( sigfig( 1234567, 7,  5 ) ).epsilon( 1E-7 ) );
  CHECK( 1234573 == Approx( sigfig( 1234567, 7,  6 ) ).epsilon( 1E-7 ) );
  CHECK( 1234574 == Approx( sigfig( 1234567, 7,  7 ) ).epsilon( 1E-7 ) );
  CHECK( 1234575 == Approx( sigfig( 1234567, 7,  8 ) ).epsilon( 1E-7 ) );
  CHECK( 1234576 == Approx( sigfig( 1234567, 7,  9 ) ).epsilon( 1E-7 ) );

  CHECK( -1234568 == Approx( sigfig( -1234567, 7, -1 ) ).epsilon( 1E-7 ) );
  CHECK( -1234567 == Approx( sigfig( -1234567, 7,  0 ) ).epsilon( 1E-7 ) );
  CHECK( -1234566 == Approx( sigfig( -1234567, 7,  1 ) ).epsilon( 1E-7 ) );
  CHECK( -1234566 == Approx( sigfig( -1234567, 7     ) ).epsilon( 1E-7 ) );

  CHECK( 1233     == Approx( sigfig( 1234, 4, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 1233.9   == Approx( sigfig( 1234, 5, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 1233.99  == Approx( sigfig( 1234, 6, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 1233.999 == Approx( sigfig( 1234, 7, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 1234.001 == Approx( sigfig( 1234, 7,  1 ) ).epsilon( 1E-7 ) );
  CHECK( 1234.01  == Approx( sigfig( 1234, 6,  1 ) ).epsilon( 1E-7) );
  CHECK( 1234.1   == Approx( sigfig( 1234, 5,  1 ) ).epsilon( 1E-7) );
  CHECK( 1235     == Approx( sigfig( 1234, 4,  1 ) ).epsilon( 1E-7) );

  CHECK( 0.1233    == Approx( sigfig( 0.1234, 4, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 0.12339   == Approx( sigfig( 0.1234, 5, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 0.123399  == Approx( sigfig( 0.1234, 6, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 0.1233999 == Approx( sigfig( 0.1234, 7, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 0.1234001 == Approx( sigfig( 0.1234, 7,  1 ) ).epsilon( 1E-7 ) );
  CHECK( 0.123401  == Approx( sigfig( 0.1234, 6,  1 ) ).epsilon( 1E-7 ) );
  CHECK( 0.12341   == Approx( sigfig( 0.1234, 5,  1 ) ).epsilon( 1E-7 ) );
  CHECK( 0.1235    == Approx( sigfig( 0.1234, 4,  1 ) ).epsilon( 1E-7 ) );
  
  CHECK( 9.999998E-8 == Approx( sigfig( 1E-7, 7, -2 ) ).epsilon( 1E-7 ) );
  CHECK( 9.999999E-8 == Approx( sigfig( 1E-7, 7, -1 ) ).epsilon( 1E-7 ) );
  CHECK( 1.000001E-7 == Approx( sigfig( 1E-7, 7,  1 ) ).epsilon( 1E-7 ) );
  CHECK( 1.000002E-7 == Approx( sigfig( 1E-7, 7,  2 ) ).epsilon( 1E-7 ) );

  CHECK( 1E-7 == Approx( sigfig( 1E-7, 7, 0 ) ).epsilon( 1E-7 ) );
  CHECK( 1E-7 == Approx( sigfig( 1E-7, 6, 0 ) ).epsilon( 1E-7 ) );
  CHECK( 1E-7 == Approx( sigfig( 1E-7, 5, 0 ) ).epsilon( 1E-7 ) );
  CHECK( 1E-7 == Approx( sigfig( 1E-7, 4, 0 ) ).epsilon( 1E-7 ) );
  CHECK( 1E-7 == Approx( sigfig( 1E-7, 3, 0 ) ).epsilon( 1E-7 ) );
  CHECK( 1E-7 == Approx( sigfig( 1E-7, 2, 0 ) ).epsilon( 1E-7 ) );
  CHECK( 1E-7 == Approx( sigfig( 1E-7, 1, 0 ) ).epsilon( 1E-7 ) );
  CHECK( 1E-7 == Approx( sigfig( 1E-7, 0, 0 ) ).epsilon( 1E-7 ) );

} // SCENARIO

// SCENARIO( "Testing round" ){
//   CHECK(-1.000000000000000 == Approx( sigfig(-1.234567891123456, 1, 0) ).epsilon( 1E-16) );
//   CHECK(-1.200000000000000 == Approx( sigfig(-1.234567891123456, 2, 0) ).epsilon( 1E-16) );
//   CHECK(-1.230000000000000 == Approx( sigfig(-1.234567891123456, 3, 0) ).epsilon( 1E-16) );
//   CHECK(-1.235000000000000 == Approx( sigfig(-1.234567891123456, 4, 0) ).epsilon( 1E-16) );
//   CHECK(-1.234600000000000 == Approx( sigfig(-1.234567891123456, 5, 0) ).epsilon( 1E-16) );
//   CHECK(-1.234570000000000 == Approx( sigfig(-1.234567891123456, 6, 0) ).epsilon( 1E-16) );
//   CHECK(-1.234568000000000 == Approx( sigfig(-1.234567891123456, 7, 0) ).epsilon( 1E-16) );
//   CHECK(-1.234567900000000 == Approx( sigfig(-1.234567891123456, 8, 0) ).epsilon( 1E-16) );
//   CHECK(-1.234567890000000 == Approx( sigfig(-1.234567891123456, 9, 0) ).epsilon( 1E-16) );
//   CHECK( 1.000000000000000 == Approx( sigfig( 1.234567891123456, 1, 0) ).epsilon( 1E-16) );
//   CHECK( 1.200000000000000 == Approx( sigfig( 1.234567891123456, 2, 0) ).epsilon( 1E-16) );
//   CHECK( 1.230000000000000 == Approx( sigfig( 1.234567891123456, 3, 0) ).epsilon( 1E-16) );
//   CHECK( 1.235000000000000 == Approx( sigfig( 1.234567891123456, 4, 0) ).epsilon( 1E-16) );
//   CHECK( 1.234600000000000 == Approx( sigfig( 1.234567891123456, 5, 0) ).epsilon( 1E-16) );
//   CHECK( 1.234570000000000 == Approx( sigfig( 1.234567891123456, 6, 0) ).epsilon( 1E-16) );
//   CHECK( 1.234568000000000 == Approx( sigfig( 1.234567891123456, 7, 0) ).epsilon( 1E-16) );
//   CHECK( 1.234567900000000 == Approx( sigfig( 1.234567891123456, 8, 0) ).epsilon( 1E-16) );
//   CHECK( 1.234567890000000 == Approx( sigfig( 1.234567891123456, 9, 0) ).epsilon( 1E-16) );
  
// } // SCENARIO
