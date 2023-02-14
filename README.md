# EMPI: Enhanced Message Passing Interface in Modern C++
EMPI is a C++ library that aims to enhance the MPI programming model's interface while providing competitive performance to OpenMPI. Its objective is to provide a more modern and straightforward interface to MPI while exploiting C++ language features to reduce programming errors and improve performance. It is developed using C++20, exploiting several modern C++ features. 

***CAVEAT: EMPI is highly experimental and its interface and features will probably change frequently***

## EMPI Requirements
EMPI is written in C++20, so a valid C++20 compiler (gcc 12 is recommended) alongside with Cmake version 3.0 are required.
EMPI is shipped with a customized OpenMPI implementation, which is required to exploit the best performance. However, EMPI works with a vanilla OpenMPI as well, in case you don't want to compile the customized OpenMPI on your own. 
To do so, disable the unchecked function in the CMake configuring phase when building yout application with the flag `-DENABLE_UNCHECKED_FUNCTION=OFF`.  
However, to benefit the maximum performance, it is suggested to compile with our customized OMPI.

## Building EMPI examples
```bash
cd empi
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$WORKSPACE/deps/openmpi/bin/mpicxx -DCMAKE_CXX_FLAGS='-O3 -ffast-math -march=native -I$WORKSPACE/deps/openmpi/include'
make -j 
```

### Building LULESH
LULESH is placed under `empi/benchmarks/LULESH`. It uses the CMake variable `-DWITH_MPI=[1|0]` to enable the MPI implementation, and a variable to enable EMPI `-DUSE_EMPI=[1|0]`.  
To build LULESH using EMPI, type:
```bash
cd $WORKSPACE/empi/benchmarks/LULESH
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$WORKSPACE/deps/openmpi/bin/mpicxx -DCMAKE_CXX_FLAGS='-O3 -ffast-math -march=native -I$WORKSPACE/deps/openmpi/include' -DWITH_MPI=1 -DUSE_EMPI=1 -DEMPI_PATH=$WORKSPACE/empi/include
make -j 
```

## Examples

The examples scripts are developed with the following dependencies:

- Python 3.8.10
	- Command
	- argparse

You can install the requirements with:
```bash
pip3 install -r empi/scripts/requirements.txt
```

Under `empi/scripts/` you will find 3 python scripts:
- `minibench.py`  
- `vibrating_string.py`  
- `lulesh.py`

`minibench.py`, `vibrating_string.py`, and `lulesh.py` launch the related applications and takes a number of command line arguments to customize each run.

They provide a series of command line parameteres to configure the benchmark's execution. Type `python [minibench.py | lulesh.py | vibrating_string.py] --help` to have a deeper look to the possible options.

For example, to run all the microbenchmarks with: 
- 4 processors `--num_proc=4`
- 4 bytes message size  `--size=2`
- 10 inner iterations per benchmark `--app_iter=10`
- 20 outer repetitions per benchmark (per mpirun: how many times do you want to run mpirun and report their average) `--app_restart=20`
- root permissions (if you are root)
- Default dependencies path  
```bash
python3 minibench.py --num_proc=4 --size=2 --app_iter=10 --app_restart=20 --root 
```






