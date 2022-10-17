//
// Created by luigi on 03/10/22.
//

#ifndef EMPI_PROJECT_INCLUDE_EMPI_ASYNC_EVENT_HPP_
#define EMPI_PROJECT_INCLUDE_EMPI_ASYNC_EVENT_HPP_

#include <memory>
#include <mpi.h>

struct async_event {

  constexpr async_event() : res(-1) {
    request = std::make_shared<MPI_Request>();
  }

  auto get_request() -> MPI_Request * { return request.get(); };

  std::unique_ptr<MPI_Status> wait() {
    auto status = std::make_unique<MPI_Status>();
    MPI_Wait(request.get(), status.get());
    return status;
  }

  int res;
  std::shared_ptr<MPI_Request> request;
};

#endif // EMPI_PROJECT_INCLUDE_EMPI_ASYNC_EVENT_HPP_
