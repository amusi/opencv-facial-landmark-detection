# opencv-facial-landmark-detection
Summary：利用OpenCV中的LBF算法进行人脸关键点检测（Facial Landmark Detection）

Author：     Amusi

Date：         2018-03-20

Note：         **OpenCV3.4+OpenCV-Contrib**以及上支持Facemark



下面是Amusi具体利用OpenCV中的LBF算法进行人脸关键点检测的教程，**大家如果喜欢这个教程，记得给个star！**项目的教程和源码，只需要你在命令行或终端打开git后，输入下述命令即可，或者直接点击Clone or download，并选择Download ZIP下载压缩包即可。

`git clone https://github.com/amusi/opencv-facial-landmark-detection.git`



# 测试环境

- Windows10
- Visual Studio 2013
- [OpenCV3.4.1](https://github.com/opencv/opencv/releases)+[OpenCV-Contrib3.4.1](https://github.com/opencv/opencv_contrib/releases/tag/3.4.1)



Note：感谢apple ^(oo)^兰 童鞋的提醒，注意本教程一定需要OpenCV-Contrib的支持。即你的OpenCV环境需要使用CMake对OpenCV和OpenCV-Contrib进行编译，生成动态链接库。版本最好是3.4.1。




# 引言

人脸一般是有68个关键点，常用的人脸开源库有Dlib，还有很多深度学习的方法。

![OpenCV Facemark : Facial Landmark Detection](http://www.learnopencv.com/wp-content/uploads/2018/03/OpenCV-Facemark.jpg)

本教程仅利用OpenCV，不依赖其它第三方库来实现人脸关键点检测，这一特性是之前没有的。因为OpenCV自带的samples中只有常见的人脸检测、眼睛检测和眼镜检测等（方法是harr+cascade或lbp+cascade）。

本教程主要参考[Facemark : Facial Landmark Detection using OpenCV](https://www.learnopencv.com/facemark-facial-landmark-detection-using-opencv/)



截止到2018-03-20，OpenCV3.4可支持三种人脸关键点检测，但目前只能找到一种已训练好的模型，所以本教程只介绍一种实现人脸关键点检测的算法。而且此类算法还没有Python接口，所以这里只介绍C++的代码实现。



# Facemark API

OpenCV官方的人脸关键点检测API称为Facemark。Facemark目前分别基于下述三篇论文，实现了三种人脸关键点检测的方法。

- [FacemarkKazemi](https://docs.opencv.org/trunk/dc/de0/classcv_1_1face_1_1FacemarkKazemi.html): This implementation is based on a paper titled “[One Millisecond Face Alignment with an Ensemble of Regression Trees](http://www.csc.kth.se/~vahidk/face_ert.html)” by V.Kazemi and J. Sullivan published in CVPR 2014. An alternative implementation of this algorithm can be found in DLIB
- [FacemarkAAM](https://docs.opencv.org/trunk/d5/d7b/classcv_1_1face_1_1FacemarkAAM.html): This implementation uses an Active Appearance Model (AAM) and is based on an the paper titled “[Optimization problems for fast AAM fitting in-the-wild](https://ibug.doc.ic.ac.uk/media/uploads/documents/tzimiro_pantic_iccv2013.pdf)” by G. Tzimiropoulos and M. Pantic, published in ICCV 2013.
- [FacemarkLBF](https://docs.opencv.org/trunk/dc/d63/classcv_1_1face_1_1FacemarkLBF.html): This implementation is based a paper titled “[Face alignment at 3000 fps via regressing local binary features](http://www.jiansun.org/papers/CVPR14_FaceAlignment.pdf)” by S. Ren published in CVPR 2014.

在写这篇文章的时候，FacemarkKazemi类似乎不是从Facemark类派生的，而其他两个类都是。



# Facemark训练好的模型

尽管Facemark API包含三种不同的实现，但只有FacemarkLBF（local binary features，LBF）才提供经过训练的模型。 （之后在我们根据公共数据集训练我们自己的模型后，这篇文章将在未来更新）

你可以从中下载已训练好的模型：

- [lbfmodel.yaml](https://github.com/kurnianggoro/GSOC2017/blob/master/data/lbfmodel.yaml)



# 利用OpenCV代码进行实时人脸关键点检测

## 步骤

### 1. 加载人脸检测器（face detector）

所有的人脸关键点检测算法的输入都是一个截切的人脸图像。因为，我们的第一步就是在图像中检测所有的人脸，并将所有的人脸矩形框输入到人脸关键点检测器中。这里，我们可以使用OpenCV的Haar人脸检测器或者lbp人脸检测器来检测人脸。

### 2. 创建Facemark对象

创建Facemark类的对象。在OpenCV中，Facemark是使用智能指针（smart pointer，PTR），所以我们不需要考虑内存泄漏问题。

### 3. 加载landmark检测器

加载关键点检测器（lbfmodel.yaml）。此人脸检测器是在几千幅带有关键点标签的人脸图像上训练得到的。

带有注释/标签关键点的人脸图像公共数据集可以访问这个链接下载：<https://ibug.doc.ic.ac.uk/resources/facial-point-annotations/>

### 4.从网络摄像头中捕获帧

捕获视频帧并处理。我们既可以打开一个本地视频(.mp4)，也可以打开网络摄像机（如果电脑有的话）来进行人脸关键点检测。

### 5. 检测人脸

我们对视频的每一帧运行人脸检测器。人脸检测器的输出是一个包含一个或多个矩形（rectangles）的容器（vector），即视频帧中可能有一张或者多张人脸。

### 6. 运行人脸关键点检测器

我们根据人脸矩形框截取原图中的人脸ROI，再利用人脸关键点检测器（facial landmark detector）对人脸ROI进行检测。

对于每张脸我们获得，我们可以获得68个关键点，并将其存储在点的容器中。因为视频帧中可能有多张脸，所以我们应采用点的容器的容器。

### 7. 绘制人脸关键点

根据获得关键点，我们可以在视频帧上绘制出来并显示。

代码

本教程的代码一共有两个程序，分别为**faceLandmarkDetection.cpp**和**drawLandmarks.hpp**。

- faceLandmarkDetection.cpp实现视频帧捕获、人脸检测、人脸关键点检测；

- drawLandmarks.hpp实现人脸关键点绘制和多边形线绘制。

  ​

**faceLandmarkDetection.cpp和drawLandmarks.hpp都在src文件夹中**



## 实验结果

![检测结果](https://note.youdao.com/yws/public/resource/b5334a177843104c56a91e9ee2a81451/xmlnote/WEBRESOURCE28d3a06b0c49897c89bba8de641eeabe/33724)



# Reference

[1]Tutorial：<https://www.learnopencv.com/facemark-facial-landmark-detection-using-opencv/>

[2]Code：<https://github.com/spmallick/learnopencv/tree/master/FacialLandmarkDetection>

[3]Models：<https://github.com/kurnianggoro/GSOC2017>

[4]本教程所有文件打包：

链接1（百度云网盘）：https://pan.baidu.com/s/16PZ-McVgRwB3bH1Y2fEWBA  密码：x8be

链接2：<http://anonfile.com/e9caRad3bf/Facemark_LBF.rar>