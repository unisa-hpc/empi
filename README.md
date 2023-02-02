# EMPI: Enhanced Message Passing Interface in Modern C++
EMPI is a C++ library that aims to enhance the MPI programming model's interface while providing competitive performance. Its objective is to provide a more modern and straightforward interface to MPI while exploiting C++ language features to reduce programming errors and improve performance. It is developed using C++20, exploiting several modern C++ features. 

## Requirements
EMPI is written in C++20, so a valid C++20 compiler is required (e.g. gcc 12). The building system is Cmake, with version 3.0 as the minimun requirement.
EMPI is shipped with a customized OpenMPI implementation, which is required to exploit the best performance. However, EMPI works with a vanilla OpenMPI as well in case you don't want to compile OpenMPI on your own. To do that, disable the unchecked function in the CMake configuring phase with the flag `-DENABLE_UNCHECKED_FUNCTION=OFF`.



## How to Compile and run:
```
cd buid
cmake .. -DCMAKE_CXX_COMPILER=/root/opt/openmpi/bin/mpicxx -DCMAKE_CXX_FLAGS='-O3 -g -ffast-math -I/root/opt/openmpi/include'
make -j 
cd examples/[your_example_directory]
mpirun .... 
```
### Compile mpl subset:
```
cd lib\mpl-subset
mkdir build
cd build
cmake .. 
make -j install
```
