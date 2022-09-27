//
// Created by luigi on 27/09/22.
//

#ifndef EMPI_PROJECT_INCLUDE_EMPI_EMPI_DEFINES_HPP_
#define EMPI_PROJECT_INCLUDE_EMPI_EMPI_DEFINES_HPP_

#include <mpi.h>
#include <vector>
#include <memory>

namespace empi{
	constexpr int NOSIZE = 0;
	using request_pool = std::shared_ptr<std::vector<MPI_Request*>>;
}

#endif //EMPI_PROJECT_INCLUDE_EMPI_EMPI_DEFINES_HPP_
