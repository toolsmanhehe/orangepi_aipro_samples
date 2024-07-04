## 算子开发样例
|  目录名称                                                   |  功能描述                                              |  运行环境 |
| ------------------------------------------------------------ | ---------------------------------------------------- | -- |
| [AddCustomSample](./AddCustomSample) | 基于Ascend C的Add自定义Vector算子及调用样例 | Atlas训练系列产品<br>Atlas 200/500 A2 推理产品<br>Atlas推理系列产品(Ascend 310P 处理器)<br>Atlas A2训练系列产品 |
| [HelloWorldSample](./HelloWorldSample) | 基于Ascend C的自定义算子调用结构演示样例 |Atlas推理系列产品(Ascend 310P 处理器)<br>Atlas A2训练系列产品 |
| [LeakyReluCustomSample](./LeakyReluCustomSample) | 基于AscendC的LeakyReLU自定义Vector算子及调用样例 |Atlas推理系列产品(Ascend 310P 处理器)<br>Atlas 200/500 A2 推理产品<br>Atlas A2训练系列产品|
| [MatMulCustomSample](./MatMulCustomSample) | 基于AscendC的Matmul自定义Cube算子及调用样例 | Atlas推理系列产品(Ascend 310P 处理器)<br>Atlas A2训练系列产品 |
| [MatMulLeakyReluCustomSample](./MatMulLeakyReluCustomSample) | 基于AscendC的MatMulLeakyRelu自定义Cube+Cube算子及调用样例 | Atlas推理系列产品(Ascend 310P 处理器)<br>Atlas A2训练系列产品 |
| [SubCustomSample](./SubCustomSample) | 基于AscendC的Sub自定义算子及调用样例 |Atlas训练系列产品<br>Atlas推理系列产品(Ascend 310P 处理器)<br>Atlas A2训练系列产品 |

## 算子运行环境
以下为算子运行前的环境初步准备，同时提供了Python3.9和Python3.7的环境信息供用户参考。
### 操作系统
查看操作系统

```
lsb_release -a
```
输出信息

```
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 18.04.3 LTS
Release:        18.04
Codename:       bionic
```
### Python3.9的环境
查看编译环境

```bash
python3 --version; cmake --version; g++ --version
```

输出信息

```log
Python 3.9.12
cmake version 3.29.3
g++ (Ubuntu/Linaro 7.5.0-3ubuntu1~18.04) 7.5.0
```

安装python三方模块并查看

```bash
pip3 install torch==2.1.0 torch-npu==2.1.0 tensorflow numpy decorator sympy cffi protobuf attrs pyyaml pathlib2 scipy requests psutil absl-py easyprocess wheel expecttest hypothesis cloudpickle decorator tornado synr==0.5.0 packaging cmake setuptools Ninja pybind11
pip3 list
```

输出信息

```log
Package                      Version
---------------------------- -----------
absl-py                      2.1.0
astunparse                   1.6.3
attrs                        23.2.0
certifi                      2024.6.2
cffi                         1.16.0
charset-normalizer           3.3.2
cloudpickle                  3.0.0
cmake                        3.29.3
decorator                    5.1.1
EasyProcess                  1.1
exceptiongroup               1.2.1
expecttest                   0.2.1
filelock                     3.14.0
flatbuffers                  24.3.25
fsspec                       2024.6.0
gast                         0.5.4
google-pasta                 0.2.0
grpcio                       1.64.1
h5py                         3.11.0
hypothesis                   6.103.1
idna                         3.7
importlib_metadata           7.1.0
Jinja2                       3.1.4
keras                        3.3.3
libclang                     18.1.1
Markdown                     3.6
markdown-it-py               3.0.0
MarkupSafe                   2.1.5
mdurl                        0.1.2
ml-dtypes                    0.3.2
mpmath                       1.3.0
namex                        0.0.8
networkx                     3.2.1
ninja                        1.11.1.1
numpy                        1.26.4
opt-einsum                   3.3.0
optree                       0.11.0
packaging                    24.0
pathlib2                     2.3.7.post1
pip                          24.0
protobuf                     4.25.3
psutil                       5.9.8
pybind11                     2.12.0
pycparser                    2.22
Pygments                     2.18.0
PyYAML                       6.0.1
requests                     2.32.3
rich                         13.7.1
scipy                        1.13.1
setuptools                   69.5.1
six                          1.16.0
sortedcontainers             2.4.0
sympy                        1.12.1
synr                         0.5.0
tensorboard                  2.16.2
tensorboard-data-server      0.7.2
tensorflow                   2.16.1
tensorflow-io-gcs-filesystem 0.37.0
termcolor                    2.4.0
torch                        2.1.0
torch-npu                    2.1.0
tornado                      6.4.1
typing_extensions            4.12.2
urllib3                      2.2.1
Werkzeug                     3.0.3
wheel                        0.43.0
wrapt                        1.16.0
zipp                         3.19.2
```
由上方信息得知，pytorch和torch-npu版本均为2.1.0，tensorflow版本为2.16.1，numpy版本为1.26.4。

### Python3.7的环境
查看编译环境

```bash
python3 --version; cmake --version; g++ --version
```

输出信息

```log
Python 3.7.5
cmake version 3.29.3
g++ (Ubuntu/Linaro 7.5.0-3ubuntu1~18.04) 7.5.0
```

安装python三方模块并查看

```bash
pip3 install torch==1.11.0 tensorflow numpy decorator sympy cffi protobuf attrs pyyaml pathlib2 scipy requests psutil absl-py easyprocess wheel expecttest hypothesis cloudpickle decorator tornado synr==0.5.0 packaging cmake setuptools Ninja pybind11
# 以torch-npu仓介绍为准，下载对应版本的whl包，并安装
pip3 install torch_npu-1.11.0.post13-cp37-cp37m-linux_aarch64.whl
pip3 list
```

输出信息

```log
Package                      Version
---------------------------- -------------
absl-py                      2.1.0
astunparse                   1.6.3
attrs                        23.2.0
cachetools                   5.3.3
certifi                      2022.12.7
cffi                         1.15.1
charset-normalizer           3.3.2
cloudpickle                  2.2.1
cmake                        3.29.3
decorator                    5.1.1
EasyProcess                  1.1
exceptiongroup               1.2.1
expecttest                   0.1.6
flatbuffers                  24.3.25
gast                         0.4.0
google-auth                  2.30.0
google-auth-oauthlib         0.4.6
google-pasta                 0.2.0
grpcio                       1.62.2
h5py                         3.8.0
hypothesis                   6.79.4
idna                         3.7
importlib-metadata           6.7.0
keras                        2.11.0
libclang                     18.1.1
Markdown                     3.4.4
MarkupSafe                   2.1.5
mpmath                       1.3.0
ninja                        1.11.1.1
numpy                        1.21.6
oauthlib                     3.2.2
opt-einsum                   3.3.0
packaging                    24.0
pathlib2                     2.3.7.post1
pip                          23.2.1
protobuf                     3.19.6
psutil                       5.9.8
pyasn1                       0.5.1
pyasn1-modules               0.3.0
pybind11                     2.12.0
pycparser                    2.21
PyYAML                       6.0.1
requests                     2.31.0
requests-oauthlib            2.0.0
rsa                          4.9
scipy                        1.7.3
setuptools                   65.6.3
six                          1.16.0
sortedcontainers             2.4.0
sympy                        1.10.1
synr                         0.5.0
tensorboard                  2.11.2
tensorboard-data-server      0.6.1
tensorboard-plugin-wit       1.8.1
tensorflow                   2.11.0
tensorflow-cpu-aws           2.11.0
tensorflow-estimator         2.11.0
tensorflow-io-gcs-filesystem 0.29.0
termcolor                    2.3.0
torch                        1.11.0
torch-npu                    1.11.0.post13
tornado                      6.2
typing_extensions            4.7.1
urllib3                      2.0.7
Werkzeug                     2.2.3
wheel                        0.38.4
wrapt                        1.16.0
zipp                         3.15.0
```
由上方信息得知，pytorch和torch-npu版本均为1.11.0，tensorflow版本为2.11.0，numpy版本为1.21.6。
