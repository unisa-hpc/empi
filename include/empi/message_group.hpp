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
	  _request_pool = std::make_shared<request_pool_t>();
	}

	//Wait an all Message in this group, so that no request is pending
	~MessageGroup(){
	  wait_all();
	}

	//---------------- SEND ------------------

	template<Tag tag, size_t size, typename T, typename C = typename T::value_type>
	int send(T&& data, int dest){
	  MessageGroupHandler<C,tag,size> h(comm, _request_pool);
	  return h.template send(data,dest);
	}

	template<Tag tag, size_t size, typename T>
	int send(const T* data, int dest){
	  MessageGroupHandler<T,tag,size> h(comm, _request_pool);
	  return h.template send(data,dest);
	}

	template<size_t size, typename T>
	int send(T&& data, int dest, Tag tag){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, NOTAG, size> h(comm, _request_pool);
		return h.template send(data,dest,tag);
	  }
	  else{
		MessageGroupHandler<T, NOTAG, size> h(comm, _request_pool);
		return h.template send(data,dest,tag);
	  }
	}

	template<size_t size, typename T>
	int send(const T* data, int dest, Tag tag){
	  MessageGroupHandler<T,NOTAG,size> h(comm, _request_pool);
	  return h.template send(data,dest,tag);
	}

	template<Tag tag, typename T, typename C = typename T::value_type>
	int send(T&& data, int dest, size_t size){
	  MessageGroupHandler<C,tag,NOSIZE> h(comm, _request_pool);
	  return h.template send(data,dest,size);
	}

	template<Tag tag, typename T>
	int send(const T* data, int dest, size_t size){
	  MessageGroupHandler<T,tag,NOSIZE> h(comm, _request_pool);
	  return h.template send(data,dest,size);
	}

	template<typename T, typename C = typename T::value_type>
	int send(T&& data, int dest, size_t size, Tag tag){
	  MessageGroupHandler<C,NOTAG,NOSIZE> h(comm, _request_pool);
	  return h.template send(data,dest,size,tag);
	}

	template<typename T>
	int send(const T* data, int dest, size_t size, Tag tag){
	  MessageGroupHandler<T,NOTAG,NOSIZE> h(comm, _request_pool);
	  return h.template send(data,dest,size,tag);
	}

	// ------------------------- END SEND -------------------------------

	template<Tag tag, size_t size, typename T, typename C = typename T::value_type>
	int recv(T&& data, int src, MPI_Status& status){
	  MessageGroupHandler<C,tag,size> h(comm, _request_pool);
	  return h.recv(data, src, status);
	}

	template<Tag tag, size_t size, typename T>
	int recv(T* data, int src, MPI_Status& status){
	  MessageGroupHandler<T,tag,size> h(comm, _request_pool);
	  return h.template recv(data, src, status);
	}

	template<typename T>
	void run(T cgf){
	  typedef function_traits<decltype(cgf)> traits;
	  using Handler = std::remove_reference_t<typename traits::template arg<0>::type>;

	  Handler cgh(comm, _request_pool);
	  cgf(cgh);

	}

	void wait_all(){
	  for(auto& event : *_request_pool){
		MPI_Status status;
		MPI_Wait(event->request.get(),&status);
		//TODO: how we handle those errors?
	  }
	}

   private:
	MPI_Comm comm;
	request_pool _request_pool;

  };
}
#endif //EMPI_PROJECT_INCLUDE_EMPI_MESSAGE_GROUP_HPP_
