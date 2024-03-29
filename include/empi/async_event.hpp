/*
 * Copyright (c) 2022-2023 University of Salerno, Italy. All rights reserved.
 */

#ifndef INCLUDE_EMPI_ASYNC_EVENT
#define INCLUDE_EMPI_ASYNC_EVENT

#include "empi/datatype.hpp"
#include <memory>
#include <mpi.h>

namespace empi {

struct async_event {
    constexpr async_event() : res(-1) { request = std::make_unique<MPI_Request>(MPI_REQUEST_NULL); }

    async_event &operator=(async_event &&e) noexcept {
        res = e.res;
        request = std::move(e.request);
        return *this;
    }

    [[nodiscard]] auto get_request() const -> MPI_Request * { return request.get(); };


    [[nodiscard]] std::unique_ptr<MPI_Status> wait() const {
        auto mpi_status = std::make_unique<MPI_Status>();
        MPI_Wait(request.get(), mpi_status.get());
        return mpi_status;
    }

    template<bool status>
        requires(status == empi::details::no_status)
    void wait() const {
        MPI_Wait(request.get(), MPI_STATUS_IGNORE);
    }

    int res;
    std::unique_ptr<MPI_Request> request;
};

using async_event_p = std::shared_ptr<empi::async_event>;

} // namespace empi

#endif /* INCLUDE_EMPI_ASYNC_EVENT */
