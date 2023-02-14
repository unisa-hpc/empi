#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <malloc.h>
#include <mpi.h>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <vector>

using namespace std;
using value_type = char;

double Mean(double[], int);
double Median(double[], int);
void Print_times(double[], int);

int main(int argc, char **argv) {
  double t_start, t_end;
  double mpi_time = 0.0;
  constexpr int SCALE = 1000000;
  int _rank, _size, _succ, _prev;
  int err;
  long pow_2_bytes;
  int n;
  long max_iter;

  MPI_Status status;
  err = MPI_Init(&argc, &argv);

  if (err != MPI_SUCCESS) {
    cout << "\nError initializing MPI.\n";
    MPI_Abort(MPI_COMM_WORLD, err);
  }

  pow_2_bytes = strtol(argv[1], nullptr, 10);
  n = static_cast<int>(std::pow(2, pow_2_bytes));
  max_iter = strtol(argv[2], nullptr, 10);

  std::vector<value_type> arr(n,0);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &_size);

  _succ = (_rank + 1) % _size;
  _prev = _rank == 0 ? (_size - 1) : (_rank - 1);

  {
    MPI_Request requets[4];
    // Warmup
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_IUrecv(arr.data(), n, MPI_CHAR, _prev, MPI_ANY_TAG, MPI_COMM_WORLD,  &requets[0]);
    MPI_IUrecv(arr.data(), n, MPI_CHAR, _succ, MPI_ANY_TAG, MPI_COMM_WORLD, &requets[1]);
    MPI_IUsend(arr.data(), n, MPI_CHAR, _prev, 0, MPI_COMM_WORLD, &requets[2]);
    MPI_IUsend(arr.data(), n, MPI_CHAR, _succ, 1, MPI_COMM_WORLD, &requets[3]);

    #pragma unroll
    for(auto& requet : requets){
        MPI_Wait(&requet, &status);
    }
  }
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (_rank == 0)
      t_start = MPI_Wtime();

    for(auto iter = 0; iter < max_iter; iter++){
        MPI_Request requets[4];
        MPI_IUrecv(arr.data(), n, MPI_CHAR, _prev, MPI_ANY_TAG, MPI_COMM_WORLD,  &requets[0]);
        MPI_IUrecv(arr.data(), n, MPI_CHAR, _succ, MPI_ANY_TAG, MPI_COMM_WORLD, &requets[1]);
        MPI_IUsend(arr.data(), n, MPI_CHAR, _prev, 0, MPI_COMM_WORLD, &requets[2]);
        MPI_IUsend(arr.data(), n, MPI_CHAR, _succ, 1, MPI_COMM_WORLD, &requets[3]);

        #pragma unroll
        for(auto& requet : requets)
          MPI_Wait(&requet, &status);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (_rank == 0) {
      t_end = MPI_Wtime();
      mpi_time = (t_end - t_start) * SCALE;
    }
  
  MPI_Barrier(MPI_COMM_WORLD);
  
  if (_rank == 0) {
    // cout << "\nData Size: " << nBytes << " bytes\n";
    cout << mpi_time << "\n";
    // cout << "Mean of communication times: " << Mean(mpi_time , num_restart)
    //      << "\n";
    // cout << "Median of communication times: " << Median(mpi_time , num_restart )
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