// solve the time-dependent one-dimensional wave equation
// via a finite difference discretization and explicit time stepping

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <mpl/mpl.hpp>
#include <vector>

const int N = 1001;       // total number of grid points
const double L = 1;       // lengths of domain
const double c = 1;       // speed of sound
const double dt = 0.001;  // temporal step width
const double t_end = 2.4; // simulation time

enum : int { left_copy, right_copy };

// update grid points
void string(const std::vector<double> &u, const std::vector<double> &u_old, std::vector<double> &u_new, double eps) {
    using size_type = std::vector<double>::size_type;
    size_type N = u.size();
    u_new[0] = u[0];
    for(size_type i = 1; i < N - 1; ++i) u_new[i] = eps * (u[i - 1] + u[i + 1]) + 2.0 * (1.0 - eps) * u[i] - u_old[i];
    u_new[N - 1] = u[N - 1];
}

// initial elongation of string
inline double u_0(double x) {
    if(x <= 0 or x >= L) return 0;
    return std::exp(-200.0 * (x - 0.5 * L) * (x - 0.5 * L));
}

// initial velocity of string
inline double u_0_dt(double x) { return 0.0; }

void f(const mpl::communicator &comm_world) {
    double dx = L / (N - 1); // grid spacing
    double eps = dt * dt * c * c / (dx * dx);
    int C_size = comm_world.size();
    int C_rank = comm_world.rank();
    std::vector<int> N_l, N0_l;
    for(int i = 0; i < C_size; ++i) {
        // number of local grid points of process i
        N_l.push_back((i + 1) * (N - 2) / C_size - i * (N - 2) / C_size + 2);
        // position of local grid of process i within the global grid
        N0_l.push_back(i * (N - 2) / C_size);
    }
    // grid data for times (t-dt), t and t+dt
    std::vector<double> u_old_l(N_l[C_rank]);
    std::vector<double> u_l(N_l[C_rank]);
    std::vector<double> u_new_l(N_l[C_rank]);
    // 1st propagation step uses current elongation and velocity
    // calculate all grid points including overlapping border data
    for(int i = 0; i < N_l[C_rank]; ++i) {
        double x = (i + N0_l[C_rank]) * dx;
        u_old_l[i] = u_0(x);
        u_l[i] = 0.5 * eps * (u_0(x - dx) + u_0(x + dx)) + (1.0 - eps) * u_0(x) + dt * u_0_dt(x);
    }

    // propagate
    for(double t = 2 * dt; t <= t_end; t += dt) {
        // make one time step to get elongation
        string(u_l, u_old_l, u_new_l, eps);
        // update border data
        mpl::irequest_pool r;
        r.push(
            comm_world.isend(u_new_l[N_l[C_rank] - 2], C_rank + 1 < C_size ? C_rank + 1 : mpl::proc_null, right_copy));
        r.push(comm_world.isend(u_new_l[1], C_rank - 1 >= 0 ? C_rank - 1 : mpl::proc_null, left_copy));
        r.push(comm_world.irecv(u_new_l[0], C_rank - 1 >= 0 ? C_rank - 1 : mpl::proc_null, right_copy));
        r.push(
            comm_world.irecv(u_new_l[N_l[C_rank] - 1], C_rank + 1 < C_size ? C_rank + 1 : mpl::proc_null, left_copy));
        r.waitall();

        std::swap(u_l, u_old_l);
        std::swap(u_new_l, u_l);
    }

    // finally gather all the data at rank 0 and print result
    mpl::layouts<double> layouts;
    for(int i = 0; i < C_size; ++i) layouts.push_back(mpl::indexed_layout<double>({{N_l[i] - 2, N0_l[i] + 1}}));

    mpl::contiguous_layout<double> layout(N_l[C_rank] - 2);
    if(C_rank == 0) {
        std::vector<double> u(N, 0);
        comm_world.gatherv(0, u_l.data() + 1, layout, u.data(), layouts);
        // for (int i = 0; i < N; ++i)
        //   std::cout << dx * i << '\t' << u[i] << '\n';
    } else
        comm_world.gatherv(0, u_l.data() + 1, layout);
}

int main(int argc, char **argv) {
    int myid, procs, n, err, max_iter, nBytes, sleep_time, iter = 0, range = 100, pow_2;
    const mpl::communicator &comm_world(mpl::environment::comm_world());
    double t_start, t_end, t_start_inner;
    constexpr int SCALE = 1000000;

    // ------ PARAMETER SETUP -----------
    pow_2 = atoi(argv[1]);
    max_iter = atoi(argv[2]);
    // int num_restart = strtol(argv[3], NULL, 10);

    double mpi_time = 0.0;
    nBytes = std::pow(2, pow_2);
    n = nBytes;

    std::vector<char> myarr(n, 0);

    {
        const int rank = comm_world.rank();
        // Warmup
        f(comm_world);
        if(comm_world.rank() == 0) t_start = MPI_Wtime();

        for(size_t i = 0; i < max_iter; i++) { f(comm_world); }

        comm_world.barrier();
        if(comm_world.rank() == 0) {
            t_end = MPI_Wtime();
            mpi_time = (t_end - t_start) * SCALE;
        }

        comm_world.barrier();
    }

    if(comm_world.rank() == 0) { std::cout << mpi_time << "\n"; }

    return EXIT_SUCCESS;
}