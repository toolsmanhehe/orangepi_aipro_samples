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

rm -rf dist build
python3 setup.py build bdist_wheel
(
    cd dist
    pip3 install custom_ops-*.whl
)
(
    cd graph
    mkdir build
    cmake -B build
    cmake --build build -t generate_ge_raw_custom_ops -j
    mv auto_generated_ge_raw_custom_ops.py ../test
)
(
    cd test
    python3 test_add_custom_graph.py
)
