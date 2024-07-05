中文|[English](README.md)

**本样例为大家学习昇腾软件栈提供参考，非商业目的！**

**本README只提供命令行方式运行样例的指导，如需在Mindstudio下运行样例，请参考[Mindstudio运行图片样例wiki](https://gitee.com/ascend/samples/wikis/Mindstudio%E8%BF%90%E8%A1%8C%E5%9B%BE%E7%89%87%E6%A0%B7%E4%BE%8B?sort_id=3164874)。**

## crop样例
功能：调用dvpp的crop接口，实现图像裁剪功能。   
样例输入：原始YUV图片。   
样例输出：裁剪后的YUV图片。   

### 前置条件
请检查以下条件要求是否满足，如不满足请按照备注进行相应处理。如果CANN版本升级，请同步检查第三方依赖(如Mxvision等)是否需要重新安装。
| 条件 | 要求 | 备注 |
|---|---|---|
| CANN版本 | >=8.0.0 | 本案例基于CANN 8.0.RC2.alpha003 版本测试，不兼容5.0.4以下版本。如需更换版本请参考[安装说明](../../README.md#软件)进行安装。|
| 硬件要求 | OrangePi_AIpro (8-12T)/Orangepi_AIpro (20T)  | 当前已在Orangepi_AIpro测试通过|

### 样例部署

执行以下命令，执行编译脚本，开始样例编译。   
```
cd ./scripts    
bash sample_build.sh
```

### 样例运行
    
执行运行脚本，开始样例运行。

    ```
    bash sample_run.sh
    ```

### 查看结果

运行完成后，会在运行环境样例目录下的out/output中生成crop后的YUV图片。

### 常见错误
请参考[常见问题定位](https://gitee.com/ascend/samples/wikis/%E5%B8%B8%E8%A7%81%E9%97%AE%E9%A2%98%E5%AE%9A%E4%BD%8D/%E4%BB%8B%E7%BB%8D)对遇到的错误进行排查。如果wiki中不包含，请在samples仓提issue反馈。