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
TENSORFLOW_VERSION=$(python3 -c "import tensorflow as tf; print(tf.__version__)")

if [[ $TENSORFLOW_VERSION =~ ^1\..* ]]; then
    python3 run_add_custom.py
elif [[ $TENSORFLOW_VERSION =~ ^2\..* ]]; then
    python3 run_add_custom_tf2.py
else
    echo "unknown version $TENSORFLOW_VERSION, or tensorflow not installed"
fi
