#ifndef __MESSAGE_GRP_HDL_H__
#define __MESSAGE_GRP_HDL_H__

#include <empi/context.hpp>
#include <empi/type_traits.hpp>
#include <span>


namespace empi{

	template<typename T, size_t TAG>
	class MessageGroupHandler{

		friend class Context;
		public:
			
			explicit MessageGroupHandler(int comm) : communicator(comm) {}

			MessageGroupHandler(const MessageGroupHandler& chg) = default;
			MessageGroupHandler(MessageGroupHandler&& chg) = default;


			void send(std::span<T> data, int dest){
				MPI_Send(data.data(), data.size(), MpiType<T>::value, dest, TAG, communicator);
			}
			
			 
			void recv(std::span<T> data, int src){
				MPI_Status s;
				MPI_Recv(data.data(), data.size(), MpiType<T>::value, src, TAG, communicator, &s);
			}
		




		private:
			MPI_Comm communicator;

	};


}
#endif // __MESSAGE_GRP_HDL_H__