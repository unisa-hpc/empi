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
	
	run_experiment(args, "Vibrating String: MPI", make_minibench_command(args, "vibrating_string/mpi_vibrating_string"),noop)
	run_experiment(args, "Vibrating String: EMPI", make_minibench_command(args, "vibrating_string/empi_vibrating_string"),noop)
