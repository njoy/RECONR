#ifndef RECONR_DETAILS_SIMPLEENDFTESTSTRING_HPP
#define RECONR_DETAILS_SIMPLEENDFTESTSTRING_HPP
std::string MF1(){
  return R"(1.001000+3 9.991673-1          0          0          0          5 125 1451     
 0.000000+0 0.000000+0          0          0          0          6 125 1451     
 1.000000+0 2.000000+7          0          0         10          8 125 1451     
 0.000000+0 0.000000+0          0          0          0          0 125 1451     
  1-H -  1 LANL       EVAL-JUL16 G.M.Hale                          125 1451     
 NDS 148, 1 (2018)    DIST-FEB18                       20170124    125 1451     
----ENDF/B-VIII.0     MATERIAL  125                                125 1451     
-----INCIDENT NEUTRON DATA                                         125 1451     
------ENDF-6 FORMAT                                                125 1451     
 0.000000+0 0.000000+0          0          0          0          0 125 1  0     
 0.000000+0 0.000000+0          0          0          0          0 125 0  0     )";
}

std::string MF3(){
  return R"(
 1.001000+3 9.991673-1          0          0          0          0 125 3  1     
 2.224648+6 3.224648+6          0          0          2          6 125 3  1     
          3          1          6          5                       125 3  1     
 1.000000-5 1.000000+0 2.000000-5 1.182897+1 7.500000+5 3.347392-5 125 3  1     
 1.900000+7 2.751761-5 1.950000+7 2.731301-5 2.000000+7 2.710792-5 125 3  1     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.001000+3 9.991673-1          0          0          0          0 125 3 16     
 2.224648+6 3.224648+6          0          0          1          6 125 3 16     
          6          1                                             125 3 16     
 1.000000-5 1.600000+1 2.000000-5 1.182897+1 7.500000+5 3.347392-5 125 3 16     
 1.900000+7 2.751761-5 1.950000+7 2.731301-5 2.000000+7 2.710792-5 125 3 16     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.001000+3 9.991673-1          0          0          0          0 125 3 18     
 2.224648+6 3.224648+6          0          0          2          6 125 3 18     
          3          1          6          2                       125 3 18     
 1.000000+5 1.800000+1 1.500000+5 1.182897+1 7.500000+5 3.347392-5 125 3 18     
 1.900000+7 2.751761-5 1.950000+7 2.731301-5 2.000000+7 2.710792-5 125 3 18     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.001000+3 9.991673-1          0          0          0          0 125 3102     
 2.224648+6 3.224648+6          0          0          2          6 125 3102     
          3          3          6          4                       125 3102     
 1.000000-5 1.020000+2 2.000000-5 1.182897+1 7.500000+5 3.347392-5 125 3102     
 1.900000+7 2.751761-5 1.950000+7 2.731301-5 2.000000+7 2.710792-5 125 3102     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 0.000000+0 0.000000+0          0          0          0          0 125 0  0     )";
}

std::string MEND(){
  return R"(
 0.000000+0 0.000000+0          0          0          0          0   0 0  0     )";
}

std::string SLBW(){
  return "";
}

std::string MLBW(){
  return "";
}

std::string RM(){
  return "";
}

// njoy::ENDFtk::syntaxTree::Material< std::string::iterator > 
inline njoy::RECONR::R2D2::ENDFMaterial_t
ENDFMaterial( std::string formalism = "SLBW" ){
  std::string ENDF = MF1();

  if( formalism == "SLBW" ) ENDF += SLBW();
  if( formalism == "MLBW" ) ENDF += MLBW();
  if( formalism == "RM" )   ENDF += RM();

  const std::string endf = ENDF + MF3() + MEND();

  auto begin = endf.begin();
  auto start = begin;
  auto end = endf.end();
  long lineNumber = 0;
  njoy::ENDFtk::HeadRecord head( begin, end, lineNumber );

  // return njoy::ENDFtk::syntaxTree::Material< std::string::iterator >( 
  return njoy::RECONR::R2D2::ENDFMaterial_t(
    head, start, begin, end, lineNumber );
}

#endif // RECONR_DETAILS_SIMPLEENDFTESTSTRING_HPP