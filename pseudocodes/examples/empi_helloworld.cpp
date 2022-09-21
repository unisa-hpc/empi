// TODO: 
//  - error handling
//  - so far: Context = mpi program (Init+Finalize) + communicator, problem: how do we handle multiple communicators?

#include <empi.h>  
#include <iostream>

int main(int argc, char **argv){
  empi::Context ctx(&argc, &argv); 
  printf("\tI am processor %d: Total No. of processors =  %d.\n", ctx.rank(), ctx.size());
  printf("\tHello from processor %d.\n", ctx.rank());
  return 0;  
}

