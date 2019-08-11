# fatigue-detection
1. pipeline:
Main thread: Using opencv to capture video frames, adding to image queue.
thread1: using MTCNN for face detetction and using KCF for face tracking, adding results to task queue.
thread2: using ert for landmark detection.
Main thread: showing frame by Qt.

2. Dependencies 
tensorflow c++(how to compile on linux https://blog.csdn.net/Android_chunhui/article/details/97579895)
dlib , opencv, Qt5.9

video:
https://youtu.be/rKe3EQg9AAk


- **疲劳驾驶检测**
结合眼睛的闭合状态和嘴巴闭合状态，综合判断驾驶人员的疲劳状况。Qt编写界面，tensorflow，opencv和dlib实现人脸的检测和特征点提取。
- **运行流程**：
主线程：打开摄像设备，将图片加入处理队列。
线程1：从图片队列中获得图片进行人脸的检测和追踪，并将检测结果加入新的任务队列。
线程2：从任务队列中获得图像和检测结果，进行特征点提取并判断疲劳状态和专心程度。
主线程：显示图片和诊断结果。
- **具体细节**
1. 人脸检测与跟踪
在初始帧以及追踪置信度低于阈值时进行人脸检测，其他情况使用KCF追踪人脸。
检测部分使用MTCNN模型（tensorflow实现），640*480图片处理时间大约在70ms左右，比opencv（HOG+adaboost）的>100ms快。
KCF人脸追踪时间在40ms左右。

2. 人脸特征点提取
使用dlib进行特征点提取，所用模型为ert，是对GDBT的级联算法。使用ibug的人脸特征点数据集和300张线下采集的图片重新训练ERT模型。

3. 判断疲劳与注意力分散的指标。
dlib特征点标注：
![]("https://github.com/chunhui204/fatigue-detection/blob/master/a.PNG")
![]("https://github.com/chunhui204/fatigue-detection/blob/master/b.PNG")
