#ifndef EMPI_PROJECT_MGH_HPP
#define EMPI_PROJECT_MGH_HPP

#include <span>

#include <empi/type_traits.hpp>
#include <empi/tag.hpp>
#include <empi/empi_defines.hpp>
#include <empi/request.hpp>
#include <empi/async_event.hpp>

namespace empi{



	template<typename T1, Tag TAG = NOTAG, std::size_t SIZE = 0>
	class MessageGroupHandler{

	  	using T = remove_all_t<T1>;

		public:
		  explicit MessageGroupHandler(MPI_Comm comm, request_pool request_pool_) : communicator(comm), request_pool_(request_pool_) {
			MPI_Datatype type = details::mpi_type<T>::get_type();
			int flag;
			MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_TAG_UB, &max_tag, &flag);
			if(!flag){
			  max_tag = -1;
			}
			MPI_Checktype(type);
			MPI_Checkcomm(communicator);

		  }

		  MessageGroupHandler(const MessageGroupHandler& chg) = default;
		  MessageGroupHandler(MessageGroupHandler&& chg)  noexcept = default;


		  // -------------- SEND -----------------------------------------

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE > 0) && (TAG != -1)
		  int send(K& data, int dest){
			return MPI_Send(data.data(),SIZE,  details::mpi_type<C>::get_type(), dest, TAG.value, communicator);
		  }

		  template<typename K>
		  requires std::is_same_v<T,remove_all_t<K>> && (SIZE > 0) && (TAG != -1)
		  int send(K* data, int dest){
			return MPI_Send(data,SIZE,  details::mpi_type<T>::get_type(), dest, TAG.value, communicator);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE > 0) && (TAG == NOTAG)
		  int send(K& data, int dest, Tag tag){
			return MPI_Send(data.data(),SIZE,  details::mpi_type<C>::get_type(), dest, tag.value, communicator);
		  }

		  template<typename K>
		  requires std::is_same_v<T,remove_all_t<K>> && (SIZE > 0) && (TAG == NOTAG)
		  int send(K* data, int dest, Tag tag){
			return MPI_Send(data,SIZE,  details::mpi_type<T>::get_type(), dest, tag.value, communicator);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE == NOSIZE) && (TAG != -1)
		  int send(K& data, int dest, size_t size){
			if(size == 0)
			  return MPI_ERR_SIZE;
			return MPI_Send(data.data(),size,  details::mpi_type<C>::get_type(), dest, TAG.value, communicator);
		  }

		  template<typename K>
		  requires std::is_same_v<T,remove_all_t<K>> && (SIZE == NOSIZE) && (TAG != -1)
		  int send(K* data, int dest, size_t size){
			return MPI_USend(data,size,  details::mpi_type<T>::get_type(), dest, TAG.value, communicator);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE == NOSIZE) && (TAG == NOTAG)
		  int send(K& data, int dest, size_t size, Tag tag){
			return MPI_Send(data.data(),size,  details::mpi_type<C>::get_type(), dest, tag.value, communicator);
		  }

		  template<typename K>
		  requires std::is_same_v<T,remove_all_t<K>> && (SIZE == NOSIZE) && (TAG == NOTAG)
		  int send(K* data, int dest, size_t size, Tag tag){
			return MPI_Send(data,size,  details::mpi_type<T>::get_type(), dest, tag.value, communicator);
		  }

		  // ---------------------------- END SEND --------------------------------

		  // ---------------------------- START RECV ------------------------------
		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE > 0) && (TAG.value >= -1)
		  int recv(K& data, int src, MPI_Status& status){
			return MPI_Recv(data.data(),SIZE,  details::mpi_type<C>::get_type(), src, TAG.value, communicator, &status);
		  }

		  template<typename K>
		  requires std::is_same_v<T,K> && (SIZE > 0) && (TAG.value >= -1)
		  int recv(K* data, int src, MPI_Status& status){
			return MPI_Recv(data,SIZE,  details::mpi_type<T>::get_type(), src, TAG.value, communicator, &status);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE == NOSIZE) && (TAG.value >= -1)
		  int recv(K& data, int src, size_t size, MPI_Status& status){
			return MPI_URecv(data.data(),size,  details::mpi_type<C>::get_type(), src, TAG.value, communicator, &status);
		  }

		  template<typename K>
		  requires std::is_same_v<T,K> && (SIZE == NOSIZE) && (TAG.value >= -1)
		  int recv(K* data, int src, size_t size, MPI_Status& status){
			return MPI_Recv(data,size,  details::mpi_type<T>::get_type(), src, TAG.value, communicator, &status);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE > 0) && (TAG == NOTAG)
		  int recv(K& data, int src, Tag tag, MPI_Status& status){
			return MPI_Recv(data.data(),SIZE,  details::mpi_type<C>::get_type(), src, tag.value, communicator, &status);
		  }

		  template<typename K>
		  requires std::is_same_v<T,K> && (SIZE > 0) && (TAG == NOTAG)
		  int recv(K* data, int src, Tag tag, MPI_Status& status){
			return MPI_Recv(data,SIZE,  details::mpi_type<T>::get_type(), src, tag.value, communicator, &status);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE == NOSIZE) && (TAG == NOTAG)
		  int recv(K& data, int src, size_t size, Tag tag, MPI_Status& status){
			return MPI_Recv(data.data(),size,  details::mpi_type<C>::get_type(), src, tag.value, communicator, &status);
		  }

		  template<typename K>
		  requires std::is_same_v<T,K> && (SIZE == NOSIZE) && (TAG == NOTAG)
		  int recv(K* data, int src, size_t size, Tag tag, MPI_Status& status){
			return MPI_Recv(data,size,  details::mpi_type<T>::get_type(), src, tag.value, communicator, &status);
		  }

		  // ------------------------- END RECV -----------------------------


		  // ------------------------- START ISEND --------------------------
		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE > 0) && (TAG != -1)
		  std::shared_ptr<async_event> Isend(K& data, int dest){
			auto event = std::make_shared<async_event>();
			event->res = MPI_Isend(data.data(),SIZE, details::mpi_type<C>::get_type(),dest,TAG.value,communicator, event->request.get());
			request_pool_->push_back(event);
			return event;
		  }

		  template<typename K>
		  requires std::is_same_v<T,remove_all_t<K>> && (SIZE > 0) && (TAG != -1)
		  std::shared_ptr<async_event> Isend(K* data, int dest){
			auto event = std::make_shared<async_event>();
			event->res = MPI_Isend(data,SIZE, details::mpi_type<T>::get_type(),dest,TAG.value,communicator,event->request.get());
			request_pool_->push_back(event);
			return event;
		  }


		private:
			MPI_Comm communicator;
			request_pool request_pool_;
			int max_tag;

	};

}
#endif // __MESSAGE_GRP_HDL_H__