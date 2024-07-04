## 使用pytorch调用的方式调用AddCustom算子工程
该样例脚本基于Pytorch2.1、python3.9 运行

## 运行样例算子
### 1.编译算子工程
运行此样例前，请参考[编译算子工程](../README.md#operatorcompile)完成前期准备。
### 2.pytorch调用的方式调用样例运行

  - 进入到样例目录

    ```
    cd $HOME/samples/operator/AddCustomSample/FrameworkLaunch/PytorchInvocation
    ```

  - 样例执行

    样例执行过程中会自动生成测试数据，然后运行pytorch样例，最后检验运行结果。具体过程可参见run_op_plugin.sh脚本。
    ```
    bash run_op_plugin.sh
    ```
## 更新说明
| 时间       | 更新事项     |
| ---------- | ------------ |
| 2024/05/22 | 新增本readme |