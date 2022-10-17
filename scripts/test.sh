#!/bin/bash
SIZE=$1
ITER=$2
RESTART=$3
NUM_PROC=$4

MPIRUN="/root/opt/openmpi/bin/mpirun --allow-run-as-root -n $4"

function run_experiment {
	echo "------$1------"
	declare mean=0.0
	for((i=0;i<${RESTART};i+=1))
	do
		val=$(${MPIRUN} ../build/examples/$2 ${SIZE} ${ITER} | bc)
		mean=$(echo "$mean + $val" | bc) 
		pkill mpirun
	done
	echo Total time: $mean
	echo Time: $(echo "$mean / $RESTART.0" | bc -l) 
	sleep 1
	echo "" # Spacing
}

echo "Config: size: $SIZE -- ITER: $ITER -- RESTART: $RESTART -- NUM_PROC: $NUM_PROC"

# run_experiment "MPI" "mpi_test_send_recv"
# run_experiment "MPI (UNCHECKED)" "mpi_test_usend_urecv"
# run_experiment "EMPI" "empi_test_send_recv"
# run_experiment "MPI (ASYNC)" "mpi_test_isend_irecv"
# run_experiment "MPI (UNCHECKED ASYNC)" "mpi_test_iusend_iurecv"
# run_experiment "EMPI (ASYNC)" "empi_test_isend_irecv"
run_experiment "MPI (ASYNC)" "mpi_bdring_isend_irecv"
run_experiment "MPI (UNCHECKED ASYNC)" "mpi_bdring_iusend_iurecv"
run_experiment "EMPI (ASYNC)" "empi_bdring_isend_irecv"
run_experiment "MPL (ASYNC)" "mpl_bdring_isend_irecv"