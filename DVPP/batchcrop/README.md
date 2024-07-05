# Media Data Processing \(Single-Image, Multi-ROI Cropping\)<a name="EN-US_TOPIC_0302603665"></a>

## Overview<a name="section7940919203810"></a>

In this example, eight 224 x 224 child images \(YUV420SP NV12\) are cropped out from the YUV420SP \(NV12\) input based on the specified cropping areas.


## Directory Structure<a name="section1394162513386"></a>

The sample directory is organized as follows:

```
├── data
│   ├── dvpp_vpc_1920x1080_nv12.yuv            //Test image. Obtain the test image according to the guide and save it to the data directory.

├── inc
│   ├── dvpp_process.h               //Header file that declares functions related to data preprocessing
│   ├── sample_process.h               //Header file that declares functions related to model processing
│   ├── utils.h                       //Header file that declares common functions (such as file reading function)

├── src
│   ├── acl.json              //Configuration file for system initialization
│   ├── CMakeLists.txt         //Build script
│   ├── dvpp_process.cpp       //Implementation file of functions related to video processing
│   ├── main.cpp               //Main function, which is the implementation file of single-image, multi-ROI cropping
│   ├── sample_process.cpp     //Implementation file of functions related to resource initialization and destruction
│   ├── utils.cpp              //Implementation file of common functions (such as the file reading function)

├── CMakeLists.txt    //Build script that calls the CMakeLists file in the src directory
```

## Environment Requirements<a name="section3833348101215"></a>

-   OS and architecture: Ubuntu 22.04.4 LTS aarch64
-   Compiler: g++ or aarch64-linux-gnu-g++
-   SoC: Ascend 310B AI Processor
-   The Ascend AI software stack has been deployed on the environment and the corresponding environment variables have been configured. Please refer to the corresponding version of the CANN installation guide in [Link](https://www.hiascend.com/document).

## Prepare pictures <a name="section137281211130"></a>

1. Check if there are test images in the [data](./data) directory. If not, execute the following command as a `HwHiAiUser` user on the development board to obtain the input image of the sample.

   ```
   cd ./data
   wget https://obs-9be7.obs.cn-east-2.myhuaweicloud.com/models/aclsample/dvpp\_vpc\_1920x1080\_nv12.yuv
   ```


## Build and Run <a name="section19471849121012"></a>

1. Compile code
    1.  Log in to the development environment as `HwHiAiUser`.

    2.  Set environment variables and configure the header and library file paths that program compilation depends on. If using the image provided by Orange Pi, you can skip this step.
  
        After the following environment variables are set, the compilation script will look for the compiled-dependent header files according to the "{DDK_PATH} environment variable value/runtime/include/acl" directory, and the compiled-independent library files according to the directory pointed to by the {NPU_HOST_LIB} environment variable. Replace "DDK_PATH" with the actual installation path of the "Ascend-cann-toolkit" package.
  
           ````
        vim ~/.bashrc   #Add the following environment variables
        export DDK_PATH=/usr/local/Ascend/ascend-toolkit/latest/
        export NPU_HOST_LIB=$DDK_PATH/runtime/lib64/stub
        source ~/.bashrc
           ````
       
          You can log in to the corresponding environment and run the "uname -a" command to query the architecture of its operating system.


    3.  Go to the sample directory and create a directory for storing build outputs. For example, the directory created in this sample is  **build/intermediates/host**.

        ```
        mkdir -p build/intermediates/host
        ```

    4.  Go to the  **build/intermediates/host**  directory and run the  **cmake**  command.

        Replace  **../../../src**  with the actual directory of  **CMakeLists.txt**.

        Set **DCMAKE_SKIP_RPATH** to  **TRUE**,  **rpath**  (path specified by  **NPU_HOST_LIB**) is not added to the executable generated after build. The executable automatically looks up for dynamic libraries in the path  included in  **LD_LIBRARY_PATH**.

        ```
        cd build/intermediates/host
        cmake ../../../src -DCMAKE_CXX_COMPILER=g++ -DCMAKE_SKIP_RPATH=TRUE
        ```


    6.  Run the  **make **command. The  **main**  executable file is generated in  **/out**  under the sample directory.

        ```
        make
        ```


2.  Run the app.

    1.  Go to the directory where the executable file  **main**  is located and grant execute permission on the  **main**  file in the directory.

        ```
        cd ../../../out
        chmod +x main
        ```

    4.  Go to the directory where the executable file  **main**  is located  and run the executable file.

        ```
        ./main
        ```

        The following messages indicate that the file is successfully executed.

        ```
        [INFO]  acl init success
        [INFO]  set device 0 success
        [INFO]  create context success
        [INFO]  create stream success
        [INFO]  dvpp init resource success
        [INFO]  ProcessBatchCrop start.
        [INFO]  open file = ../data/dvpp_vpc_1920x1080_nv12.yuv success.
        [INFO]  set inputDesc success.
        [INFO]  write out to file ./cropName0 success.
        [INFO]  write out to file ./cropName1 success.
        [INFO]  write out to file ./cropName2 success.
        [INFO]  write out to file ./cropName3 success.
        [INFO]  write out to file ./cropName4 success.
        [INFO]  write out to file ./cropName5 success.
        [INFO]  write out to file ./cropName6 success.
        [INFO]  write out to file ./cropName7 success.
        [INFO]  ProcessBatchCrop success.
        [INFO]  ProcessBatchCrop success.
        [INFO]  DestroyBatchCropResource start.
        [INFO]  DestroyBatchCropResource end.
        [INFO]  end to destroy stream
        [INFO]  end to destroy context
        [INFO]  end to reset device 0
        [INFO]  end to finalize acl
        ```

        After the executable file is executed successfully, a result file is generated in the directory at the same level as the  **main**  file for later query.

        The child images cropped from the input image  **dvpp\_vpc\_1920×1080\_nv12.yuv**  are as follows:  **cropName0**,  **cropName1**,  **cropName2**,  **cropName3**,  **cropName4**,  **cropName5**,  **cropName6**, and  **cropName7**



## Key Interfaces<a name="section6271153719394"></a>

The following lists the key functions and key interfaces involved in this sample.

-   Initialization
    -   **aclInit**: initializes AscendCL.
    -   **aclFinalize**: deinitializes AscendCL.

-   Device management
    -   **aclrtSetDevice**: sets the compute device.
    -   **aclrtGetRunMode**: obtains the run mode of the  Ascend AI Software Stack. The internal processing varies with the run mode.
    -   **aclrtResetDevice**: resets the compute device and cleans up all resources associated with the device.

-   Context management
    -   **aclrtCreateContext**: creates a context.
    -   **aclrtDestroyContext**: destroys a context.

-   Stream management
    -   **aclrtCreateStream**: creates a stream.
    -   **aclrtDestroyStream**: destroys a stream.
    -   **aclrtSynchronizeStream**: waits for stream tasks to complete.

-   Memory management
    -   **aclrtMallocHost**: allocates host memory.
    -   **aclrtFreeHost**: frees host memory.
    -   **aclrtMalloc**: allocates device memory.
    -   **aclrtFree**: frees device memory.
    -   In data preprocessing, if you need to allocate device memory to store the input or output data, call  **acldvppMalloc**  to allocate memory and call  **acldvppFree**  to free memory.

-   Data transfer

    **aclrtMemcpy**: copies memory.

-   Data preprocessing

    **acldvppVpcBatchCropAsync**: crops one or more selected ROIs from each input image and loads the cropped one or more images to the output buffer. 

