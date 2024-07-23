import numpy as np
import videocapture as video
import acl
import acllite_utils as utils
import time
import cv2
import constants as const

from acllite_resource import AclLiteResource
from acllite_model import AclLiteModel
from acllite_imageproc import AclLiteImageProc
from acllite_image import AclLiteImage
from acllite_logger import log_error, log_info
from multiprocessing import Process, Queue, Pool, Value

Q_PRE_SIZE = 32
Q_OUT_SIZE = 32
VIDEO_WIDTH = 1920
VIDEO_HEIGHT = 1080
WAIT_TIME = 0.000003
labels = ["person", "bicycle", "car", "motorbike", "aeroplane",
          "bus", "train", "truck", "boat", "traffic light",
          "fire hydrant", "stop sign", "parking meter", "bench", "bird",
          "cat", "dog", "horse", "sheep", "cow",
          "elephant", "bear", "zebra", "giraffe", "backpack",
          "umbrella", "handbag", "tie", "suitcase", "frisbee",
          "skis", "snowboard", "sports ball", "kite", "baseball bat",
          "baseball glove", "skateboard", "surfboard", "tennis racket", "bottle",
          "wine glass", "cup", "fork", "knife", "spoon",
          "bowl", "banana", "apple", "sandwich", "orange", "broccoli", "carrot", "hot dog",
          "pizza", "donut", "cake", "chair", "sofa", "potted plant", "bed", "dining table",
          "toilet", "TV monitor", "laptop", "mouse", "remote", "keyboard", "cell phone",
          "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase",
          "scissors", "teddy bear", "hair drier", "toothbrush"]

def preprocess(path, q_pre, model_width, model_height, channel):
    print(f'sub process preprocess{i} start')
    VIDEO_WIDTH = 1920
    VIDEO_HEIGHT = 1080
    scale_x = VIDEO_WIDTH / model_width
    scale_y = VIDEO_HEIGHT / model_height
    # get scale factor
    if scale_x > scale_y:
        max_scale = scale_x
        resize_shape = (model_width, int(VIDEO_HEIGHT/max_scale))
    else:
        max_scale = scale_y
        resize_shape = (int(VIDEO_WIDTH/max_scale), model_height)
    count = 0
    cap = cv2.VideoCapture(path)
    if not cap.isOpened() :
        print('video connect failed')
        exit(0)
    while True:
        ret, frame = cap.read()
        if not ret: #  
            print('cap read end! close subprocess cap read')
            q_pre.put('EOF')
            break
        else:
            img = np.zeros([model_height, model_width, 3], dtype=np.uint8)
            # frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            resize_image = cv2.resize(frame, resize_shape)
            img[0:resize_shape[1],0:resize_shape[0]] = resize_image
            q_pre.put(img)
            count += 1
    print(f'pre process end! {channel}')

def infer(model_path, q_pre, q_out, pnums):
    resource = AclLiteResource()
    resource.init()
    model = AclLiteModel(model_path)
    count = 0
    nums = pnums
    image_info = np.array([640, 640,
                            640, 640],
                            dtype=np.float32)
    start = time.time()
    while True:
        if pnums == 0:
            break
        for i,q in enumerate(q_pre):
            img = q.get()
            if isinstance(img, str):
                pnums -= 1
                q_out[i].put('EOF')
                continue
            output = model.execute([img, image_info])
            count += 1
            q_out[i].put(output)
    end = time.time()
    print(f'fps: {count/(end-start):.3f}')
    del resource
    del model
    print('infer end! close infer ')

def postprocess(q_out, model_width, model_height):
    VIDEO_WIDTH = 1920
    VIDEO_HEIGHT = 1080
    while True:
        output = q_out.get()
        if isinstance(output, str):
            print('postprocess end! close subprocess postprocess')
            break
        box_num = output[1][0, 0]
        box_info = output[0].flatten()
        scale_x = VIDEO_WIDTH / model_width
        scale_y = VIDEO_HEIGHT / model_height

        # get scale factor
        if scale_x > scale_y:
            max_scale = scale_x
        else:
            max_scale = scale_y
        colors = [0, 0, 255]

        # draw the boxes in original image
        result_msg = ""
        for n in range(int(box_num)):
            ids = int(box_info[5 * int(box_num) + n])
            score = box_info[4 * int(box_num) + n]
            label = labels[ids] + ":" + str("%.2f" % score)
            top_left_x = box_info[0 * int(box_num) + n] * max_scale
            top_left_y = box_info[1 * int(box_num) + n] * max_scale
            bottom_right_x = box_info[2 * int(box_num) + n] * max_scale
            bottom_right_y = box_info[3 * int(box_num) + n] * max_scale
            result_msg += f'label:{label}  '
            # cv2.rectangle(src_image, (int(top_left_x), int(top_left_y)),
            #             (int(bottom_right_x), int(bottom_right_y)), colors)
            # p3 = (max(int(top_left_x), 15), max(int(top_left_y), 15))
            # cv2.putText(src_image, label, p3, cv2.FONT_ITALIC, 0.6, colors, 1)
            # cv2.imshow('frame', src_image)
        # cv2.imwrite(f'../out/out_{count}.jpg', src_image)
        print(f'results: {result_msg}')

if __name__ == '__main__':  
    stream_path = "../data/test.mp4"
    model_path = "../model/yolov5s_rgb.om"
    model_width = 640
    model_height = 640
    pnums = 2

    q_pre = [Queue(maxsize=Q_PRE_SIZE) for i in range(pnums)]
    q_out = [Queue(maxsize=Q_OUT_SIZE) for i in range(pnums)]
    
    loopTime, initTime = time.time(), time.time()

    processes = []
    for i in range(pnums):
        processes.append(Process(target=preprocess, args=(stream_path, q_pre[i], model_width, model_height, i)))
    processes.append(Process(target=infer, args=(model_path, q_pre, q_out, pnums)))
    for i in range(pnums):
        processes.append(Process(target=postprocess, args=(q_out[i],model_width, model_width)))

    [process.start() for process in processes]
    [process.join() for process in processes]
    print('子进程运行结束')

