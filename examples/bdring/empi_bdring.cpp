#include "empi/context.hpp"
#include "empi/message_grp_hdl.hpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <malloc.h>
#include <mpi.h>
#include <cstdio>
#include <ctime>
#include <unistd.h>

#include <empi/empi.hpp>
/**
A simple ping pong test that iterates many times to measure communication time
between 2 nodes HOW TO RUN: mpirun -n num_procs (2 in this example) a.out
nBytes(size of data in bytes) max_iter(how many times does it itarate?)
sleep_time(sleep time between iterations)
**/
using namespace std;

using value_type = char;

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

  pow_2_bytes = strtol(argv[1], nullptr, 10);
  n = static_cast<int>(std::pow(2, pow_2_bytes));
  max_iter = strtol(argv[2], nullptr, 10);

  std::vector<value_type> arr(n,0);

  MPI_Status status;
  auto ctx = empi::Context(&argc, &argv);
  auto message_group = ctx.create_message_group(MPI_COMM_WORLD);

  const volatile auto prev = ctx.prev();
  const volatile auto next = ctx.succ();

  message_group->run(
      [&](empi::MessageGroupHandler<char, empi::Tag{0}, empi::NOSIZE> &mgh) { 
          std::shared_ptr<async_event> events[4];
          // First iter
          events[0] = mgh.Irecv(arr, prev, n);
          events[1] = mgh.Irecv(arr, next, n);
          events[2] = mgh.Isend(arr, prev, n);
          events[3] = mgh.Isend(arr, next, n);
          #pragma unroll
          for(auto& req : events)
            req->wait();
          MPI_Barrier(MPI_COMM_WORLD);
          

          if (ctx.rank() == 0)
              t_start = MPI_Wtime();

          for (auto iter = 0; iter < max_iter; iter++) {
            // std::cout << iter << "\n";
              events[0] = mgh.Irecv(arr, prev, n);
              events[1] = mgh.Irecv(arr, next, n);
              events[2] = mgh.Isend(arr, prev, n);
              events[3] = mgh.Isend(arr, next, n);
              
              #pragma unroll
              for(auto& req : events)
                req->wait();
                
            }
            
          message_group->barrier();
          if (ctx.rank() == 0) {
            t_end = MPI_Wtime();
            mpi_time =
                (t_end - t_start) * SCALE;
          }
      });

  message_group->barrier();

  if (ctx.rank() == 0) {
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