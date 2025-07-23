cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gt_webserver)

# 添加头文件
include_directories(./commondefine)

add_subdirectory(gt_server)
add_subdirectory(gt_client)