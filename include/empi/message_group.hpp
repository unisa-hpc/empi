//
// Created by luigi on 26/09/22.
//

#ifndef EMPI_PROJECT_INCLUDE_EMPI_MESSAGE_GROUP_HPP_
#define EMPI_PROJECT_INCLUDE_EMPI_MESSAGE_GROUP_HPP_

#include "empi/async_event.hpp"
#include <mpi.h>
#include <memory>

#include <empi/request_pool.hpp>
#include <empi/message_grp_hdl.hpp>
#include <empi/tag.hpp>
#include <empi/utils.hpp>
#include <empi/type_traits.hpp>
#include <empi/empi_defines.hpp>

namespace empi {
  class MessageGroup {
   public:
	explicit MessageGroup(MPI_Comm comm) : comm(comm) {
	  MPI_Checkcomm(comm); //TODO: exception?
	  _request_pool = std::make_shared<request_pool>();
	}

	//Wait an all Message in this group, so that no request is pending
	~MessageGroup(){
	  wait_all();
	}

	int barrier(){
	  return MPI_Barrier(comm);
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

	// ------------------ ISEND --------------------------------------

	template<Tag tag, size_t size, typename T>
	std::shared_ptr<async_event> Isend(T&& data, int dest){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, tag, size> h(comm, _request_pool);
		return h.template Isend(data,dest);
	  }
	  else{
		MessageGroupHandler<T, tag, size> h(comm, _request_pool);
		return h.template Isend(data,dest);
	  }
	}

	template<Tag tag, typename T>
	std::shared_ptr<async_event> Isend(T&& data, int dest, int size){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, tag, NOSIZE> h(comm, _request_pool);
		return h.template Isend(data,dest,size);
	  }
	  else{
		MessageGroupHandler<T, tag, NOSIZE> h(comm, _request_pool);
		return h.template Isend(data,dest, size);
	  }
	}

	template<int size, typename T>
	std::shared_ptr<async_event> Isend(T&& data, int dest, Tag tag){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, NOTAG, size> h(comm, _request_pool);
		return h.template Isend(data,dest,tag);
	  }
	  else{
		MessageGroupHandler<T, NOTAG, size> h(comm, _request_pool);
		return h.template Isend(data,dest,tag);
	  }
	}

	template<typename T>
	std::shared_ptr<async_event> Isend(T&& data, int dest, int size, Tag tag){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, NOTAG, NOSIZE> h(comm, _request_pool);
		return h.template Isend(data,dest,size, tag);
	  }
	  else{
		MessageGroupHandler<T, NOTAG, NOSIZE> h(comm, _request_pool);
		return h.template Isend(data,dest,size,tag);
	  }
	}

	// ------------------ END ISEND --------------------------------------
	// ------------------ IRECV --------------------------------------

	template<Tag tag, size_t size, typename T>
	std::shared_ptr<async_event> Irecv(T&& data, int src){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, tag, size> h(comm, _request_pool);
		return h.template Irecv(data,src);
	  }
	  else{
		MessageGroupHandler<T, tag, size> h(comm, _request_pool);
		return h.template Irecv(data,src);
	  }
	}

	template<size_t size, typename T>
	std::shared_ptr<async_event> Irecv(T&& data, int src, Tag tag){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, NOTAG, size> h(comm, _request_pool);
		return h.template Irecv(data,src, tag);
	  }
	  else{
		MessageGroupHandler<T, NOTAG, size> h(comm, _request_pool);
		return h.template Irecv(data,src, tag);
	  }
	}

	template<Tag tag, typename T>
	std::shared_ptr<async_event> Irecv(T&& data, int src, int size){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, tag, NOSIZE> h(comm, _request_pool);
		return h.template Irecv(data,src,size);
	  }
	  else{
		MessageGroupHandler<T, tag, NOSIZE> h(comm, _request_pool);
		return h.template Irecv(data,src,size);
	  }
	}

	template<typename T>
	std::shared_ptr<async_event> Irecv(T&& data, int src, int size, Tag tag){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, NOTAG, NOSIZE> h(comm, _request_pool);
		return h.template Irecv(data,src,size,tag);
	  }
	  else{
		MessageGroupHandler<T, NOTAG, NOSIZE> h(comm, _request_pool);
		return h.template Irecv(data,src,size,tag);
	  }
	}

	// ------------------ END IRECV --------------------------------------
	// ------------------ BCAST -----------------------------

	template<size_t size, typename T>
	int Bcast(T&& data, int root){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, NOTAG, size> h(comm, _request_pool);
		return h.template Bcast(data, root);
	  }
	  else{
		MessageGroupHandler<T, NOTAG, size> h(comm, _request_pool);
		return h.template Bcast(data, root);
	  }
	}

	template<typename T>
	int Bcast(T&& data, int root, int size){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, NOTAG, NOSIZE> h(comm, _request_pool);
		return h.template Bcast(data, root, size);
	  }
	  else{
		MessageGroupHandler<T, NOTAG, NOSIZE> h(comm, _request_pool);
		return h.template Bcast(data, root, size);
	  }
	}

	// ------------------ END BCAST -----------------------------
	// ------------------ IBCAST -----------------------------

	template<size_t size, typename T>
	std::shared_ptr<async_event> Ibcast(T&& data, int root){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, NOTAG, size> h(comm, _request_pool);
		return h.template Ibcast(data, root);
	  }
	  else{
		MessageGroupHandler<T, NOTAG, size> h(comm, _request_pool);
		return h.template Ibcast(data, root);
	  }
	}

	template<typename T>
	std::shared_ptr<async_event> Ibcast(T&& data, int root, int size){
	  if constexpr (has_data<T>) {
		MessageGroupHandler<typename T::value_type, NOTAG, NOSIZE> h(comm, _request_pool);
		return h.template Ibcast(data, root, size);
	  }
	  else{
		MessageGroupHandler<T, NOTAG, NOSIZE> h(comm, _request_pool);
		return h.template Ibcast(data, root, size);
	  }
	}
	// ------------------ END BCAST -----------------------------

	// ------------------ ALLREDUCE -----------------------------

	template<size_t size, typename T>
	int Allreduce(T&& sendbuf, T&& recvbuf, MPI_Op op){
		MessageGroupHandler<typename get_true_type<T>::type, NOTAG, size> h(comm, _request_pool);
		return h.template Allreduce<T>(std::forward<T>(sendbuf),std::forward<T>(recvbuf),op);
	}

	template<typename T>
	int Allreduce(T&& sendbuf, T&& recvbuf, int size, MPI_Op op){
	  MessageGroupHandler<typename get_true_type<T>::type , NOTAG, NOSIZE> h(comm, _request_pool);
	  return h.template Allreduce(sendbuf,recvbuf,size,op);
	}
	// ------------------ END ALLREDUCE -----------------------------



	template<typename T>
	void run(T cgf){
	  typedef function_traits<decltype(cgf)> traits;
	  using Handler = std::remove_reference_t<typename traits::template arg<0>::type>;
	  Handler cgh(comm, _request_pool);
	  cgf(cgh);
	}

	template<typename T>
	void run_and_wait(T cgf){
	  typedef function_traits<decltype(cgf)> traits;
	  using Handler = std::remove_reference_t<typename traits::template arg<0>::type>;

	  Handler cgh(comm, _request_pool);
	  cgf(cgh);
	  wait_all();
	}

	void wait_all(){
	//   for(auto& event : *_request_pool){
	// 	MPI_Status status;
	// 	MPI_Wait(event->request.get(),&status);
	// 	//TODO: how we handle those errors?
	//   }
	}

   private:
	MPI_Comm comm;
	std::shared_ptr<request_pool> _request_pool;

  };
}
#endif //EMPI_PROJECT_INCLUDE_EMPI_MESSAGE_GROUP_HPP_
