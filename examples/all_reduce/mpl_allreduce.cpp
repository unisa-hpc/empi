#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <malloc.h>
#include <mpi.h>
#include <mpl/operator.hpp>
#include <unistd.h>
#include <mpl/mpl.hpp>
#include <vector>

using namespace std;
using value_type = int;

double Mean(double[], int);
double Median(double[], int);
void Print_times(double[], int);

int main(int argc, char **argv) {
  double t_start, t_end;
  double mpi_time = 0.0;
  constexpr int SCALE = 1000000;

  int err;
  long pow_2_bytes;
  int n;
  int myid;
  long max_iter;

  MPI_Status status;

  // ------ PARAMETER SETUP -----------
  pow_2_bytes = strtol(argv[1], nullptr, 10);
  n = static_cast<int>(std::pow(2, pow_2_bytes));
  max_iter = strtol(argv[2], nullptr, 10);

  std::vector<value_type> myarr(n);
  std::vector<value_type> arr(n);

  const mpl::communicator &comm_world(mpl::environment::comm_world());
  mpl::contiguous_layout<value_type> l(n);
  mpl::irequest_pool events;

  // Warmup
  comm_world.barrier();
  comm_world.allreduce(mpl::plus<value_type>(), myarr.data(), arr.data(),l);
  comm_world.barrier();

  // main measurement
  if (comm_world.rank() == 0)
    t_start = mpl::environment::wtime();

  for (auto iter = 0; iter < max_iter; iter++) {
    comm_world.allreduce(mpl::plus<value_type>(), myarr.data(), arr.data(),l);
  }

  comm_world.barrier();
  if (comm_world.rank() == 0) {
    t_end = mpl::environment::wtime();
    mpi_time = (t_end - t_start) * SCALE;
  }

  comm_world.barrier();

  if (comm_world.rank() == 0) {
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