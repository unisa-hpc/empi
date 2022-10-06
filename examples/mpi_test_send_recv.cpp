#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>
#include <bits/stdc++.h>
/**
A simple ping pong test that iterates many times to measure communication time between 2 nodes
HOW TO RUN: mpirun -n num_procs (2 in this example) a.out nBytes(size of data in bytes) max_iter(how many times does it itarate?) sleep_time(sleep time between iterations)
**/
using namespace std;

double Mean(double[], int);
double Median(double[], int);
void Print_times(double[], int);

int main(int argc, char **argv)
{
  int myid, procs, n, err, max_iter, nBytes, sleep_time, iter = 0, range = 100, *arr, *myarr, pow_2;
  double t_start = 0.0, t_end = 0.0;
  MPI_Status status;
  err = MPI_Init(&argc, &argv);
  if (err != MPI_SUCCESS)
  {
	cout << "\nError initializing MPI. \narg1: size of Data in Bytes, arg2: number of iterations, arg3: sleep time between iterations, arg4: name (ip) of second node\n";
	MPI_Abort(MPI_COMM_WORLD, err);
  }

  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  pow_2 = atoi(argv[1]);
  max_iter = atoi(argv[2]);
  sleep_time = atoi(argv[3]);
  // pongnode = argv[4];

  double mpi_time[max_iter];
  nBytes = pow(2, pow_2);
  n = nBytes / sizeof(int);

  myarr = new int[n];
  arr = new int[n];

  if (myid == 0)
  {
	for (int j = 0; j < n; j++)
	  arr[j] = rand() % range;
  }
  while (iter < max_iter)
  {
	MPI_Barrier(MPI_COMM_WORLD);
	if(myid == 0)
		t_start = MPI_Wtime();

	if (myid == 0)
	{

	  MPI_Send(arr, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
	  MPI_Recv(arr, n, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}
	else
	{ //Node rank 1
	  MPI_Recv(myarr, n, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	  MPI_Send(myarr, n, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(myid == 0)
	{
	  t_end = MPI_Wtime();
	  mpi_time[iter] = ((t_end - t_start) * 1000000) / 2;
	}

	usleep(sleep_time); //waits between iterations in microseconds
	iter++;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  if (myid == 0)
  {
	cout << "\nData Size: " << nBytes << " bytes\n";
	cout << "Mean of communication times: " << Mean(mpi_time, max_iter) << "\n";
	cout << "Median of communication times: " << Median(mpi_time, max_iter) << "\n";
  }
  free(arr);
  free(myarr);
  MPI_Finalize();
  return 0;
} // end main

double Mean(double a[], int n)
{
  double sum = 0.0;
  for (int i = 0; i < n; i++)
	sum += a[i];

  return (sum / (double)n);
}

double Median(double a[], int n)
{
  sort(a, a + n);
  if (n % 2 != 0)
	return a[n / 2];

  return (a[(n - 1) / 2] + a[n / 2]) / 2.0;
}

void Print_times(double a[], int n)
{
  cout << "\n------------------------------------";
  for (int t = 0; t < n; t++)
	cout << "\n " << a[t];
}