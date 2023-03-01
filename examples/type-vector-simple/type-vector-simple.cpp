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
    int position = 0;
    double t1, t2, t3, t4, tu1, tu2;

    err = MPI_Init(&argc, &argv);
    if (err != MPI_SUCCESS)
    {
        printf("\nError initializing MPI.\n");
        MPI_Abort(MPI_COMM_WORLD, err);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    

    n = atoi(argv[1]);
    sub_m = atoi(argv[2]);
    // sub_n = atoi(argv[3]);
    // stride = atoi(argv[4]);
    // max_iter = atoi(argv[5]);   //number of iterations to repeat the procedure

    int sendarr [n][n];
    int recarr [n][n];
    memset( recarr, 0, n*n*sizeof(int) );
    for (int i = 0; i<n; i++)
    {
        for (int j = 0; j<n; j++)
            sendarr[i][j] = 1;
    }
    
    int i, j;

    t1 = MPI_Wtime();
    MPI_Datatype mytype;
    MPI_Type_vector( 2, n, 0, MPI_INT, &mytype );
    MPI_Type_commit( &mytype );
    t2 = MPI_Wtime();

    if ( myid == 0 )
    {   
        t3 = MPI_Wtime();
        MPI_Send( &sendarr[sub_m][0], 2, mytype, 1, 0, MPI_COMM_WORLD );
        t4 = MPI_Wtime();
    }
    else
    {   
        // for (i = 0; i < n; i++)
        // {
        //     printf("\n" );
        //     for (j=0; j < n; j++)
        //         printf("%i ", recarr[i][j] );
        // }
        MPI_Recv( &recarr[sub_m][0], 2, mytype, 0, 0, MPI_COMM_WORLD, &mystatus );
        printf("\n" );
        // for (i = 0; i < n; i++)
        // {
        //     printf("\n" );
        //     for (j=0; j < n; j++)
        //         printf("%i ", recarr[i][j] );
        // }
    }    
     
    MPI_Barrier(MPI_COMM_WORLD);
    if ( myid == 0 )
    {  
        printf("\nMaking DDtype time:     %lf  us\n", ((t2 - t1) * 1e6));
        printf("\nSending time:     %lf  us\n", ((t4 - t3) * 1e6));
    }


    MPI_Finalize( );
    return 0;
}




