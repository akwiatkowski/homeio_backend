export HOMEIO_SITE=leak_test
[ -d build ] || mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=/usr/bin/g++ ..
make
