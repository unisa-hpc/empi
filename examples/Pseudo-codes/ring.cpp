//ring pattern
//https://github.com/open-mpi/ompi/blob/main/examples/ring_c.c

while (1) {
        MPI_Recv(&message, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (0 == rank) {
            --message;
            printf("Process 0 decremented value: %d\n", message);
        }

        MPI_Send(&message, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
        if (0 == message) {
            printf("Process %d exiting\n", rank);
            break;
        }
    }