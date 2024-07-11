#!/bin/bash
ScriptPath="$( cd "$(dirname "$BASH_SOURCE")" ; pwd -P )"

function main()
{
    echo "[INFO] The sample starts to run"

    cd ${ScriptPath}/../out
    ./main ../data/wood_rabbit_1024_1068_nv12.yuv 1024 1068 ./output/output.yuv 350 280 224 224

    if [ $? -ne 0 ];then
        echo "[INFO] The program runs failed"
    else
        echo "[INFO] The program runs successfully"
    fi
}
main
