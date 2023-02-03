#!/usr/bin/python3
import common
from common import run_experiment
from common import make_minibench_command


if __name__ == '__main__':
	args = common.cmdline_args()
	print(args)

	run_experiment(args, "Ping pong: MPI", make_minibench_command(args, "ping_pong/mpi_ping_pong"))
	
	run_experiment(args, "Ping_pong: EMPI", make_minibench_command(args, "ping_pong/empi_ping_pong"))
	run_experiment(args, "Ping_pong: MPL", make_minibench_command(args, "ping_pong/mpl_ping_pong"))

	run_experiment(args, "Bidirectional ring: MPI", make_minibench_command(args, "bdring/mpi_bdring"))
	run_experiment(args, "Bidirectional ring: EMPI", make_minibench_command(args, "bdring/empi_bdring"))
	run_experiment(args, "Bidirectional ring: MPL", make_minibench_command(args, "bdring/mpl_bdring"))

	run_experiment(args, "Allreduce: MPI", make_minibench_command(args, "all_reduce/mpi_allreduce"))
	run_experiment(args, "Allreduce: EMPI", make_minibench_command(args, "all_reduce/empi_allreduce"))
	run_experiment(args, "Allreduce: MPL", make_minibench_command(args, "all_reduce/mpl_allreduce"))

	run_experiment(args, "Vibrating String: MPI", make_minibench_command(args, "vibrating_string/mpi_vibrating_string"))
	run_experiment(args, "Vibrating String: EMPI", make_minibench_command(args, "vibrating_string/empi_vibrating_string"))
	run_experiment(args, "Vibrating String: MPL", make_minibench_command(args, "vibrating_string/mpl_vibrating_string"))

	run_experiment(args, "IBcast: MPI", make_minibench_command(args, "ibcast/mpi_ibcast"))
	run_experiment(args, "IBcast: EMPI", make_minibench_command(args, "ibcast/empi_ibcast"))
	run_experiment(args, "IBcast: MPL", make_minibench_command(args, "ibcast/mpl_ibcast"))

	run_experiment(args, "Bcast: MPI", make_minibench_command(args, "bcast/mpi_bcast"))
	run_experiment(args, "Bcast: EMPI", make_minibench_command(args, "bcast/empi_bcast"))
	run_experiment(args, "Bcast: MPL", make_minibench_command(args, "bcast/mpl_bcast"))
