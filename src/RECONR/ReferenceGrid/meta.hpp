template< bool B >
using if_ = std::enable_if_t< B, bool >;

template< typename T >
using Resonance_t = decltype( std::declval<T>().resonances() );

template< typename T, typename = void >
struct HasResonances : public std::false_type {};

template< typename T >
struct HasResonances< T, utility::void_t< Resonance_t< T > > >
  : public std::true_type {};

template< typename T >
static constexpr bool hasResonances = HasResonances<T>::value;

template< typename T >
using Jvalue_t = decltype( std::declval<T>().jValues() );

template< typename T, typename = void >
struct HasJvalues : public std::false_type {};

template< typename T >
struct HasJvalues< T, utility::void_t< Jvalue_t< T > > >
  : public std::true_type {};

template< typename T >
static constexpr bool hasJvalues = HasJvalues<T>::value;

template< typename T >
using Lvalue_t = decltype( std::declval<T>().lValues() );

template< typename T, typename = void >
struct HasLvalues : public std::false_type {};

template< typename T >
struct HasLvalues< T, utility::void_t< Lvalue_t< T > > >
  : public std::true_type {};

template< typename T >
static constexpr bool hasLvalues = HasLvalues<T>::value;
