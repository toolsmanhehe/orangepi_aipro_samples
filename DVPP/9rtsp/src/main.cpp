#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <dirent.h>
#include <string.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "acllite_dvpp_lite/ImageProc.h"
#include "acllite_media/CameraRead.h"
#include "acllite_dvpp_lite/VideoRead.h"
#include "acllite_common/Queue.h"
#include <sys/time.h>
#include <chrono>

using namespace std;
using namespace acllite;
using namespace cv;
namespace
{
    aclrtContext context = nullptr;
    // Set the number of channels to open the corresponding number of video streams
    const int channelNum = 9; // 修改为9路摄像头
    // Set showNum for cv::imshow windows arrangement rules
    const int showNum = 3;
    const uint32_t modelWidth = 640;
    const uint32_t modelHeight = 640;

    const uint32_t fullScreenWidth = 1920;
    const uint32_t fullScreenHeight = 1050;
    const uint32_t titleHeight = 30;
    const int channel[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8}; // 修改为9路摄像头

    bool exitFlag = false;

    struct Config
    {
        int32_t deviceId;
        int channel;
    };

    struct MsgData
    {
        std::shared_ptr<uint8_t> data = nullptr;
        uint32_t size = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        int ChannelId = 0;
        bool videoEnd = false;
        cv::Mat srcImg;
    };

    Queue<MsgData> msgDataQueue(channelNum); // 修改为"channelNum"路摄像头
    std::vector<string> winNameList;
    std::string displayMode = "";
}

bool Initparam(int argc, char *argv[])
{
    DIR *dir;
    int outputDisplay = 1;
    if ((dir = opendir("./out")) == NULL)
        system("mkdir ./out");
    int paramNum = 2;
    if (argc != paramNum)
    {
        LOG_PRINT("[ERROR] please choose output display mode: [./main imshow] [./main stdout]");
        return false;
    }
    displayMode = argv[outputDisplay];
    return true;
}

void Draw(cv::Mat &srcImg, int channel)
{
    cv::Mat resizedImg;
    cv::resize(srcImg, resizedImg, cv::Size(fullScreenWidth / showNum, (fullScreenHeight - titleHeight) / showNum));

    // 获取当前时间
    static std::vector<std::chrono::steady_clock::time_point> lastTime(channelNum);
    static std::vector<int> frameCount(channelNum, 0);
    static std::vector<int> fps(channelNum, 0);

    auto currentTime = std::chrono::steady_clock::now();

    // 获取当前帧数
    if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime[channel]).count() >= 1)
    {
        fps[channel] = frameCount[channel];
        lastTime[channel] = currentTime; // 更新上次记录的时间
        frameCount[channel] = 0;         // 重置帧数
    }
    frameCount[channel]++;

    // 创建一个字符串来显示帧数
    std::string fpsStr = "Fps: " + std::to_string(fps[channel]);

    // 在图像左上角添加帧数
    cv::putText(resizedImg, fpsStr, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);

    cv::imshow(winNameList[channel], resizedImg);
    cv::waitKey(1);
}


void *GetInput(void *arg)
{
    bool ret = SetCurContext(context);
    CHECK_RET(ret, LOG_PRINT("[ERROR] set cur context for pthread  %ld failed.", pthread_self()); return NULL);
    Config cfg = *(Config *)arg;
    string videoPath;
    switch (cfg.channel)
    {
    case 0:
        videoPath = "rtsp://10.31.2.47:8554/live";
        break;
    case 1:
        videoPath = "rtsp://10.31.2.47:8554/live";
        break;
    case 2:
        videoPath = "rtmp://10.31.2.47:8554/live";
        break;
    case 3:
        videoPath = "rtmp://10.31.2.47:8554/live";
        break;
    case 4:
        videoPath = "rtmp://10.31.2.47:8554/live";
        break;
    case 5:
        videoPath = "rtmp://10.31.2.47:8554/live";
        break;
    case 6:
        videoPath = "rtmp://10.31.2.47:8554/live";
        break;
    case 7:
        videoPath = "rtmp://10.31.2.47:8554/live";
        break;
    case 8:
        videoPath = "rtmp://10.31.2.47:8554/live";
        break;

    default:
        LOG_PRINT("[ERROR] Invalid channel for GetInput.");
        break;
    }
    VideoRead cap(videoPath, cfg.deviceId);
    CHECK_RET(cap.IsOpened(), LOG_PRINT("[ERROR] open %s failed.", videoPath.c_str()); return NULL);
    ImageProc imageProcess;
    ImageData frame;
    ImageSize modelSize(modelWidth, modelHeight);
    LOG_PRINT("[INFO] start to decode...");
    uint32_t frameRate = 1;
    uint32_t cnt = 1;
    int isHost = GetRunMode();
    while (1)
    {
        ret = cap.Read(frame);
        if (ret)
        {
            if (cnt % frameRate == 0)
            {
                ImageData dst;
                imageProcess.Resize(frame, dst, modelSize, RESIZE_PROPORTIONAL_UPPER_LEFT);
                MsgData msgData;
                msgData.data = dst.data;
                msgData.size = dst.size;
                msgData.width = dst.width;
                msgData.height = dst.height;
                msgData.ChannelId = cfg.channel;
                msgData.videoEnd = false;
                cv::Mat yuv_img(frame.height * 1.5, frame.width, CV_8UC1);
                if (isHost)
                {
                    void *hostDataBuffer = CopyDataToHost(frame.data.get(), frame.size);
                    memcpy(yuv_img.data, (unsigned char *)hostDataBuffer, frame.size);
                    FreeHostMem(hostDataBuffer);
                    hostDataBuffer = nullptr;
                }
                else
                {
                    memcpy(yuv_img.data, (unsigned char *)frame.data.get(), frame.size);
                }
                cv::cvtColor(yuv_img, msgData.srcImg, cv::COLOR_YUV2RGB_NV21);
                while (1)
                {
                    if (msgDataQueue.Push(msgData))
                    {
                        break;
                    }
                    usleep(100);
                }
            }
            cnt++;
        }
        else
        {
            LOG_PRINT("[INFO] frame read end.");
            break;
        }
    }
    cap.Release();
    MsgData msgData;
    msgData.videoEnd = true;
    msgData.ChannelId = cfg.channel;
    while (1)
    {
        if (msgDataQueue.Push(msgData))
        {
            break;
        }
        usleep(100);
    }
    return NULL;
}

void *PostProcess(void *arg)
{
    while (1)
    {
        if (!msgDataQueue.Empty())
        {
            MsgData msgData = msgDataQueue.Pop();
            usleep(100);
            if (msgData.videoEnd)
            {
                break;
            }
            if (displayMode == "imshow")
            {
                Draw(msgData.srcImg, msgData.ChannelId);
            }
            else if (displayMode == "stdout")
            {
                cout << "Channel-" << msgData.ChannelId << "-frame: " << msgData.srcImg.cols << "x" << msgData.srcImg.rows << endl;
            }
            else
            {
                LOG_PRINT("[ERROR] output display mode not supported.");
            }
        }
    }
    exitFlag = true;
    LOG_PRINT("[INFO] *************** all video get done ***************");
    return NULL;
}

int main(int argc, char *argv[])
{
    int32_t deviceId = 0;
    AclLiteResource aclResource(deviceId);
    bool ret = aclResource.Init();
    CHECK_RET(ret, LOG_PRINT("[ERROR] InitACLResource failed."); return 1);
    context = aclResource.GetContext();
    ret = Initparam(argc, argv);
    CHECK_RET(ret, LOG_PRINT("[ERROR] Initparam failed."); return 1);

    std::vector<Config> cfg;
    cfg.resize(channelNum);
    pthread_t preTids[channelNum], posTids;
    for (int i = 0; i < channelNum; i++)
    {
        cfg[i].deviceId = deviceId;
        cfg[i].channel = i;
        pthread_create(&preTids[i], NULL, GetInput, (void *)&cfg[i]);
    }

    if (displayMode == "imshow")
    {
        for (int i = channelNum - 1; i >= 0; i--)
        {
            std::string head = "window";
            string winName = head + to_string(i);
            cv::namedWindow(winName, WINDOW_NORMAL);
            int x = (channel[i] % showNum) * (fullScreenWidth / showNum);
            int y = (channel[i] / showNum) * ((fullScreenHeight - titleHeight) / showNum);
            cv::moveWindow(winName, x, y + titleHeight);
            cv::resizeWindow(winName, fullScreenWidth / showNum, (fullScreenHeight - titleHeight) / showNum);
            winNameList.push_back(winName);
        }
    }
    pthread_create(&posTids, NULL, PostProcess, NULL);

    for (int i = 0; i < channelNum; i++)
    {
        pthread_detach(preTids[i]);
    }
    pthread_detach(posTids);

    while (!exitFlag)
    {
        sleep(10);
    }
    if (displayMode == "imshow")
    {
        cv::destroyAllWindows();
    }
    return 0;
}
