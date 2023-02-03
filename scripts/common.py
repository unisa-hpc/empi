#!/usr/bin/env python3
"""
Copyright (c) 2023, Luigi Crisci
"""
import sys
import command
import argparse

def cmdline_args():
        # Make parser object
    p = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    
    p.add_argument("--size",
                   type=int,
                   help="The number of bytes for the benchmarks in power-of-two (e.g. 4 -> 2^4 bytes) (default: 2)",
                   required=True)
    p.add_argument("--app_iter", 
                   type=int,
                   help="Number of times the body of the app is repeated. This increases the number of communication in the app (default: 10)",
                   default=10)
    p.add_argument("--app_restart", 
                   type=int,
                   help="Number of times the app is executed before collecting the mean execution time (default: 10)",
                   default=10)
    p.add_argument("--num_proc",
                   type=int,
                   default=2,
                   help="Number of MPI processors. Note that in some benchmarks (ping_pong) this value is ignored due to the app structure")
    p.add_argument("--ompi_path",
                   type=str,
                   default="/opt/openmpi/",
                   help="mpirun path (default: /opt/openmpi/)")
    p.add_argument("--root",
                   action='store_true',
                   default=False,
                   help="enable mpi with root user",
                   )
    p.add_argument("--bench_path",
                 type=str,
                 default="../build/benchmarks/",
                 help="Path to the benchmark folder (default: ../build/benchmarks)"
                 )
    return(p.parse_args())


def run_experiment(args,exp_name,run_command):
    print(f"//==----------{exp_name}----------==//")
    time_sum = 0.0
    newline="\n" #f-string limitation bypass
    for i in range(0,args.app_restart,1):
        print(f'{i+1}..{newline if i==(args.app_restart-1) else ""}',end='',flush=True)
        res = command.run(run_command)
        time_sum += float(res.output)
    print(f"Aggregated time: {time_sum}")
    print(f"Mean: {time_sum/args.app_restart}")
    command.run(["sleep","1"]) #Wait
    print("-----------------------------------------------")
    
def make_minibench_command(args, exp_path):
	return [
		f'{args.ompi_path}bin/mpirun',
		f'{"--allow-run-as-root" if args.root else ""}',
		'-n',
		f'{args.num_proc}',
		f'{args.bench_path}{exp_path}',  # App name
		f'{args.size}',
		f'{args.app_iter}'
]

def make_lulesh_command(args, exp_path):
    