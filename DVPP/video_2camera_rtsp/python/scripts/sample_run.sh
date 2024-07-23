#!/bin/bash
ScriptPath="$( cd "$(dirname "$BASH_SOURCE")" ; pwd -P )"

echo "[INFO] The sample starts to run"
running_command="python3 YOLOV5MultiInput.py"
cd ${ScriptPath}/../src
${running_command}
if [ $? -ne 0 ];then
    echo "[INFO] The program runs failed"
else
    echo "[INFO] The program runs successfully"
fi
