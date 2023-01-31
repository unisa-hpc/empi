#!/bin/bash
SIZE=$1
ITER=$2
RESTART=$3
NUM_PROC=$4

MPIRUN="/root/opt/openmpi/bin/mpirun --allow-run-as-root -n $4"

function run_experiment {
	echo "------$1------"
	declare time_sum=0.0
	for((i=0;i<${RESTART};i+=1))
	do
		val=$(${MPIRUN} ../build/examples/$2 ${SIZE} ${ITER} | bc)
		time_sum=$(echo "$time_sum + $val" | bc) 
		pkill mpirun
	done
	echo Total time: $time_sum
	echo Mean: $(echo "$time_sum / $RESTART.0" | bc -l) 
	sleep 1
	echo "" # Spacing
}

echo "Config: size: $SIZE -- ITER: $ITER -- RESTART: $RESTART -- NUM_PROC: $NUM_PROC"

# run_experiment "Ping pong: MPI" "ping_pong/mpi_ping_pong"
# run_experiment "Ping pong: MPI (UNCHECKED)" "ping_pong/umpi_ping_pong"
# run_experiment "Ping pong: EMPI" "ping_pong/empi_ping_pong"
# run_experiment "Ping pong: MPL" "ping_pong/mpl_ping_pong"

# run_experiment "Bidirectional ring: MPI" "bdring/mpi_bdring"
# run_experiment "Bidirectional ring: MPI (UNCHECKED)" "bdring/umpi_bdring"
# run_experiment "Bidirectional ring: EMPI" "bdring/empi_bdring"ibcast
# run_experiment "Bidirectional ring: MPL" "bdring/mpl_bdring"

# run_experiment "Allreduce: MPI" "all_reduce/mpi_allreduce"
# run_experiment "Allreduce: MPI (UNCHECKED)" "all_reduce/umpi_allreduce"
# run_experiment "Allreduce: EMPI" "all_reduce/empi_allreduce"
# run_experiment "Allreduce: MPL" "all_reduce/mpl_allreduce"

run_experiment "Vibrating String: MPI" "vibrating_string/mpi_vibrating_string"
# run_experiment "Vibrating String: MPI (UNCHECKED)" "vibrating_string/umpi_vibrating_string"
run_experiment "Vibrating String: EMPI" "vibrating_string/empi_vibrating_string"
run_experiment "Vibrating String: MPL" "vibrating_string/mpl_vibrating_string"

# run_experiment "IBcast: MPI" "ibcast/mpi_ibcast"
# run_experiment "IBcast: MPI (UNCHECKED)" "ibcast/umpi_ibcast"
# run_experiment "IBcast: EMPI" "ibcast/empi_ibcast"
# run_experiment "IBcast: MPL" "ibcast/mpl_ibcast"

# run_experiment "Bcast: MPI" "bcast/mpi_bcast"
# run_experiment "Bcast: MPI (UNCHECKED)" "bcast/umpi_bcast"
# run_experiment "Bcast: EMPI" "bcast/empi_bcast"
# run_experiment "Bcast: MPL" "bcast/mpl_bcast"