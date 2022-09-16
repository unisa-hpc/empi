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

  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);

  int number;
  if (myid == 0) {
    number = -1;
    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else if (myid == 1) {
    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process 1 received number %d from process 0\n", number);
  }
  MPI_Finalize();
  return 0;
}
