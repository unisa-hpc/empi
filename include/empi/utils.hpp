//
// Created by luigi on 21/09/22.
//

#ifndef EMPI_PROJECT_UTILS_HPP
#define EMPI_PROJECT_UTILS_HPP

namespace empi{
	namespace details{

		template<typename T>
		requires has_data<T>
		static inline auto get_underlying_pointer(T&& buf){
		  return buf.data();
		}

		template<typename T>
		static inline auto get_underlying_pointer(T* buf){
		  return buf;
		}

		template<typename T>
		static inline auto get_underlying_pointer(const T* buf){
		  return buf;
		}


	}
}

#endif //EMPI_PROJECT_UTILS_HPP
