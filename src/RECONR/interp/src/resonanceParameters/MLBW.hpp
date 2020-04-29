template< typename... Args >
struct MLBW_t : 
  resonanceReconstruction::breitWigner::multiLevel::Type< Args... > {
};

template< typename... Args >
using MLBW = interpolation::Table< MLBW_t< Args... > >;
