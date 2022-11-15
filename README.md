# EMPI: Enhanced Message Passing Interface in Modern C++
Enhanced Message Passing Interface in Modern C++

## TODO:
- [ ] Pre-allocated circular queue for request pool to reduce insert overhead
- [ ] Refactor memory ownership of request_pool


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
