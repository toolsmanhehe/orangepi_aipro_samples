#!/bin/bash
ScriptPath="$( cd "$(dirname "$BASH_SOURCE")" ; pwd -P )"

function main()
{
    echo "[INFO] The sample starts to run"

    cd ${ScriptPath}/../out
    ./main ../data/dvpp_output.yuv 1024 688

    if [ $? -ne 0 ];then
        echo "[INFO] The program runs failed"
    else
        echo "[INFO] The program runs successfully"
    fi
}
main
