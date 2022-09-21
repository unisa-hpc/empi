#ifndef __TYPE_TRAITS_H__
#define __TYPE_TRAITS_H__

#include <type_traits>
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

} // namespace empi


#endif // __TYPE_TRAITS_H__