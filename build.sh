#!/bin/bash
set -e

# 创建 build 目录
mkdir -p build
cd build

# 运行 CMake 生成 Makefile
cmake -DCMAKE_CXX_COMPILER=g++ ..

# 编译
make -j$(nproc)

echo "Build finished."
