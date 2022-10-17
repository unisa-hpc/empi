#include <chrono>
#include <cmath>
#include <iostream>
#include <malloc.h>
#include <mpi.h>
#include <cstdio>
#include <ctime>
#include <unistd.h>

#include <mpl/mpl.hpp>
#include <vector>


/**
A simple ping pong test that iterates many times to measure communication time
between 2 nodes HOW TO RUN: mpirun -n num_procs (2 in this example) a.out
nBytes(size of data in bytes) max_iter(how many times does it itarate?)
sleep_time(sleep time between iterations)
**/
using namespace std;

double Mean(double[], int);
double Median(double[], int);
void Print_times(double[], int);


int main(int argc, char **argv) {
  int myid, procs, n, err, max_iter, nBytes, sleep_time, iter = 0, range = 100,
                                                         pow_2;
  double t_start, t_end, t_start_inner;
  constexpr int SCALE = 1000000;


  // ------ PARAMETER SETUP -----------
  pow_2 = atoi(argv[1]);
  max_iter = atoi(argv[2]);
  // int num_restart = strtol(argv[3], NULL, 10);

  double mpi_time = 0.0;
  nBytes = std::pow(2, pow_2);
  n = nBytes;

  std::vector<char> myarr(32);
  std::vector<char> arr(32);


  const mpl::communicator &comm_world(mpl::environment::comm_world());
  mpl::contiguous_layout<char> l(32);
  if (comm_world.rank() == 0) {
    for (int j = 0; j < n; j++)
      arr[j] = 0;
  }

  MPI_Status status;

   int _succ, _prev;
    _succ = (comm_world.rank() + 1) % comm_world.size();
    _prev = comm_world.rank() == 0 ? (comm_world.size() - 1) : (comm_world.rank() - 1);

    mpl::irequest_pool events;

    // First iter
    auto r1{comm_world.isend(arr.data(),l, _prev)};
    auto r2{comm_world.isend(arr.data(),l, _succ)};
    auto r3{comm_world.irecv(arr.data(),l, _prev)};
    auto r4{comm_world.irecv(arr.data(),l, _succ)};

    r1.wait();
    r2.wait();
    r3.wait();
    r4.wait();
    comm_world.barrier();
    
    if (comm_world.rank() == 0)
        t_start = mpl::environment::wtime();

    while (iter < max_iter) {
      // std::cout << iter << "\n";
        r1 = comm_world.isend(arr.data(), l, _prev);
        r2 = comm_world.isend(arr.data(), l, _succ);
        r3 = comm_world.irecv(arr.data(), l, _prev);
        r4 = comm_world.irecv(arr.data(), l, _succ);

        r1.wait();
        r2.wait();
        r3.wait();
        r4.wait();
        // events.cancelall();
        iter++;
      }
      
    comm_world.barrier();
    if (comm_world.rank() == 0) {
      t_end = mpl::environment::wtime();
      mpi_time =
          (t_end - t_start) * SCALE;
    }

  comm_world.barrier();

  if (comm_world.rank() == 0) {
    // cout << "\nData Size: " << nBytes << " bytes\n";
    cout << mpi_time << "\n";
    // cout << "Mean of communication times: " << Mean(mpi_time, num_restart)
    //      << "\n";
    // cout << "Median of communication times: " << Median(mpi_time, num_restart)
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