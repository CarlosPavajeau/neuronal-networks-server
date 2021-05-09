#!/bin/bash
#
# Install all project dependencies
#

apt-get install -y cmake wget

# install Eigen
curl -L https://gitlab.com/libeigen/eigen/-/archive/3.3.9/eigen-3.3.9.tar.gz | tar -zxf -
cd eigen-3.3.9 && mkdir "build" && cd build && cmake ../ && make install && cd ..

# install Boost libraries
wget https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.bz2 | tar -zxf -
cd boost_1_76_0 && ./boostrap.sh --prefix=/usr/ && ./b2 && ./b2 install && cd ..
