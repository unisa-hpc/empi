#!/usr/bin/python3
import common
from common import run_experiment
from common import make_minibench_command

if __name__ == '__main__':
	p = common.base_args()
	p = common.common_args(p)
	p.add_argument("--bench_path",
                 type=str,
                 default="../build/examples/",
                 help="Path to the benchmark folder (default: ../build/examples)"
                 )
	args = p.parse_args()
	noop= lambda x: x
 
	tmp = args.num_proc
	args.num_proc = 2
	run_experiment(args, "Ping pong: MPI", make_minibench_command(args, "ping_pong/mpi_ping_pong"),noop)
	run_experiment(args, "Ping_pong: EMPI", make_minibench_command(args, "ping_pong/empi_ping_pong"),noop)
	args.num_proc = tmp

	run_experiment(args, "Bidirectional ring: MPI", make_minibench_command(args, "bdring/mpi_bdring"),noop)
	run_experiment(args, "Bidirectional ring: EMPI", make_minibench_command(args, "bdring/empi_bdring"),noop)

	run_experiment(args, "Allreduce: MPI", make_minibench_command(args, "all_reduce/mpi_allreduce"),noop)
	run_experiment(args, "Allreduce: EMPI", make_minibench_command(args, "all_reduce/empi_allreduce"),noop)

	run_experiment(args, "IBcast: MPI", make_minibench_command(args, "ibcast/mpi_ibcast"),noop)
	run_experiment(args, "IBcast: EMPI", make_minibench_command(args, "ibcast/empi_ibcast"),noop)

	run_experiment(args, "Bcast: MPI", make_minibench_command(args, "bcast/mpi_bcast"),noop)
	run_experiment(args, "Bcast: EMPI", make_minibench_command(args, "bcast/empi_bcast"),noop)