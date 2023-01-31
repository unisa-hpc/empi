//
// Created by luigi on 21/09/22.
//

#ifndef EMPI_PROJECT_UTILS_HPP
#define EMPI_PROJECT_UTILS_HPP

#include <empi/type_traits.hpp>

namespace empi::details{

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
		
		template<typename T>
		inline constexpr auto abs(T& a, T&b) -> decltype(std::declval<T>() - std::declval<T>()) {
			return std::abs(static_cast<long long>(a) - static_cast<long long>(b));
		}

	}

#endif //EMPI_PROJECT_UTILS_HPP
