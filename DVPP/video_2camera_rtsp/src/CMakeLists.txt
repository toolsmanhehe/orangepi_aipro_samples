# Copyright (c) Huawei Technologies Co., Ltd. 2019. All rights reserved.

cmake_minimum_required(VERSION 3.5.1)

project(sampleUsbCamera)

add_compile_options(-std=c++11)

add_definitions(-DENABLE_DVPP_INTERFACE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "../../../out")
set(CMAKE_CXX_FLAGS_DEBUG "-fPIC -O0 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -Wall")

set(INC_PATH $ENV{DDK_PATH})
if (NOT DEFINED ENV{DDK_PATH})
    set(INC_PATH "/usr/local/Ascend/ascend-toolkit/latest")
    message(STATUS "set default INC_PATH: ${INC_PATH}")
else()
    message(STATUS "set INC_PATH: ${INC_PATH}")
endif ()

set(LIB_PATH $ENV{NPU_HOST_LIB})
if (NOT DEFINED ENV{NPU_HOST_LIB})
    set(LIB_PATH "/usr/local/Ascend/ascend-toolkit/latest/runtime/lib64/stub")
    message(STATUS "set default LIB_PATH: ${LIB_PATH}")
else()
    message(STATUS "set LIB_PATH: ${LIB_PATH}")
endif ()

find_package(OpenCV REQUIRED)
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

include_directories(
   ${OpenCV_INCLUDE_DIRS}
   ${AVCODEC_INCLUDE_DIR}
   ${INC_PATH}/runtime/include/
   ./
)

link_directories(
    ${OpenCV_LIB_DIRS}
    ${AVCODEC_LIBRARY}
    ${LIB_PATH}
)

add_executable(main
               main.cpp)

if(target STREQUAL "Simulator_Function")
    target_link_libraries(main funcsim)
else()
    target_link_libraries(main ascendcl acl_dvpp stdc++ dl rt pthread acllite_dvpp_lite acllite_media acllite_om_execute acllite_common ${AVCODEC_LIBRARY} ${OpenCV_LIBS})
endif()

install(TARGETS main DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
