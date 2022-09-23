#ifndef __TYPE_TRAITS_H__
#define __TYPE_TRAITS_H__

#include <type_traits>
#include <tuple>
#include <mpi.h>

namespace empi
{

template<typename T>
struct MpiType
{};

#define MAKE_TYPE_CONVERSION(T, mpi_type) \
	template<> \
	struct MpiType<T> \
	{ static constexpr int value = mpi_type; };

MAKE_TYPE_CONVERSION(int,	MPI_INT)
MAKE_TYPE_CONVERSION(char,	MPI_CHAR)
MAKE_TYPE_CONVERSION(short,	MPI_SHORT)
MAKE_TYPE_CONVERSION(long,	MPI_LONG)
MAKE_TYPE_CONVERSION(float,	MPI_FLOAT)
MAKE_TYPE_CONVERSION(double,MPI_DOUBLE)

template<typename T, T v1, T v2>
struct is_greater{
  static constexpr bool value = v1 > v2;
};

template<typename T, T v1, T v2>
struct is_same{
  static constexpr bool value = v1 == v2;
};

template<typename T, T v1, T v2>
constexpr bool is_greater_v = is_greater<T,v1,v2>::value;
template<typename T, T v1, T v2>
constexpr bool is_same_v = is_same<T,v1,v2>::value;


// https://stackoverflow.com/a/7943765
template <typename T>
struct function_traits
	: public function_traits<decltype(&T::operator())>
{};
// For generic types, directly use the result of the signature of its 'operator()'

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
// we specialize for pointers to member function
{
  enum { arity = sizeof...(Args) };
  // arity is the number of arguments.

  typedef ReturnType result_type;

  template <size_t i>
  struct arg
  {
	typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
	// the i-th argument is equivalent to the i-th tuple element of a tuple
	// composed of those arguments.
  };
};

template <class, template <class, class...> class>
struct is_instance : public std::false_type {};

template <class...Ts, template <class, class...> class U>
struct is_instance<U<Ts...>, U> : public std::true_type {};

template<typename T>
struct val{
  T x;
};

template<typename T, template <class, class...> class K>
concept has_parameter = requires(T f){
	is_instance<std::remove_reference_t<typename function_traits<decltype(f)>::template arg<0>::type>,K>{};
};



} // namespace empi


#endif // __TYPE_TRAITS_H__