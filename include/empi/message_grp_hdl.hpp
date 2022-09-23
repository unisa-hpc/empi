#ifndef EMPI_PROJECT_MGH_HPP
#define EMPI_PROJECT_MGH_HPP

#include <empi/type_traits.hpp>
#include <empi/tag.hpp>
#include <span>


namespace empi{

	template<typename T, Tag TAG, std::size_t SIZE>
	class MessageGroupHandler{

		public:
			
			explicit MessageGroupHandler(int comm) : communicator(comm) {}

			MessageGroupHandler(const MessageGroupHandler& chg) = default;
			MessageGroupHandler(MessageGroupHandler&& chg)  noexcept = default;

            template<typename T1 = T, std::enable_if_t<(sizeof(T1), SIZE == 0),bool> = true>
			void send(std::span<T> data, int dest){
				MPI_Send(data.data(), data.size(), MpiType<T>::value, dest, TAG.value, communicator);
			}

            template<typename T1 = T, std::enable_if_t<(sizeof(T1), SIZE > 0),bool> = true>
            void send(std::span<T> data, int dest){
                MPI_Send(data.data(),SIZE, MpiType<T>::value, dest, TAG.value, communicator);
            }

            template<typename T1 = T, std::enable_if_t<(sizeof(T1), SIZE == 0),bool> = true>
            void recv(std::span<T> data, int src){
                MPI_Status s;
                MPI_Recv(data.data(), data.size(), MpiType<T>::value, src, TAG.value, communicator, &s);
            }

            template<typename T1 = T, std::enable_if_t<(sizeof(T1), SIZE > 0),bool> = true>
            void recv(std::span<T> data, int src){
                MPI_Status s;
                MPI_Recv(data.data(), data.size(), MpiType<T>::value, src, TAG.value, communicator, &s);
            }


		private:
			MPI_Comm communicator;

	};


}
#endif // __MESSAGE_GRP_HDL_H__