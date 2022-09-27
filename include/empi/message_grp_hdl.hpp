#ifndef EMPI_PROJECT_MGH_HPP
#define EMPI_PROJECT_MGH_HPP

#include <span>

#include <empi/type_traits.hpp>
#include <empi/tag.hpp>
#include <empi/empi_defines.hpp>

namespace empi{



	template<typename T, Tag TAG = NOTAG, std::size_t SIZE = 0>
	class MessageGroupHandler{

		public:
		  explicit MessageGroupHandler(int comm, request_pool request_pool_) : communicator(comm), request_pool_(request_pool_) {}
		  MessageGroupHandler(const MessageGroupHandler& chg) = default;
		  MessageGroupHandler(MessageGroupHandler&& chg)  noexcept = default;

		  // -------------- SEND -----------------------------------------

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE > 0) && (TAG != -1)
		  int send(K& data, int dest){
			return MPI_Send(data.data(),SIZE, MpiType<C>::value, dest, TAG.value, communicator);
		  }

		  template<typename K>
		  requires std::is_same_v<T,std::remove_const_t<K>> && (SIZE > 0) && (TAG != -1)
		  int send(K* data, int dest){
			return MPI_Send(data,SIZE, MpiType<T>::value, dest, TAG.value, communicator);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE > 0) && (TAG == NOTAG)
		  int send(K& data, int dest, Tag tag){
			return MPI_Send(data.data(),SIZE, MpiType<C>::value, dest, tag.value, communicator);
		  }

		  template<typename K>
		  requires std::is_same_v<T,std::remove_const_t<K>> && (SIZE > 0) && (TAG == NOTAG)
		  int send(K* data, int dest, Tag tag){
			return MPI_Send(data,SIZE, MpiType<T>::value, dest, tag.value, communicator);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE == NOSIZE) && (TAG != -1)
		  int send(K& data, int dest, size_t size){
			return MPI_Send(data.data(),size, MpiType<C>::value, dest, TAG.value, communicator);
		  }

		  template<typename K>
		  requires std::is_same_v<T,std::remove_const_t<K>> && (SIZE == NOSIZE) && (TAG != -1)
		  int send(K* data, int dest, size_t size){
			return MPI_Send(data,size, MpiType<T>::value, dest, TAG.value, communicator);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE == NOSIZE) && (TAG == NOTAG)
		  int send(K& data, int dest, size_t size, Tag tag){
			return MPI_Send(data.data(),size, MpiType<C>::value, dest, tag.value, communicator);
		  }

		  template<typename K>
		  requires std::is_same_v<T,std::remove_const_t<K>> && (SIZE == NOSIZE) && (TAG == NOTAG)
		  int send(K* data, int dest, size_t size, Tag tag){
			return MPI_Send(data,size, MpiType<T>::value, dest, tag.value, communicator);
		  }

		  // ---------------------------- END SEND --------------------------------

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE > 0) && (TAG.value >= -1)
		  int recv(K& data, int src, MPI_Status& status){
			return MPI_Recv(data.data(),SIZE, MpiType<C>::value, src, TAG.value, communicator, &status);
		  }

		  template<typename K>
		  requires std::is_same_v<T,K> && (SIZE > 0) && (TAG.value >= -1)
		  int recv(K* data, int src, MPI_Status& status){
			return MPI_Recv(data,SIZE, MpiType<T>::value, src, TAG.value, communicator, &status);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE == NOSIZE) && (TAG.value >= -1)
		  int recv(K& data, int src, size_t size, MPI_Status& status){
			return MPI_Recv(data.data(),size, MpiType<C>::value, src, TAG.value, communicator, &status);
		  }

		  template<typename K>
		  requires std::is_same_v<T,K> && (SIZE == NOSIZE) && (TAG.value >= -1)
		  int recv(K* data, int src, size_t size, MPI_Status& status){
			return MPI_Recv(data,size, MpiType<T>::value, src, TAG.value, communicator, &status);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE > 0) && (TAG == NOTAG)
		  int recv(K& data, int src, Tag tag, MPI_Status& status){
			return MPI_Recv(data.data(),SIZE, MpiType<C>::value, src, tag.value, communicator, &status);
		  }

		  template<typename K>
		  requires std::is_same_v<T,K> && (SIZE > 0) && (TAG == NOTAG)
		  int recv(K* data, int src, Tag tag, MPI_Status& status){
			return MPI_Recv(data,SIZE, MpiType<T>::value, src, tag.value, communicator, &status);
		  }

		  template<typename K, typename C = typename K::value_type>
		  requires has_data<K> && std::is_same_v<T,C> && (SIZE == NOSIZE) && (TAG == NOTAG)
		  int recv(K& data, int src, size_t size, Tag tag, MPI_Status& status){
			return MPI_Recv(data.data(),size, MpiType<C>::value, src, tag.value, communicator, &status);
		  }

		  template<typename K>
		  requires std::is_same_v<T,K> && (SIZE == NOSIZE) && (TAG == NOTAG)
		  int recv(K* data, int src, size_t size, Tag tag, MPI_Status& status){
			return MPI_Recv(data,size, MpiType<T>::value, src, tag.value, communicator, &status);
		  }

		  // ------------------------- END SEND -----------------------------


		private:
			MPI_Comm communicator;
			request_pool request_pool_;


	};


}
#endif // __MESSAGE_GRP_HDL_H__