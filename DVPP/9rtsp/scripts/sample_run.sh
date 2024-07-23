#!/bin/bash
ScriptPath="$( cd "$(dirname "$BASH_SOURCE")" ; pwd -P )"

function main()
{
  if [ "$1" != "imshow" ] && [ "$1" != "stdout" ]; then
      echo "[ERROR] please choose output display mode: [bash sample_run imshow] [bash sample_run stdout]"
      return
  fi

  echo "[INFO] The sample starts to run"
  cd ${ScriptPath}/../out
  ./main $1
  if [ $? -ne 0 ];then
      echo "[INFO] The program runs failed"
  else
      echo "[INFO] The program runs successfully"
  fi
}
main $1
