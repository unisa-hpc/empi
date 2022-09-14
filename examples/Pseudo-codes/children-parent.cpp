//miniAMR
//https://github.com/Mantevo/miniAMR/blob/master/ref/comm_parent.c

for (i = 0; i < par_b.num_comm_part; i++)
      MPI_Irecv(&recv_int[par_b.index[i]], par_b.comm_num[i], MPI_INT,
                 par_b.comm_part[i], type, MPI_COMM_WORLD, &request[i]);

for (i = 0; i < par_p.num_comm_part; i++) {
       { /* do some work */ }
         MPI_Isend(&send_int[par_p.index[i]], par_p.comm_num[i], MPI_INT,
                   par_p.comm_part[i], type, MPI_COMM_WORLD, &s_req[i]);
   }

for (i = 0; i < par_p.num_comm_part; i++)
         MPI_Waitany(par_p.num_comm_part, s_req, &which, &status);
