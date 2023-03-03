/*
* Copyright (c) 2022-2023 University of Salerno, Italy. All rights reserved.
*/

#ifndef EMPI_PROJECT_UTILS_HPP
#define EMPI_PROJECT_UTILS_HPP

#include <empi/type_traits.hpp>
#include <empi/defines.hpp>

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

		template<mpi_function f> 
		void checktag(int tag, int maxtag){
			if constexpr (details::is_all<f>){
				if(tag > maxtag)
					throw std::runtime_error("Incorrect tag entered in send function");
			}
			else if constexpr (details::is_send<f>){
				if(tag > maxtag || tag == -1)
					throw std::runtime_error("Incorrect tag entered in send function");
			}
			else if constexpr (details::is_recv<f>){
				if(tag > maxtag || tag < -1)
					throw std::runtime_error("Incorrect tag entered in recv function");
			}
		}

	}

#endif //EMPI_PROJECT_UTILS_HPP
