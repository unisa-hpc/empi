//
// Created by luigi on 05/10/22.
//

#ifndef EMPI_PROJECT_INCLUDE_EMPI_DATATYPE_HPP_
#define EMPI_PROJECT_INCLUDE_EMPI_DATATYPE_HPP_

#include <empi/type_traits.hpp>

namespace empi::details {

template<typename T>
struct mpi_type_impl {
  static MPI_Datatype get_type() noexcept { return nullptr; }
};

#define MAKE_TYPE_CONVERSION(T, base_type) \
    template<> \
    struct mpi_type_impl<T> \
    {                                      \
        static MPI_Datatype get_type() noexcept { return base_type; } \
    };

MAKE_TYPE_CONVERSION(int, MPI_INT)
MAKE_TYPE_CONVERSION(char, MPI_CHAR)
MAKE_TYPE_CONVERSION(short, MPI_SHORT)
MAKE_TYPE_CONVERSION(long, MPI_LONG)
MAKE_TYPE_CONVERSION(float, MPI_FLOAT)
MAKE_TYPE_CONVERSION(double, MPI_DOUBLE)

template<typename T>
struct mpi_type {
  static MPI_Datatype get_type() {
	if constexpr (empi::has_data<T>)
	  return mpi_type_impl<typename T::value_type>::get_type();
	else
	  return mpi_type_impl<T>::get_type();
  }
};

}

#endif //EMPI_PROJECT_INCLUDE_EMPI_DATATYPE_HPP_
