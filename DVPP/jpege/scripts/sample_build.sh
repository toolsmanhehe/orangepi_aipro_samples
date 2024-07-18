#!/bin/bash
ScriptPath="$( cd "$(dirname "$BASH_SOURCE")" ; pwd -P )"

function data_prepare()
{
  cd ${ScriptPath}/../
  if [ ! -f "./data/dvpp_input.yuv" ];then
    echo "[INFO] Start data prepare"
    wget -O ./data/dvpp_input.yuv https://obs-9be7.obs.cn-east-2.myhuaweicloud.com/models/aclsample/dvpp_vpc_1920x1080_nv12.yuv --no-check-certificate
  fi
}

function build()
{
  if [ -d ${ScriptPath}/../out ];then
    rm -rf ${ScriptPath}/../out
  fi

  if [ -d ${ScriptPath}/../build ];then
    rm -rf ${ScriptPath}/../build
  fi

  mkdir -p ${ScriptPath}/../build
  cd ${ScriptPath}/../build

  cmake ../src -DCMAKE_CXX_COMPILER=g++ -DCMAKE_SKIP_RPATH=TRUE
  if [ $? -ne 0 ];then
    echo "[ERROR] cmake error, Please check your environment!"
    return 1
  fi
  make
  if [ $? -ne 0 ];then
    echo "[ERROR] build failed, Please check your environment!"
    return 1
  fi
  cd - > /dev/null
}

function main()
{
  data_prepare
  if [ $? -ne 0 ];then
    echo "[INFO] Data prepared failed!"
    return 1
  fi
  echo "[INFO] Data is complete"

  echo "[INFO] Sample preparation"
  build
  if [ $? -ne 0 ];then
    return 1
  fi
  echo "[INFO] Sample preparation is complete"
}
main
