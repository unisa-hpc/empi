#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <mpi.h>
#include <span>

#include <empi/message_grp_hdl.hpp>

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

			template<typename T, int TAG>
			void send(std::span<T> data, int dest) const{
				MessageGroupHandler<T,TAG> cgh{MPI_COMM_WORLD};
				cgh.send(data,dest);
			}

			template<typename T, int TAG>
			void recv(std::span<T> data, int src) const{
				MessageGroupHandler<T,TAG> cgh{MPI_COMM_WORLD};
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





		private:
			int _rank;
			int _size;
			int _prev;
			int _succ;

	};

};
#endif // __CONTEXT_H__