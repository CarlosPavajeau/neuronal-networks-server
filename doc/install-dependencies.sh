#!/bin/bash
#
# Install all project dependencies
#

apt-get install -y cmake

# install Eigen
cd /usr/ && curl -L https://gitlab.com/libeigen/eigen/-/archive/3.3.9/eigen-3.3.9.tar.gz | tar -zxf -
cd eigen-3.3.9 && mkdir "build" && cd build && cmake ../ && make install

# install Boost libraries
cd /usr/ && curl -L https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.gz | tar -zxf -
cd boost_1_76_0 && ./bootstrap.sh --prefix=/usr/ && ./b2 && ./b2 install
