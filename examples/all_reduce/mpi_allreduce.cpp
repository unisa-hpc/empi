#include <bits/stdc++.h>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <malloc.h>
#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

/**
A simple ping pong test that iterates many times to measure communication time
between 2 nodes HOW TO RUN: mpirun -n num_procs (2 in this example) a.out
nBytes(size of data in bytes) max_iter(how many times does it itarate?)
sleep_time(sleep time between iterations)
**/
using namespace std;

using value_type = int;

double Mean(double[], int);
double Median(double[], int);
void Print_times(double[], int);

int main(int argc, char **argv) {
  int myid, procs, n, err, max_iter, nBytes, sleep_time, iter = 0, range = 100,
                                                         pow_2;
  double t_start, t_end, t_start_inner;
  constexpr int SCALE = 1000000;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  // ------ PARAMETER SETUP -----------
  pow_2 =    static_cast<int>(strtol(argv[1],nullptr,10));
  max_iter = static_cast<int>(strtol(argv[2],nullptr,10));

  double mpi_time = 0.0;
  nBytes = std::pow(2, pow_2);
  n = nBytes;

  std::vector<value_type> arr(n, 0);
  std::vector<value_type> dest(n, 0);

  // Warmup
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Allreduce(arr.data(), dest.data(), n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);

  // main measurement
  if (myid == 0)
    t_start = MPI_Wtime();

  while (iter < max_iter) {
    MPI_Allreduce(arr.data(), dest.data(), n, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    iter++;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  if (myid == 0) {
    t_end = MPI_Wtime();
    mpi_time = (t_end - t_start) * SCALE;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  if (myid == 0) {
    // cout << "\nData Size: " << nBytes << " bytes\n";
    cout << mpi_time << "\n";
    // cout << "Mean of communication times: " << Mean(mpi_time , num_restart)
    //      << "\n";
    // cout << "Median of communication times: " << Median(mpi_time ,
    // num_restart )
    //      << "\n";
    // Print_times(mpi_time, num_restart);
  }


  MPI_Finalize();
  return 0;
} // end main

double Mean(double a[], int n) {
  double sum = 0.0;
  for (int i = 0; i < n; i++)
    sum += a[i];

  return (sum / (double)n);
}

double Median(double a[], int n) {
  sort(a, a + n);
  if (n % 2 != 0)
    return a[n / 2];

  return (a[(n - 1) / 2] + a[n / 2]) / 2.0;
}

void Print_times(double a[], int n) {
  cout << "\n------------------------------------";
  for (int t = 0; t < n; t++)
    cout << "\n " << a[t];
}