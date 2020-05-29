#ifndef RECONR_DETAILS_SIMPLEENDFTESTSTRING_HPP
#define RECONR_DETAILS_SIMPLEENDFTESTSTRING_HPP

namespace details{
inline
std::string MF1(){
  return R"( 1.001000+3 9.991673-1          0          0          0          5 125 1451     
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

inline 
std::string FEND( ){
  return R"(
 0.000000+0 0.000000+0          0          0          0          0 125 0  0     )";
}

inline
std::string MEND(){
  return R"(
 0.000000+0 0.000000+0          0          0          0          0   0 0  0     )";
}

inline
std::string SLBW(){
  return
    //        EL         EH        LRU        LRF        NRO       NAPS
    " 1.000000-5 7.500000+0          1          1          0          0 125 2151     \n" // CONT (range)
    " 3.000000+0 0.000000+0          0          0          1          0 125 2151     \n" // CONT
    " 1.664920+2 0.000000+0          0          0         24          4 125 2151     \n" // LIST
    "-2.974700+0 2.500000+0 7.846160-2 4.616000-4 7.800000-2 0.000000+0 125 2151     \n"
    "-9.747000-1 2.500000+0 7.846160-2 4.616000-4 7.800000-2 0.000000+0 125 2151     \n"
    " 1.025300+0 2.500000+0 7.846160-2 4.616000-4 7.800000-2 0.000000+0 125 2151     \n"
    " 3.025300+0 2.500000+0 7.846160-2 4.616000-4 7.800000-2 0.000000+0 125 2151     \n"
    " 0.000000+0 0.000000+0          0          0          0          0 125 2  0     ";
}

inline
std::string MLBW(){
  return "";
}

inline
std::string RM(){
  return
    //        EL         EH        LRU        LRF        NRO       NAPS
    " 9.859600-1 5.500000+0          1          3          0          0 125 2151     \n" // CONT (range)
    " 3.500000+0 9.859600-1          1          0          1          3 125 2151     \n"
    " 2.330200+2 9.859600-1          0          0        102         17 125 2151     \n"
    "-1.000000+2 3.000000+0 1.145800-2 3.871290-2 1.229980-4 7.233640-2 125 2151     \n"
    "-9.000000+1 4.000000+0 2.422100-6 3.680760-2 5.617020-2-2.168940-1 125 2151     \n"
    "-4.297600+0 4.000000+0 7.164100-3 3.481860-2 3.192990-1-1.153500-1 125 2151     \n"
    "-3.493400+0 3.000000+0 8.471500-8 3.780160-2-6.760010-3 1.298560-2 125 2151     \n"
    "-1.504300+0 3.000000+0 8.519600-8 3.767610-2-7.010690-3 1.232090-2 125 2151     \n"
    "-4.116100-1 3.000000+0 1.487500-4 2.984470-2-1.027260-3-1.554150-1 125 2151     \n"
    "-1.942800-1 4.000000+0 5.044600-7 3.504170-2 1.989540-1-1.694210-3 125 2151     \n"
    " 3.657500-5 4.000000+0 6.50520-11 2.984470-2-5.263430-4 9.645560-4 125 2151     \n"
    " 2.819000-1 3.000000+0 4.439200-6 3.837130-2 1.065400-1-4.849860-3 125 2151     \n"
    " 1.138900+0 4.000000+0 1.384200-5 4.069500-2-4.640000-6 1.093200-1 125 2151     \n"
    " 2.036100+0 3.000000+0 9.358700-6 3.933000-2-7.736600-3-1.573500-3 125 2151     \n"
    " 2.776700+0 4.000000+0 1.277200-6 3.887600-2 6.049200-2-4.250300-2 125 2151     \n"
    " 3.156600+0 3.000000+0 2.432600-5 3.989600-2-7.995100-2 1.716100-2 125 2151     \n"
    " 3.620800+0 4.000000+0 4.184000-5 3.764400-2-2.679600-2 2.849100-2 125 2151     \n"
    " 4.850800+0 4.000000+0 7.560500-5 3.801700-2 4.666800-5-3.735100-3 125 2151     \n"
    " 5.449700+0 4.000000+0 3.793200-5 3.920100-2-7.874000-2-3.612500-1 125 2151     \n"
    " 6.209400+0 3.000000+0 1.654700-4 4.005100-2-1.079400-1 7.385900-2 125 2151     \n"
    " 0.000000+0 0.000000+0          0          0          0          0 125 2  0     ";
}

inline
std::string RML(){
  return
    //        EL         EH        LRU        LRF        NRO       NAPS
    "-1.900000+6 5.500000+5          1          7          0          0 125 2151     \n" // CONT (range)
    " 0.000000+0 0.000000+0          0          3          2          0 125 2151     \n"
    " 0.000000+0 0.000000+0          2          0         24          4 125 2151     \n"
    " 0.000000+0 5.446635+1 0.000000+0 2.600000+1 1.000000+0 0.000000+0 125 2151     \n"
    " 0.000000+0 0.000000+0 0.000000+0 1.020000+2 0.000000+0 0.000000+0 125 2151     \n"
    " 1.000000+0 5.347624+1 0.000000+0 2.600000+1 5.000000-1 0.000000+0 125 2151     \n"
    " 0.000000+0 1.000000+0 0.000000+0 2.000000+0 0.000000+0 1.000000+0 125 2151     \n"
    " 5.000000-1 0.000000+0          0          0         12          2 125 2151     \n"
    " 1.000000+0 0.000000+0 0.000000+0 0.000000+0 0.000000+0 0.000000+0 125 2151     \n"
    " 2.000000+0 0.000000+0 5.000000-1 0.000000+0 5.437300-1 5.437300-1 125 2151     \n"
    " 0.000000+0 0.000000+0          0          2         12          2 125 2151     \n"
    "-1.223300+6 1.000000+0 9.611086+5 0.000000+0 0.000000+0 0.000000+0 125 2151     \n"
    " 7.788000+3 1.455000+0 1.187354+3 0.000000+0 0.000000+0 0.000000+0 125 2151     \n"
    "-5.000000-1 0.000000+0          0          0         12          2 125 2151     \n"
    " 1.000000+0 0.000000+0 0.000000+0 0.000000+0 0.000000+0 0.000000+0 125 2151     \n"
    " 2.000000+0 1.000000+0 5.000000-1 0.000000+0 5.437300-1 5.437300-1 125 2151     \n"
    " 0.000000+0 0.000000+0          0          2         12          2 125 2151     \n"
    " 5.152000+4 3.600000-1 1.600200+1 0.000000+0 0.000000+0 0.000000+0 125 2151     \n"
    " 5.359000+4 1.500000+0 1.700000+1 0.000000+0 0.000000+0 0.000000+0 125 2151     \n"
    " 0.000000+0 0.000000+0          0          0          0          0 125 2  0     ";
}

inline 
std::string MF2( std::string formalism = "SLBW" ){
  std::string endf = "\n"
    //        ZA        AWR                              NIS
    " 4.510500+4 1.040000+2          0          0          1          0 125 2151     \n" // HEAD
    //       ZAI        ABN                   LFW        NER
    " 4.510500+4 1.000000+0          0          0          1          0 125 2151     \n";// CONT (isotope)

  if( formalism == "SLBW" ) endf += SLBW();
  if( formalism == "MLBW" ) endf += MLBW();
  if( formalism == "RM" )   endf += RM();
  if( formalism == "RML" )  endf += RML();

  return endf + FEND();
}

inline
std::string MF3(){
  return R"(
 1.001000+3 9.991673-1          0          0          0          0 125 3  1     
 2.224648+6 3.224648+6          0          0          1          6 125 3  1     
          6          1                                             125 3  1     
 1.000000-5 1.000000+0 2.000000-5 1.182897+1 7.500000+5 3.347392-5 125 3  1     
 1.900000+7 2.751761-5 1.950000+7 2.731301-5 2.000000+7 2.710792-5 125 3  1     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.002000+3 9.991673-1          0          0          0          0 125 3  2     
 2.224648+2 3.224648+2          0          0          1          6 125 3  2     
          6          1                                             125 3  2     
 1.000000-5 2.000000+0 2.000000-5 2.000000+0 7.500000+5 2.000000+0 125 3  2     
 1.900000+7 2.000000+0 1.950000+7 2.000000+0 2.000000+7 2.000000+0 125 3  2     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.016000+3 9.991673-1          0          0          0          0 125 3 16     
 2.22464+16 3.22464+16          0          0          1          6 125 3 16     
          6          5                                             125 3 16     
 1.000000-5 1.600000+1 2.000000-5 1.182897+1 7.500000+5 3.347392-5 125 3 16     
 1.900000+7 2.751761-5 1.950000+7 2.731301-5 2.000000+7 2.710792-5 125 3 16     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.018000+3 9.991673-1          0          0          0          0 125 3 18     
 2.22464+18 3.22464+18          0          0          2          6 125 3 18     
          3          1          6          2                       125 3 18     
 1.000000+5 1.800000+1 1.500000+5 1.182897+1 7.500000+5 3.347392-5 125 3 18     
 1.900000+7 2.751761-5 1.950000+7 2.731301-5 2.000000+7 2.710792-5 125 3 18     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.001000+3 9.991673-1          0          0          0          0 125 3 51     
 2.224648+6 3.224648+6          0          0          2          6 125 3 51     
          3          1          6          2                       125 3 51     
 1.000000+5 5.100000+1 1.500000+5 5.100000+1 7.500000+5 5.100000+1 125 3 51     
 1.900000+7 5.100000+1 1.950000+7 5.100000+1 2.000000+7 5.100000+1 125 3 51     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.001000+3 9.991673-1          0          0          0          0 125 3 52     
 2.224648+6 3.224648+6          0          0          2          6 125 3 52     
          3          1          6          2                       125 3 52     
 1.000000+5 5.200000+1 1.500000+5 5.200000+1 7.500000+5 5.200000+1 125 3 52     
 1.900000+7 5.200000+1 1.950000+7 5.200000+1 2.000000+7 5.200000+1 125 3 52     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.102000+3 9.991673-1          0          0          0          0 125 3102     
 2.224648+6 3.224648+6          0          0          2          6 125 3102     
          3          3          6          4                       125 3102     
 1.000000-5 1.020000+2 2.000000-5 1.182897+1 7.500000+5 3.347392-5 125 3102     
 1.900000+7 2.751761-5 1.950000+7 2.731301-5 2.000000+7 2.710792-5 125 3102     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.001000+3 9.991673-1          0          0          0          0 125 3875     
 2.224648+6 3.224648+6          0          0          2          6 125 3875     
          3          1          6          2                       125 3875     
 1.000000+5 0.500000+0 1.500000+5 0.500000+0 7.500000+5 0.500000+0 125 3875     
 1.900000+7 0.500000+0 1.950000+7 0.500000+0 2.000000+7 0.500000+0 125 3875     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.001000+3 9.991673-1          0          0          0          0 125 3876     
 2.224648+6 3.224648+6          0          0          2          6 125 3876     
          3          1          6          2                       125 3876     
 1.000000+5 0.500000+0 1.500000+5 0.500000+0 7.500000+5 0.500000+0 125 3876     
 1.900000+7 0.500000+0 1.950000+7 0.500000+0 2.000000+7 0.500000+0 125 3876     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 1.001000+3 9.991673-1          0          0          0          0 125 3877     
 2.224648+6 3.224648+6          0          0          2          6 125 3877     
          3          1          6          2                       125 3877     
 1.000000+5 0.500000+0 1.500000+5 0.500000+0 7.500000+5 0.500000+0 125 3877     
 1.900000+7 0.500000+0 1.950000+7 0.500000+0 2.000000+7 0.500000+0 125 3877     
 0.000000+0 0.000000+0          0          0          0          0 125 3  0     
 0.000000+0 0.000000+0          0          0          0          0 125 0  0     )";
}

inline 
njoy::ENDFtk::syntaxTree::Material< std::string > 
// njoy::RECONR::R2D2::ENDFMaterial_t
ENDFMaterial( std::string formalism = "SLBW", bool print = false ){

  std::string endf = MF1() + MF2( formalism ) + MF3() + MEND();

  if( print ){
    njoy::Log::info( "material: \n{}", endf );
  }

  auto begin = endf.begin();
  auto start = begin;
  auto end = endf.end();
  long lineNumber = 0;
  njoy::ENDFtk::HeadRecord head( begin, end, lineNumber );

  return njoy::ENDFtk::syntaxTree::Material< std::string >( 
  // return njoy::RECONR::R2D2::ENDFMaterial_t(
    head, start, begin, end, lineNumber );
}
} // namespace details

#endif // RECONR_DETAILS_SIMPLEENDFTESTSTRING_HPP
