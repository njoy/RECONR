using namespace resonances {
struct SLBW_t : resonanceReconstruction::breitWigner::singleLevel::Apply {

  template< typename Range >
  void verifyXGridAssumptions( Range&& ){ }
  template< typename Range >
  void verifyYGridAssumptions( Range&& ){ }
};

struct MLBW_t : resonanceReconstruction::breitWigner::multiLevel::Apply {

  template< typename Range >
  void verifyXGridAssumptions( Range&& ){ }
  template< typename Range >
  void verifyYGridAssumptions( Range&& ){ }
};

struct RM_t : resonanceReconstruction::reichMoore::Apply {

  template< typename Range >
  void verifyXGridAssumptions( Range&& ){ }
  template< typename Range >
  void verifyYGridAssumptions( Range&& ){ }
};

using SLBW = Table< SLBW_t >;
using MLBW = Table< MLBW_t >;
using RM = Table< RM_t >;

using Variant = std::variant<
  SLBW,
  MLBW,
  RM
>;

using Vector = interpolation::Table< 
  interpolation::table::Vector< Variant > >;

} // namespace resonances
