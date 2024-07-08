# Copyright (c) Huawei Technologies Co., Ltd. 2019. All rights reserved.

# CMake lowest version requirement
cmake_minimum_required(VERSION 3.5.1)

# project information
project(venc)

# Compile options
add_compile_options(-std=c++11)

add_definitions(-DENABLE_DVPP_INTERFACE)

# Specify target generation path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "../out")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -O0 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -Wall")

if (NOT DEFINED ENV{ASCEND_TOOLKIT_HOME})
    message(FATAL_ERROR "Not Defined ASCEND_TOOLKIT_HOME")
endif()

# Header path
include_directories(
    $ENV{ASCEND_TOOLKIT_HOME}/runtime/include/
    $ENV{ASCEND_TOOLKIT_HOME}/acllib/include/
)

# add host lib path
link_directories(
    $ENV{ASCEND_TOOLKIT_HOME}/runtime/lib64/stub
    $ENV{ASCEND_TOOLKIT_HOME}/acllib/lib/
    $ENV{ASCEND_TOOLKIT_HOME}/runtime/lib64/
)

add_executable(main
        main.cpp)

# Link libraries
target_link_libraries(main ascendcl acl_dvpp stdc++ pthread opencv_highgui opencv_core opencv_imgproc opencv_imgcodecs opencv_calib3d opencv_features2d opencv_videoio)

install(TARGETS main DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
