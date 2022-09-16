#include <mpi.h>  
#include <iostream>

int main(int argc, char **argv){
  int myid, procs, err;

	// initialize MPI_Init
	err = MPI_Init(&argc, &argv);
	if (err != MPI_SUCCESS){
    printf("\nError initializing MPI.\n");
	  MPI_Abort(MPI_COMM_WORLD, err);
  }

	// Get No. of processors
	MPI_Comm_size(MPI_COMM_WORLD,&procs);
		
	// Get processor id		
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	printf("\tI am processor %d: Total No. of processors =  %d.\n", myid, procs);
	printf("\tHello from processor %d.\n", myid);
	MPI_Finalize();
	return 0;  
}
