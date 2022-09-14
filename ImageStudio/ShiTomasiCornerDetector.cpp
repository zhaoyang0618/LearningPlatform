#include "ShiTomasiCornerDetector.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

//static 表明这些变量的作用域就到这个文件的末尾
static Mat src, src_gray;
static int maxCorners = 23;
static int maxTrackbar = 100;
static RNG rng(12345);
static const char* source_window = "Image";

void goodFeaturesToTrack_Demo(int, void*)
{
    maxCorners = MAX(maxCorners, 1);
    vector<Point2f> corners;
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3, gradientSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;
    Mat copy = src.clone();
    goodFeaturesToTrack(src_gray,
        corners,
        maxCorners,
        qualityLevel,
        minDistance,
        Mat(),
        blockSize,
        gradientSize,
        useHarrisDetector,
        k);
    cout << "** Number of corners detected: " << corners.size() << endl;
    int radius = 4;
    for (size_t i = 0; i < corners.size(); i++)
    {
        circle(copy, corners[i], radius, Scalar(rng.uniform(0, 255), rng.uniform(0, 256), rng.uniform(0, 256)), FILLED);
    }
    namedWindow(source_window);
    imshow(source_window, copy);
}
void ShiTomasiCornerDetector::test(const char* imagefile)
{
    src = imread(imagefile);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return;
    }
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    namedWindow(source_window);
    createTrackbar("Max corners:", source_window, &maxCorners, maxTrackbar, goodFeaturesToTrack_Demo);
    imshow(source_window, src);
    goodFeaturesToTrack_Demo(0, 0);
    waitKey();
}
