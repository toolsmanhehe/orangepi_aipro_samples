## 目录结构介绍
```
├── AddKernelInvocation
│   ├── cmake                   // 编译工程文件
│   ├── input                   // 存放脚本生成的输入数据目录
│   ├── output                  // 存放算子运行输出数据和真实数据的目录
│   ├── scripts
│   │   ├── acl.json            // acl配置文件
│   │   ├── gen_data.py         // 验证输出数据和针织数据是否一致的验证脚本
│   │   └── verify_result.py    // 真值对比文件
│   │── add_custom.cpp          // 算子kernel实现
│   │── CMakeLists.txt          // 编译工程文件
│   │── data_utils.h            // 数据读入写出函数
│   │── main.cpp                // 主函数，调用算子的应用程序，含CPU域及NPU域调用
│   └── run.sh                  // 编译运行算子的脚本
```
## 代码实现介绍
本调用样例中实现的是固定shape为8*2048的Add算子。
- kernel实现  
  Add算子的数学表达式为：
  ```
  z = x + y
  ```
  计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用计算接口完成两个输入参数相加，得到最终结果，再搬出到外部存储上。

  Add算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入Tensor xGm和yGm搬运到Local Memory，分别存储在xLocal、yLocal，Compute任务负责对xLocal、yLocal执行加法操作，计算结果存储在zLocal中，CopyOut任务负责将输出数据从zLocal搬运至Global Memory上的输出Tensor zGm中。具体请参考[add_custom.cpp](./add_custom.cpp)。

- 调用实现
  1. CPU侧运行验证主要通过ICPU_RUN_KF CPU调测宏等CPU调测库提供的接口来完成；
  2. NPU侧运行验证主要通过使用<<<>>>内核调用符来完成。

  应用程序通过ASCENDC_CPU_DEBUG 宏区分代码逻辑运行于CPU侧还是NPU侧。
## 运行样例算子
- 打开样例目录

  ```bash
  cd $HOME/samples/operator/AddCustomSample/KernelLaunch/AddKernelInvocation
  ```
- 配置环境变量

  这里的\$HOME需要替换为CANN包的安装路径。
  ```bash
  export ASCEND_INSTALL_PATH=$HOME/Ascend/ascend-toolkit/latest
  ```

- 样例执行

  ```bash
  bash run.sh -r [RUN_MODE] -v  [SOC_VERSION]
  ```
  - RUN_MODE：编译方式，可选择CPU调试，NPU仿真，NPU上板。支持参数为[cpu / sim / npu]，默认值为cpu。
  - SOC_VERSION：昇腾AI处理器型号，如果无法确定具体的[SOC_VERSION]，则在安装昇腾AI处理器的服务器执行npu-smi info命令进行查询，在查询到的“Name”前增加Ascend信息，例如“Name”对应取值为xxxyy，实际配置的[SOC_VERSION]值为Ascendxxxyy。支持以下参数取值（xxx请替换为具体取值）：
    - Atlas 推理系列产品（Ascend 310P处理器）参数值：Ascend310P1、Ascend310P3
    - Atlas 训练系列产品参数值：AscendxxxA、AscendxxxB
    - Atlas A2训练系列产品参数值：AscendxxxB1、AscendxxxB2、AscendxxxB3、AscendxxxB4

  注：针对Atlas 训练系列产品使用NPU仿真调试，会存在精度问题，可选择其他芯片进行NPU仿真调试。

  示例如下。
  ```bash
  bash run.sh -r cpu -v Ascend310P1
  ```
## 更新说明
| 时间       | 更新事项     |
| ---------- | ------------ |
| 2023/05/22 | 新增本readme |