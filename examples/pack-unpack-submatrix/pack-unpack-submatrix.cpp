//  mpirun -np 2 a.out 10 3 --> will make n*n matrix and send rows 3,4 (consecutive rows)  

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>


int main( int argc, char ** argv )
{
    int myid, err, procs, n, sub_m, sub_n, stride, max_iter;
    MPI_Status mystatus;
    int i, j;
    int position = 0;
    double t1, t2, tp1, tp2, tu1, tu2;

    err = MPI_Init(&argc, &argv);
    if (err != MPI_SUCCESS)
    {
        printf("\nError initializing MPI.\n");
        MPI_Abort(MPI_COMM_WORLD, err);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    
    //original matrix is n*n
    //we want to send two consecutive rows of it, from the elements [sub_m][sub_n] and [sub_m + 1][sub_n] with a stride

    n = atoi(argv[1]);

    sub_m = atoi(argv[2]);
    // sub_n = atoi(argv[3]);
    // stride = atoi(argv[4]);
    // max_iter = atoi(argv[5]);   //number of iterations to repeat the procedure

    char buffer[n*n];
    int sendarr [n][n];
    int recarr [n][n];
    memset( recarr, 0, n*n*sizeof(int) );
    
    for (int i = 0; i<n; i++)
    {
        for (int j = 0; j<n; j++)
            sendarr[i][j] = 1;
    }

    //  for (int iter = 0 ; iter < max_iter ; iter++)
    //  {
        if ( myid == 0 )
        {   
            tp1 = MPI_Wtime();
            MPI_Pack( &sendarr[sub_m][n], n, MPI_INT, buffer, n*n, &position, MPI_COMM_WORLD ) ;
            MPI_Pack( &sendarr[sub_m + 1][n], n, MPI_INT, buffer, n*n, &position, MPI_COMM_WORLD ) ;
            tp2 = MPI_Wtime();
            
            t1 = MPI_Wtime();
            MPI_Send( buffer, n*n, MPI_PACKED, 1, 0, MPI_COMM_WORLD );
            t2 = MPI_Wtime();
        }

        else
        {   
            MPI_Recv( buffer, n*n, MPI_PACKED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &mystatus );
            
            tu1 = MPI_Wtime();
            MPI_Unpack( buffer, n*n, &position, &recarr[sub_m][n], n, MPI_INT, MPI_COMM_WORLD);
            MPI_Unpack( buffer, n*n, &position, &recarr[sub_m + 1][n], n, MPI_INT, MPI_COMM_WORLD);
            tu2 = MPI_Wtime();
            printf("\nUnpacking time:  %lf  us\n", ((tu2 - tu1) * 1e6));
           
            // printf("\n" );
            // for (i = 0; i < n; i++)
            // {
            //     printf("\n" );
            //     for (j=0; j < n; j++)
            //         printf("%i ", recarr[i][j] );
            // }

        }
    //  } 
    
    MPI_Barrier(MPI_COMM_WORLD); 

    if ( myid == 0 )
    {  
        printf("\npacking time:     %lf  us\n", ((tp2 - tp1) * 1e6));
        printf("\nSending time:     %lf  us\n", ((t2 - t1) * 1e6));
        // printf("\nData Size:        %lf  bytes\n", ((t2 - t1) * 1e6));
    }


    MPI_Finalize();
    return 0;
}



