# fatigue-detection
1. pipeline:
Main thread: Using opencv to capture video frames, adding to image queue.
thread1: using MTCNN for face detetction and using KCF for face tracking, adding results to task queue.
thread2: using ert for landmark detection.
Main thread: showing frame by Qt.

2. Dependencies 
tensorflow c++(how to compile on linux https://blog.csdn.net/Android_chunhui/article/details/97579895)
dlib , opencv, Qt5.9

