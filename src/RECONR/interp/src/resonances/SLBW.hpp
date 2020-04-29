template< typename... Args >
struct SLBW_t : 
  resonanceReconstruction::breitWigner::singleLevel::Type< Args... > {
};

template< typename... Args >
using SLBW = interpolation::Table< SLBW_t< Args... > >;
