//p2p communication 
//https://mpitutorial.com/tutorials/mpi-send-and-receive/

if (world_rank == 0) {
    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	
} else if (world_rank == 1) {
    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
}
