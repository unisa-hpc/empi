M4_PATH=$PWD/m4
AUTOCONF_PATH=$PWD/autoconf
AUTOMAKE_PATH=$PWD/automake
LIBTOOL_PATH=$PWD/libtool
FLEX_PATH=$PWD/flex
PREFIX=$PWD/deps
SOURCES=$PWD/sources

OMPI_PATH=$PWD/ompi-4.1
OMPI_PREFIX=$PWD/openmpi

PATH=$PATH:$PWD/$PREFIX/bin #Setting install folder
echo $PATH

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

cd $FLEX_PATH
./autogen.pl
./configure --prefix=$PREFIX;
make -j install;
cd ..

cd $OMPI_PATH
./autogen.pl
./configure --prefix=$OMPI_PREFIX
make all install
