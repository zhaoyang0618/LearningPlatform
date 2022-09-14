#include "CornerSubPixel.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

static Mat src, src_gray;
static int maxCorners = 10;
static int maxTrackbar = 25;
static RNG rng(12345);
static const char* source_window = "Image";

void goodFeaturesToTrack_Demo2(int, void*)
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
    Size winSize = Size(5, 5);
    Size zeroZone = Size(-1, -1);
    TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 40, 0.001);
    cornerSubPix(src_gray, corners, winSize, zeroZone, criteria);
    for (size_t i = 0; i < corners.size(); i++)
    {
        cout << " -- Refined Corner [" << i << "]  (" << corners[i].x << "," << corners[i].y << ")" << endl;
    }
}

void CornerSubPixel::test(const char* imagefile)
{
    src = imread(imagefile);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return;
    }
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    namedWindow(source_window);
    createTrackbar("Max corners:", source_window, &maxCorners, maxTrackbar, goodFeaturesToTrack_Demo2);
    imshow(source_window, src);
    goodFeaturesToTrack_Demo2(0, 0);
    waitKey();
}
