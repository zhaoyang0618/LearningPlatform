#include "HarrisCornerDetector.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

//static 表明这些变量的作用域就到这个文件的末尾
static Mat src, src_gray;
static int thresh = 200;
static int max_thresh = 255;
static const char* source_window = "Source image";
static const char* corners_window = "Corners detected";

/// <summary>
/// 处理进度条
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
void cornerHarris_demo(int, void*)
{
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    Mat dst = Mat::zeros(src.size(), CV_32FC1);
    cornerHarris(src_gray, dst, blockSize, apertureSize, k);
    Mat dst_norm, dst_norm_scaled;
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);
    for (int i = 0; i < dst_norm.rows; i++)
    {
        for (int j = 0; j < dst_norm.cols; j++)
        {
            if ((int)dst_norm.at<float>(i, j) > thresh)
            {
                circle(dst_norm_scaled, Point(j, i), 5, Scalar(0), 2, 8, 0);
            }
        }
    }
    namedWindow(corners_window);
    imshow(corners_window, dst_norm_scaled);
}

void HarrisCornerDetector::test(const char* imagefile)
{
    //CommandLineParser parser(argc, argv, "{@input | building.jpg | input image}");
    //src = imread(samples::findFile(parser.get<String>("@input")));
    src = imread(imagefile);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return;
    }
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    namedWindow(source_window);
    createTrackbar("Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo);
    imshow(source_window, src);
    cornerHarris_demo(0, 0);
    waitKey();
}
