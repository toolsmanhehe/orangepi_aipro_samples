# Copyright (c) Huawei Technologies Co., Ltd. 2019. All rights reserved.

# CMake lowest version requirement
cmake_minimum_required(VERSION 3.5.1)

# project information
project(ACL_DVPP_RESNET50)

# Compile options
add_compile_options(-std=c++11)

add_definitions(-DENABLE_DVPP_INTERFACE)

# Specify target generation path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "../../../out")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -O0 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -Wall")

set(INC_PATH $ENV{DDK_PATH})

if (NOT DEFINED ENV{DDK_PATH})
    set(INC_PATH "/usr/local/Ascend/ascend-toolkit/latest/")
    message(STATUS "set default INC_PATH: ${INC_PATH}")
else ()
    message(STATUS "env INC_PATH: ${INC_PATH}")
endif()

set(LIB_PATH "$ENV{NPU_HOST_LIB}")

# Dynamic libraries in the stub directory can only be used for compilation
if (NOT DEFINED "ENV{NPU_HOST_LIB}")
    set(LIB_PATH "/usr/local/Ascend/ascend-toolkit/latest/runtime/lib64/stub/")
    message(STATUS "set default LIB_PATH: ${LIB_PATH}")
else ()
    message(STATUS "env LIB_PATH: ${LIB_PATH}")
endif()

# Header path
include_directories(
    ${INC_PATH}/runtime/include/
    ../inc/
    $ENV{ASCEND_TOOLKIT_HOME}/acllib/include/
)

# add host lib path
link_directories(
    ${LIB_PATH}
    $ENV{ASCEND_TOOLKIT_HOME}/acllib/lib/
)

add_executable(main
        utils.cpp
        dvpp_process.cpp
        #model_process.cpp
        sample_process.cpp
        main.cpp)

target_link_libraries(main
    ascendcl acl_dvpp stdc++)

install(TARGETS main DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})