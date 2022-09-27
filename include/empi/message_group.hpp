//
// Created by luigi on 26/09/22.
//

#ifndef EMPI_PROJECT_INCLUDE_EMPI_MESSAGE_GROUP_HPP_
#define EMPI_PROJECT_INCLUDE_EMPI_MESSAGE_GROUP_HPP_

#include <mpi.h>
#include <queue>

#include <empi/message_grp_hdl.hpp>
#include <empi/tag.hpp>
#include <empi/utils.hpp>
#include <empi/type_traits.hpp>
#include <empi/empi_defines.hpp>

namespace empi {
  class MessageGroup {
   public:
	explicit MessageGroup(MPI_Comm comm) : comm(comm) {
	  requests = std::make_shared<std::vector<MPI_Request*>>();
	}

	//---------------- SEND ------------------

	template<Tag tag, size_t size, typename T, typename C = typename T::value_type>
	int send(T&& data, int dest){
	  MessageGroupHandler<C,tag,size> h(comm, requests);
	  return h.template send(data,dest);
	}

	template<Tag tag, size_t size, typename T>
	int send(const T* data, int dest){
	  MessageGroupHandler<T,tag,size> h(comm, requests);
	  return h.template send(data,dest);
	}

	template<size_t size, typename T, typename C = typename T::value_type>
	int send(T&& data, int dest, Tag tag){
	  MessageGroupHandler<C,NOTAG,size> h(comm, requests);
	  return h.template send(data,dest,tag);
	}

	template<size_t size, typename T>
	int send(const T* data, int dest, Tag tag){
	  MessageGroupHandler<T,NOTAG,size> h(comm, requests);
	  return h.template send(data,dest,tag);
	}

	template<Tag tag, typename T, typename C = typename T::value_type>
	int send(T&& data, int dest, size_t size){
	  MessageGroupHandler<C,tag,NOSIZE> h(comm, requests);
	  return h.template send(data,dest,size);
	}

	template<Tag tag, typename T>
	int send(const T* data, int dest, size_t size){
	  MessageGroupHandler<T,tag,NOSIZE> h(comm, requests);
	  return h.template send(data,dest,size);
	}

	// ------------------------- END SEND -------------------------------

	template<Tag tag, size_t size, typename T, typename C = typename T::value_type>
	int recv(T&& data, int src, MPI_Status& status){
	  MessageGroupHandler<C,tag,size> h(comm, requests);
	  return h.recv(data, src, status);
	}

	template<Tag tag, size_t size, typename T>
	int recv(T* data, int src, MPI_Status& status){
	  MessageGroupHandler<T,tag,size> h(comm, requests);
	  return h.template recv(data, src, status);
	}

	template<typename T>
	void run(T cgf){
	  typedef function_traits<decltype(cgf)> traits;
	  using Handler = std::remove_reference_t<typename traits::template arg<0>::type>;
//			static_assert(is_instance<Handler,MessageGroupHandler>{},"err");

	  Handler cgh(comm, requests);
	  cgf(cgh);

	}

   private:
	MPI_Comm comm;
	request_pool requests;

  };
}
#endif //EMPI_PROJECT_INCLUDE_EMPI_MESSAGE_GROUP_HPP_
