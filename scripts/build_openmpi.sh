M4_PATH=??
AUTOCONF_PATH=??
AUTOMAKE_PATH=??
LIBTOOL_PATH=??
PREFIX=$PWD/deps

OMPI_PATH=??
OMPI_PREFIX=$PWD/openmpi

PATH=$PATH:$PWD/$PREFIX/bin #Setting install folder

cd $M4_PATH;
./configure --prefix=$PREFIX;
make -j install;
cd ..

cd $AUTOCONF_PATH
./configure --prefix=$PREFIX;
make -j install;
cd ..

cd $AUTOMAKE_PATH
./configure --prefix=$PREFIX;
make -j install;
cd ..

cd $LIBTOOL_PATH
./configure --prefix=$PREFIX;
make -j install;
cd ..

cd $OMPI_PATH
./autogen.pl
./configure --prefix=$OMPI_PREFIX
make all install
