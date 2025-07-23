cmake_minimum_required(VERSION 3.10)
project(gt_client)

set(target_name gt_client)

file(GLOB_RECURSE SRC_FILES
    ./*.cpp
)

add_executable(${target_name} ${SRC_FILES})

target_link_libraries(${target_name}
    pthread
    boost_system
)