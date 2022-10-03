//
// Created by luigi on 28/09/22.
//

#ifndef EMPI_PROJECT_INCLUDE_EMPI_REQUEST_HPP_
#define EMPI_PROJECT_INCLUDE_EMPI_REQUEST_HPP_

#include <empi/empi_defines.hpp>

namespace empi {

	struct Request {

	  explicit Request() {
		request = std::make_shared<MPI_Request>();
	  }

	  std::shared_ptr<MPI_Request> get_request(){
		return request;
	  }

	  MPI_Request* get_pointer(){
		return request.get();
	  }

	 private:
	  std::shared_ptr<MPI_Request> request;
	};

}

#endif //EMPI_PROJECT_INCLUDE_EMPI_REQUEST_HPP_
