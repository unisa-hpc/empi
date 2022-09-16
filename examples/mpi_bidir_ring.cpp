#include "mpi.h"
#include <iostream>

int main(int argc, char **argv) {
  int procs, err, myid, next, prev, buf[2], tag1=1, tag2=2;
  
  MPI_Request reqs[4];
  MPI_Status status[2];

  err = MPI_Init(&argc, &argv);
	if (err != MPI_SUCCESS){
    printf("\nError initializing MPI.\n");
	  MPI_Abort(MPI_COMM_WORLD, err);
  }

  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  prev = myid-1;
  next = myid+1;
  if (myid == 0) prev = procs - 1;
  if (myid == (procs - 1)) next = 0;

  MPI_Irecv(&buf[0], 1, MPI_INT, prev, tag1, MPI_COMM_WORLD, &reqs[0]);
  MPI_Irecv(&buf[1], 1, MPI_INT, next, tag2, MPI_COMM_WORLD, &reqs[1]);
  MPI_Isend(&myid, 1, MPI_INT, prev, tag2, MPI_COMM_WORLD, &reqs[2]);
  MPI_Isend(&myid, 1, MPI_INT, next, tag1, MPI_COMM_WORLD, &reqs[3]);
  //Do some work
  printf("Process %d received from %d and passed to %d\n", myid, prev, next);
  
  MPI_Waitall(4, reqs, MPI_STATUSES_IGNORE);
  MPI_Finalize();
  return 0; 
}
