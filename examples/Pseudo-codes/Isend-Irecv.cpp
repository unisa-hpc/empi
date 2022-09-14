//
MPI_Isend(send_buf, NELEMENTS, MPI_INT, dst, 0, MPI_COMM_WORLD, &req); 
MPI_Irecv(recv_buf, NELEMENTS, MPI_INT, dst, 0, MPI_COMM_WORLD, &req); 
MPI_Wait (&req, MPI_STATUS_IGNORE);
