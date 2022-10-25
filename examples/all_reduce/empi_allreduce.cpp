#include "empi/context.hpp"
#include "empi/message_grp_hdl.hpp"
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <malloc.h>
#include <mpi.h>
#include <unistd.h>

#include <empi/empi.hpp>
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
  int myid, n, max_iter, pow_2;
  double t_start, t_end, t_start_inner;
  constexpr int SCALE = 1000000;

  empi::Context ctx(&argc, &argv);
  // ------ PARAMETER SETUP -----------
  pow_2 =    static_cast<int>(strtol(argv[1],nullptr,10));
  max_iter = static_cast<int>(strtol(argv[2],nullptr,10));
  double mpi_time = 0.0;
  n = static_cast<int>(std::pow(2, pow_2));

  std::vector<value_type> myarr(n, 0);
  std::vector<value_type> dest(n, 0);

  auto message_group = ctx.create_message_group(MPI_COMM_WORLD);
  MPI_Status status;

  message_group->run(
      [&](empi::MessageGroupHandler<value_type, empi::Tag{0}, empi::NOSIZE> &mgh) {
        // First iter
        mgh.Allreduce(myarr.data(),dest.data(),n,MPI_SUM);
        mgh.barrier();

        if (message_group->rank() == 0)
          t_start = MPI_Wtime();

        for(auto iter = 0; iter < max_iter; iter++){
          mgh.Allreduce(myarr.data(),dest.data(),n,MPI_SUM);
        }

        mgh.barrier();
        if (message_group->rank() == 0) {
          t_end = MPI_Wtime();
          mpi_time = (t_end - t_start) * SCALE;
        }
      });

  message_group->barrier();

  if (message_group->rank() == 0) {
    // cout << "\nData Size: " << nBytes << " bytes\n";
    cout << mpi_time << "\n";
    // cout << "Mean of communication times: " << Mean(mpi_time, num_restart)
    //      << "\n";
    // cout << "Median of communication times: " << Median(mpi_time,
    // num_restart)
    //      << "\n";
    // 	Print_times(mpi_time, num_restart);
  }
  return 0;
} // end main

double Mean(const double *a, int n) {
  double sum = 0.0;
  for (int i = 0; i < n; i++)
    sum += a[i];

  return (sum / (double)n);
}

double Median(double* a, int n) {
  sort(a, a + n);
  if (n % 2 != 0)
    return a[n / 2];

  return (a[(n - 1) / 2] + a[n / 2]) / 2.0;
}

void Print_times(const double *a, int n) {
  cout << "\n------------------------------------";
  for (int t = 0; t < n; t++)
    cout << "\n " << a[t];
}