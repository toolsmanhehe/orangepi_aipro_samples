## 使用tensorflow调用的方式调用AddCustom算子工程

## 运行样例算子
### 1.编译算子工程
运行此样例前，请参考[编译算子工程](../README.md#operatorcompile)完成前期准备。
### 2.tensorflow调用的方式调用样例运行

  - 进入到样例目录

    ```bash
    cd $HOME/samples/operator/AddCustomSample/FrameworkLaunch/TensorflowInvocation
    ```

  - 样例执行(tensorflow1.15)

    样例执行过程中会自动生成随机测试数据，然后通过TensorFlow调用算子，最后对比cpu和aicore运行结果。具体过程可参见run_add_custom.py脚本。
    ```bash
    python3 run_add_custom.py
    ```
  - 样例执行(tensorflow2.x)

    样例执行过程中会自动生成随机测试数据，然后通过TensorFlow调用算子，最后对比cpu和aicore运行结果。具体过程可参见run_add_custom_tf2.py脚本。
    ```bash
    python3 run_add_custom_tf2.py
    ```

    用户亦可参考run.sh脚本进行编译与运行。
    ```bash
    bash run.sh
    ```

## 更新说明
| 时间       | 更新事项     |
| ---------- | ------------ |
| 2024/05/22 | 新增本readme |