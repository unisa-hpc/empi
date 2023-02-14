#!/usr/bin/env python3
"""
Copyright (c) 2023, Luigi Crisci
"""
import sys
import command
import argparse
import re
import multiprocessing

def base_args():
        # Make parser object
    p = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    
    p.add_argument("--ompi_path",
                   type=str,
                   default="../../deps/openmpi/",
                   help="mpirun path (default: ../../deps/openmpi/)")
    p.add_argument("--root",
                   action='store_true',
                   default=False,
                   help="enable mpi with root user",
                   )
    return p

def common_args(p):
    p.add_argument("--size",
                type=int,
                help="The number of bytes for the benchmarks in power-of-two (e.g. 4 -> 2^4 bytes). Note that in some benchmarks (vibrating string) this value is ignored (default: 2)",
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
    return p

def run_experiment(args,exp_name,run_command, parse_output):
    print(f"//==----------{exp_name}----------==//")
    print(f'--- size: {args.size} -- num_proc: {args.num_proc} ---')
    time_sum = 0.0
    newline="\n" #f-string limitation bypass
    for i in range(0,args.app_restart,1):
        print(f'{i+1}..{newline if i==(args.app_restart-1) else ""}',end='',flush=True)
        # print(run_command)
        res = command.run(run_command)
        time_sum += float(parse_output(res.output))
    print(f"Aggregated time: {time_sum}")
    print(f"Mean: {time_sum/args.app_restart}")
    print("-----------------------------------------------")
    
def make_minibench_command(args, exp_path):
	return [
		f'{args.ompi_path}bin/mpirun',
		f'--allow-run-as-root',
		f'{"--oversubscribe" if multiprocessing.cpu_count() < args.num_proc else "--nooversubscribe"}',
		'-n',
		f'{args.num_proc}',
		f'{args.bench_path}{exp_path}',  # App name
		f'{args.size}',
		f'{args.app_iter}'
]

def make_lulesh_command(args):
    return [
        f'{args.ompi_path}bin/mpirun',
	    '--allow-run-as-root',
	    f'{"--oversubscribe" if multiprocessing.cpu_count() < args.num_proc else "--nooversubscribe"}',
	    '-n',
	    f'{args.num_proc}',
        f'{args.lulesh_path}lulesh2.0',  # App name
        '-s',
        f'{args.size}',
        '-i',
        f'{args.app_iter}'
    ]

def parse_lulesh(param):
    output = str(param)
    pattern = r'[0-9]+\.[0-9]*'
    for line in output.split("\n"):
        if "Elapsed time" in line:
            match = re.search(pattern,line)
            if match:
                return match.group()    

def exp_full(args, exp_name, exp_path, op):
	num_procs = [2,4,8,16,32]
	sizes = [2,4,8,10,12,14,16]
	for num_proc in num_procs:
		for size in sizes:
			args.num_proc = num_proc
			args.size = size
			run_experiment(args, exp_name, make_minibench_command(args, exp_path),op)

def exp_fixed_proc(args, exp_name, exp_path, num_proc, op):
	sizes = [2,4,8,10,12,14,16]
	args.num_proc = num_proc
	for size in sizes:
		args.size = size
		run_experiment(args, exp_name, make_minibench_command(args, exp_path),op)

def exp_fixed_size(args, exp_name, exp_path, size, op):
	num_procs = [2,4,8,16,32]
	args.size = size
	for proc in num_procs:
		args.num_proc = proc
		run_experiment(args, exp_name, make_minibench_command(args, exp_path),op)

def exp_lulesh(args, exp_name, size, op):
	num_procs = [1, 8, 64, 216, 512, 729, 1000]
	args.size = 1
	for proc in num_procs:
		args.num_proc = proc
		run_experiment(args, exp_name, make_lulesh_command(args),op)
