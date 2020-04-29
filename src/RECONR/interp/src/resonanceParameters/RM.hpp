template< typename... Args >
struct RM_t : resonanceReconstruction::reichMoore::Type< Args... > {
};

template< typename... Args >
using RM = interpolation::Table< RM_t< Args... > >;
