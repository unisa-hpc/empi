//
// Created by luigi on 27/09/22.
//

#ifndef EMPI_PROJECT_INCLUDE_EMPI_EMPI_DEFINES_HPP_
#define EMPI_PROJECT_INCLUDE_EMPI_EMPI_DEFINES_HPP_

#include <mpi.h>
#include <vector>
#include <memory>

#include <empi/async_event.hpp>

namespace empi{
	constexpr int NOSIZE = 0;
	using request_pool_t = std::vector<std::shared_ptr<async_event>>;
	using request_pool = std::shared_ptr<request_pool_t>;
}

#endif //EMPI_PROJECT_INCLUDE_EMPI_EMPI_DEFINES_HPP_
