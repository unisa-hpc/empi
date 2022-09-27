#ifndef EMPI_PROJECT_CONTEXT_HPP
#define EMPI_PROJECT_CONTEXT_HPP

#include <mpi.h>
#include <span>
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
            MPI_Init(argc, argv);
            MPI_Comm_rank(MPI_COMM_WORLD, &_rank);
            MPI_Comm_size(MPI_COMM_WORLD, &_size);
            _succ = _rank + 1 % _size;
            _prev = _rank == 0 ? 0 : _rank - 1 % _size;
        }

        Context(const Context& c) = delete;
        Context(Context&& c) = default;

        ~Context(){
            MPI_Finalize();
        }

        template<typename T>
        void run(T cgf){
			typedef function_traits<decltype(cgf)> traits;
			using Handler = std::remove_reference_t<typename traits::template arg<0>::type>;
//			static_assert(is_instance<Handler,MessageGroupHandler>{},"err");

            Handler cgh{MPI_COMM_WORLD};

            cgf(cgh);
        }

        template<typename T, Tag TAG>
        void send(std::span<T> data, int dest) const{
            MessageGroupHandler<T,TAG,0> cgh{MPI_COMM_WORLD};
            cgh.send(data,dest);
        }

        template<typename T, Tag TAG>
        void recv(std::span<T> data, int src) const{
            MessageGroupHandler<T,TAG,0> cgh{MPI_COMM_WORLD};
            cgh.recv(data, src);
        }


        int rank() const{
            return _rank;
        }

        int size() const{
            return _size;
        }

        int prev() const{
            return _prev;
        }

        int succ() const{
            return _succ;
        }

		std::unique_ptr<MessageGroup> create_message_group(MPI_Comm comm) {
		return std::make_unique<MessageGroup>(comm);
	  }

	 private:
         int _rank;
         int _size;
         int _prev;
         int _succ;

	};



};

#endif // __EMPI_CONTEXT_H__