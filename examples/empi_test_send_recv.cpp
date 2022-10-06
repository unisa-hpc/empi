#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>
#include <bits/stdc++.h>

#include <empi/empi.hpp>
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

  empi::Context ctx(&argc, &argv);

  // ------ PARAMETER SETUP -----------
  pow_2 = atoi(argv[1]);
  max_iter = atoi(argv[2]);
  sleep_time = atoi(argv[3]);
  double mpi_time[max_iter];
  nBytes = pow(2, pow_2);
  n = nBytes / sizeof(int);

  myarr = new int[n];
  arr = new int[n];

  if (ctx.rank() == 0)
  {
	for (int j = 0; j < n; j++)
	  arr[j] = rand() % range;
  }

  MPI_Status status;
  auto message_group = ctx.create_message_group(MPI_COMM_WORLD);
  message_group->run([&](empi::MessageGroupHandler<int,empi::Tag{0},empi::NOSIZE> &mgh) {
	while (iter < max_iter)
	{
	  message_group->barrier();
	  if(ctx.rank() == 0)
		t_start = MPI_Wtime();

	  if (ctx.rank() == 0)
	  {

		mgh.send(arr,1,n);
		mgh.recv(arr,1,n,status);
	  }
	  else
	  {
		mgh.recv(arr,0,n,status);
		mgh.send(arr,0,n);
	  }
	  message_group->barrier();
	  if(ctx.rank() == 0){
		t_end = MPI_Wtime();
		mpi_time[iter] = ((t_end - t_start) * 1000000) / 2;
	  }

	  usleep(sleep_time); //waits between iterations in microseconds
	  iter++;
	}
  });

  message_group->barrier();

  if (ctx.rank() == 0)
  {
	cout << "\nData Size: " << nBytes << " bytes\n";
	cout << "Mean of communication times: " << Mean(mpi_time, max_iter) << "\n";
	cout << "Median of communication times: " << Median(mpi_time, max_iter) << "\n";
//	Print_times(mpi_time, max_iter);
  }
  free(arr);
  free(myarr);
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