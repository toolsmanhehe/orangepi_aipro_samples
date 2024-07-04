## 目录结构介绍
```
├── AclNNInvocation             //通过aclnn调用的方式调用AddCustom算子
│   ├── inc                     // 头文件目录
│   │   ├── common.h            // 声明公共方法类，用于读取二进制文件
│   │   ├── op_runner.h         // 算子描述声明文件，包含算子输入/输出，算子类型以及输入描述与输出描述
│   │   └── operator_desc.h     // 算子运行相关信息声明文件，包含算子输入/输出个数，输入/输出大小等
│   ├── input                   // 存放脚本生成的输入数据目录
│   ├── output                  // 存放算子运行输出数据和真实数据的目录
│   ├── scripts
│   │   ├── acl.json            // acl配置文件
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 真值对比文件
│   ├── src
│   │   ├── CMakeLists.txt      // 编译规则文件
│   │   ├── common.cpp          // 公共函数，读取二进制文件函数的实现文件
│   │   ├── main.cpp            // 单算子调用应用的入口
│   │   ├── op_runner.cpp       // 单算子调用主体流程实现文件
│   │   └── operator_desc.cpp   // 构造算子的输入与输出描述
│   └── run.sh                  // 执行命令脚本
```
## 代码实现介绍
完成自定义算子的开发部署后，可以通过单算子调用的方式来验证单算子的功能。src/main.cpp代码为单算子API执行方式。单算子API执行是基于C语言的API执行算子，无需提供单算子描述文件进行离线模型的转换，直接调用单算子API接口。

自定义算子编译部署后，会自动生成单算子API，可以直接在应用程序中调用。算子API的形式一般定义为“两段式接口”，形如：
   ```cpp
   // 获取算子使用的workspace空间大小
   aclnnStatus aclnnAddCustomGetWorkspaceSize(const aclTensor *x, const aclTensor *y, const alcTensor *out, uint64_t workspaceSize, aclOpExecutor **executor);
   // 执行算子
   aclnnStatus aclnnAddCustom(void *workspace, int64_t workspaceSize, aclOpExecutor **executor, aclrtStream stream);
   ```
其中aclnnAddCustomGetWorkspaceSize为第一段接口，主要用于计算本次API调用计算过程中需要多少的workspace内存。获取到本次API计算需要的workspace大小之后，按照workspaceSize大小申请Device侧内存，然后调用第二段接口aclnnAddCustom执行计算。具体参考[AscendCL单算子调用](https://hiascend.com/document/redirect/CannCommunityAscendCInVorkSingleOp)>单算子API执行 章节。

## 运行样例算子
### 1. 编译算子工程
运行此样例前，请参考[编译算子工程](../README.md#operatorcompile)完成前期准备。
### 2. aclnn调用样例运行

  - 进入到样例目录

    ```bash
    cd $HOME/samples/operator/AddCustomSample/FrameworkLaunch/AclNNInvocation
    ```
  - 样例执行

    样例执行过程中会自动生成测试数据，然后编译与运行aclnn样例，最后检验运行结果。具体过程可参见run.sh脚本。

    ```bash
    bash run.sh
    ```
## 更新说明
| 时间       | 更新事项     |
| ---------- | ------------ |
| 2024/05/22 | 新增本readme |