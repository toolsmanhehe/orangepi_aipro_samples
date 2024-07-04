#!/bin/bash
if [ -n "$ASCEND_INSTALL_PATH" ]; then
    _ASCEND_INSTALL_PATH=$ASCEND_INSTALL_PATH
elif [ -n "$ASCEND_HOME_PATH" ]; then
    _ASCEND_INSTALL_PATH=$ASCEND_HOME_PATH
else
    if [ -d "$HOME/Ascend/ascend-toolkit/latest" ]; then
        _ASCEND_INSTALL_PATH=$HOME/Ascend/ascend-toolkit/latest
    else
        _ASCEND_INSTALL_PATH=/usr/local/Ascend/ascend-toolkit/latest
    fi
fi
source $_ASCEND_INSTALL_PATH/bin/setenv.bash

set -e
pip3 install pyyaml wheel setuptools Ninja expecttest
export LD_LIBRARY_PATH=$ASCEND_OPP_PATH/vendors/customize/op_api/lib:$LD_LIBRARY_PATH
(
    cd test
    python3 test_add_custom.py
)
