// Summary: 绘制人脸关键点和多边形线
// Author:  Amusi
// Date:    2018-03-20

#ifndef _renderFace_H_
#define _renderFace_H_

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv; 
using namespace std; 

#define COLOR Scalar(255, 200,0)

// drawPolyline通过连接开始和结束索引之间的连续点来绘制多边形线。
void drawPolyline
(
  Mat &im,
  const vector<Point2f> &landmarks,
  const int start,
  const int end,
  bool isClosed = false
)
{
    // 收集开始和结束索引之间的所有点
    vector <Point> points;
    for (int i = start; i <= end; i++)
    {
        points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
    }

    // 绘制多边形曲线
    polylines(im, points, isClosed, COLOR, 2, 16);
    
}

// 绘制人脸关键点
void drawLandmarks(Mat &im, vector<Point2f> &landmarks)
{
    // 在脸上绘制68点及轮廓（点的顺序是特定的，有属性的）
    if (landmarks.size() == 68)
    {
      drawPolyline(im, landmarks, 0, 16);           // Jaw line
      drawPolyline(im, landmarks, 17, 21);          // Left eyebrow
      drawPolyline(im, landmarks, 22, 26);          // Right eyebrow
      drawPolyline(im, landmarks, 27, 30);          // Nose bridge
      drawPolyline(im, landmarks, 30, 35, true);    // Lower nose
      drawPolyline(im, landmarks, 36, 41, true);    // Left eye
      drawPolyline(im, landmarks, 42, 47, true);    // Right Eye
      drawPolyline(im, landmarks, 48, 59, true);    // Outer lip
      drawPolyline(im, landmarks, 60, 67, true);    // Inner lip
    }
    else 
    { 
		// 如果人脸关键点数不是68，则我们不知道哪些点对应于哪些面部特征。所以，我们为每个landamrk画一个圆圈。
		for(int i = 0; i < landmarks.size(); i++)
		{
			circle(im,landmarks[i],3, COLOR, FILLED);
		}
    }
    
}

#endif // _renderFace_H_