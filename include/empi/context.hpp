/*
* Copyright (c) 2022-2023 University of Salerno, Italy. All rights reserved.
*/

#ifndef EMPI_PROJECT_CONTEXT_HPP
#define EMPI_PROJECT_CONTEXT_HPP

#include <cstddef>
#include <mpi.h>
#include <memory>

#include <empi/message_grp_hdl.hpp>
#include <empi/tag.hpp>
#include <empi/type_traits.hpp>
#include <functional>
#include "message_group.hpp"

namespace empi{

    class Context{

    public:
        Context(int* argc, char*** argv){
            MPI_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &thread_support);  
        }

        Context(const Context& c) = delete;
        Context(Context&& c) = default;

        ~Context(){
            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Finalize();
        }

		std::unique_ptr<MessageGroup> create_message_group(MPI_Comm comm, size_t pool_size = request_pool::default_pool_size) {
		return std::make_unique<MessageGroup>(comm);
	  }

	 private:
         int _rank;
         int thread_support;
	};




};

#endif // __EMPI_CONTEXT_H__
