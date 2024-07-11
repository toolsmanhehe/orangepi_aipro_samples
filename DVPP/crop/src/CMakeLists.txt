# Copyright (c) Huawei Technologies Co., Ltd. 2019. All rights reserved.

# CMake lowest version requirement
cmake_minimum_required(VERSION 3.5.1)

# project information
project(CROP)

# Compile options
add_compile_options(-std=c++11)

add_definitions(-DENABLE_DVPP_INTERFACE)

# Specify target generation path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "../out")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -O0 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -Wall")
set(LIBRARY_DIR "$ENV{ASCEND_TOOLKIT_HOME}/aarch64-linux/lib64/")

if (NOT DEFINED ENV{INSTALL_DIR})
    set(INSTALL_DIR "/usr/local/Ascend/ascend-toolkit/latest/")
    message(STATUS "set default INSTALL_DIR: ${INSTALL_DIR}")
else ()
    message(STATUS "env INSTALL_DIR: ${INSTALL_DIR}")
endif()

# Header path
include_directories(
    $ENV{INSTALL_DIR}/runtime/include/
    ./
    $ENV{ASCEND_TOOLKIT_HOME}/acllib/include/
)

# add host lib path
link_directories(
    $ENV{INSTALL_DIR}/runtime/lib64/stub
    $ENV{ASCEND_TOOLKIT_HOME}/acllib/lib/
    $ENV{ASCEND_TOOLKIT_HOME}/runtime/lib64/
)

add_executable(main
        main.cpp)

target_link_libraries(main
        ascendcl acl_dvpp stdc++)

install(TARGETS main DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
