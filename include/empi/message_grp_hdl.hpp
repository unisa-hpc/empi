#ifndef EMPI_PROJECT_MGH_HPP
#define EMPI_PROJECT_MGH_HPP

#include "mpi.h"
#include <memory>
#include <limits>

#include <empi/request_pool.hpp>
#include <empi/type_traits.hpp>
#include <empi/tag.hpp>

#include <empi/request.hpp>
#include <empi/async_event.hpp>
#include <empi/utils.hpp>
#include <empi/datatype.hpp>
#include <empi/defines.hpp>

namespace empi{


	template<typename T1, Tag TAG = NOTAG, std::size_t SIZE = 0>
	class MessageGroupHandler{

	  	using T = remove_all_t<T1>;

		public:
		  explicit MessageGroupHandler(MPI_Comm comm, std::shared_ptr<request_pool> _request_pool) : communicator(comm), _request_pool(_request_pool) {
			// MPI_Datatype type = details::mpi_type<T>::get_type();
			// int flag;
			// MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_TAG_UB, &max_tag, &flag);
			// if(!flag){
			//   max_tag = -1;
			// }
			// EMPI_CHECKTYPE(type); //TODO: exceptions?
		  }

		  MessageGroupHandler(const MessageGroupHandler& chg) = default;
		  MessageGroupHandler(MessageGroupHandler&& chg)  noexcept = default;
		
		 // -------------- UTILITY -----------------------------

		int inline barrier() const {
			return MPI_Barrier(communicator);
		}

		void waitall() {
			_request_pool->waitall();
		}

		  // -------------- SEND -----------------------------------------
		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0) && (TAG != -1)
		  int send(K&& data, int dest){
			return EMPI_SEND(details::get_underlying_pointer(data), SIZE,  details::mpi_type<T>::get_type(), dest, TAG.value, communicator);
		  }

		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0) && (TAG == NOTAG)
		  int send(K&& data, int dest, Tag tag){
			details::checktag<details::mpi_function::send>(tag.value, max_tag);
			return EMPI_SEND(details::get_underlying_pointer(data), SIZE,  details::mpi_type<T>::get_type(), dest, tag.value, communicator);
		  }

		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE) && (TAG != -1)
		  int inline send(K&& data, int dest, size_t size){
			return EMPI_SEND(details::get_underlying_pointer(data), size,  details::mpi_type<T>::get_type(), dest, TAG.value, communicator);
		  }

		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE) && (TAG == NOTAG)
		  int send(K&& data, int dest, size_t size, Tag tag){
			details::checktag<details::mpi_function::send>(tag.value, max_tag);
			return EMPI_SEND(details::get_underlying_pointer(data), size,  details::mpi_type<T>::get_type(), dest, tag.value, communicator);
		  }

		  // ---------------------------- END SEND --------------------------------

		  // ---------------------------- START RECV ------------------------------

		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0) && (TAG.value >= -1)
		  int recv(K&& data, int src, MPI_Status& status){
			return EMPI_RECV(details::get_underlying_pointer(data), SIZE,  details::mpi_type<T>::get_type(), src, TAG.value, communicator, &status);
		  }

		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE) && (TAG.value >= -1)
		  int inline recv(K&& data, int src, size_t size, MPI_Status& status){
			return EMPI_RECV(details::get_underlying_pointer(data), size,  details::mpi_type<T>::get_type(), src, TAG.value, communicator, &status);
		  }


		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0) && (TAG == NOTAG)
		  int recv(K&& data, int src, Tag tag, MPI_Status& status){
			details::checktag<details::mpi_function::recv>(tag.value, max_tag);
			return EMPI_RECV(details::get_underlying_pointer(data), SIZE,  details::mpi_type<T>::get_type(), src, tag.value, communicator, &status);
		  }

		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE) && (TAG == NOTAG)
		  int recv(K&& data, int src, size_t size, Tag tag, MPI_Status& status){
			details::checktag<details::mpi_function::recv>(tag.value, max_tag);
			return EMPI_RECV(details::get_underlying_pointer(data), size,  details::mpi_type<T>::get_type(), src, tag.value, communicator, &status);
		  }

		  // ------------------------- END RECV -----------------------------


		  // ------------------------- START ISEND --------------------------
		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0) && (TAG != -1)
		  std::shared_ptr<async_event>& Isend(K&& data, int dest){
			auto&& event = _request_pool->get_req();
			event->res = EMPI_ISEND(details::get_underlying_pointer(data), SIZE, details::mpi_type<T>::get_type(),dest,TAG.value,communicator,event->request.get());
			return event;
		  }


		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE) && (TAG != -1)
		  std::shared_ptr<async_event>& Isend(K&& data, int dest, int size){
			auto&& event = _request_pool->get_req();
			event->res = EMPI_ISEND(details::get_underlying_pointer(data), size, details::mpi_type<T>::get_type(),dest,TAG.value,communicator,event->get_request());
			return event;
		  }

		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0) && (TAG == NOTAG)
		  std::shared_ptr<async_event>& Isend(K&& data, int dest, Tag tag){
			details::checktag<details::mpi_function::isend>(tag.value, max_tag);
			auto&& event = _request_pool->get_req();
			event->res = EMPI_ISEND(details::get_underlying_pointer(data), SIZE, details::mpi_type<T>::get_type(),dest,tag.value,communicator,event->request.get());
			return event;
		  }

		  template<typename K>
		  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE) && (TAG == NOTAG)
		  std::shared_ptr<async_event>& Isend(K&& data, int dest, int size, Tag tag){
			details::checktag<details::mpi_function::isend>(tag.value, max_tag);
			auto&& event = _request_pool->get_req();
			event->res = EMPI_ISEND(details::get_underlying_pointer(data),size, details::mpi_type<T>::get_type(),dest,tag.value,communicator,event->request.get());
			return event;
		  }

	  // ------------------------- END ISEND -----------------------------


	  // ------------------------- START URECV --------------------------

		template<typename K>
		requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0) && (TAG >= -2)
		std::shared_ptr<async_event>& Irecv(K&& data, int src){
		  auto&& event = _request_pool->get_req();
		  event->res = EMPI_IRECV(details::get_underlying_pointer(data),SIZE, details::mpi_type<T>::get_type(),src,TAG.value,communicator,event->request.get());

		  return event;
		}

		template<typename K>
		requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE) && (TAG >= -2)
		std::shared_ptr<async_event>& Irecv(K&& data, int src, int size){
		  auto&& event = _request_pool->get_req();
		  event->res = EMPI_IRECV(details::get_underlying_pointer(data),size, details::mpi_type<T>::get_type(),src,TAG.value,communicator,event->request.get());

		  return event;
		}

		template<typename K>
		requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0) && (TAG == NOTAG)
		std::shared_ptr<async_event>& Irecv(K&& data, int src, Tag tag){
		  details::checktag<details::mpi_function::irecv>(tag.value, max_tag);
		  auto&& event = _request_pool->get_req();
		  event->res = EMPI_IRECV(details::get_underlying_pointer(data),SIZE, details::mpi_type<T>::get_type(),src,tag.value,communicator,event->request.get());

		  return event;
		}

		template<typename K>
		requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE) && (TAG == NOTAG)
		std::shared_ptr<async_event>& Irecv(K&& data, int src, int size, Tag tag){
		  details::checktag<details::mpi_function::irecv>(tag.value, max_tag);
		  auto&& event = _request_pool->get_req();
		  event->res = EMPI_IRECV(details::get_underlying_pointer(data),size, details::mpi_type<T>::get_type(),src,tag.value,communicator,event->request.get());

		  return event;
		}

	  // ------------------------- END URECV --------------------------
	  // ------------------------- BCAST --------------------------

	  template<typename K>
	  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0)
	  int Bcast(K&& data, int root){
		return EMPI_BCAST(details::get_underlying_pointer(std::forward<K>(data)), SIZE, details::mpi_type<T>::get_type(),root,communicator);
	  }

	  template<typename K>
	  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE)
	  int Bcast(K&& data, int root, int size){
		return EMPI_BCAST(details::get_underlying_pointer(std::forward<K>(data)), size, details::mpi_type<T>::get_type(),root,communicator);
	  }

	  // ------------------------- END BCAST --------------------------
	  // ------------------------- IBCAST --------------------------

	  template<typename K>
	  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0)
	  std::shared_ptr<async_event>& Ibcast(K&& data, int root){
		auto&& event = _request_pool->get_req();
		event->res = EMPI_IBCAST(details::get_underlying_pointer(data), SIZE, details::mpi_type<T>::get_type(),root,communicator, event->get_request());
		return event;
	  }

	  template<typename K>
	  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE)
	  std::shared_ptr<async_event>& Ibcast(K&& data, int root, int size){
		auto&& event = _request_pool->get_req();
		event->res = EMPI_IBCAST(details::get_underlying_pointer(data), size, details::mpi_type<T>::get_type(),root,communicator, event->get_request());
		return event;
	  }

	  // ------------------------- END IBCAST --------------------------
	  // ------------------------- ALLREDUCE --------------------------

	  template<typename K>
	  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE > 0)
	  int Allreduce(K&& sendbuf, K&& recvbuf, MPI_Op op){
		return EMPI_ALLREDUCE(details::get_underlying_pointer(sendbuf),details::get_underlying_pointer(recvbuf),SIZE,details::mpi_type<T>::get_type(),op,communicator);
	  }

	  template<typename K>
	  requires (is_valid_container<T,K> || is_valid_pointer<T,K>) && (SIZE == NOSIZE)
	  int Allreduce(K&& sendbuf, K&& recvbuf, int size, MPI_Op op){
			return EMPI_ALLREDUCE(sendbuf,recvbuf,size,details::mpi_type<T>::get_type(),op,communicator);
	  }

	  // ------------------------- END ALLREDUCE --------------------------
	  // ------------------------- GATHERV --------------------------
	template<typename K>
	  requires (is_valid_container<T,K> || is_valid_pointer<T,K>)
	  int gatherv(int root, K&& sendbuf,int sendcount, K&& recvbuf, int* recvcounts, int* displacements){
		return EMPI_GATHERV(details::get_underlying_pointer(sendbuf), 
						   sendcount,
						   details::mpi_type<T>::get_type(),
						   details::get_underlying_pointer(recvbuf),
						   recvcounts,
						   displacements,
						   details::mpi_type<T>::get_type(),
						   root,
						   communicator);
	  }
	  // ------------------------- END ALLREDUCE --------------------------


		private:
			MPI_Comm communicator;
			std::shared_ptr<request_pool> _request_pool;
			int max_tag;
	};

}
#endif // __MESSAGE_GRP_HDL_H__
