cmake_minimum_required(VERSION 3.10)
project(gt_server)

set(target_name gt_server)

include_directories(
    ./include
)

file(GLOB_RECURSE SRC_FILES
    ./src/*.cpp
)

add_executable(${target_name} ${SRC_FILES})

target_link_libraries(${target_name}
    pthread
    boost_system
)