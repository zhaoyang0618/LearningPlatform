#include "FeatureDetection.h"
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"

using namespace cv;
using std::cout;
using std::endl;

#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/xfeatures2d.hpp"
using namespace cv::xfeatures2d;
#endif

/// <summary>
/// ORB, KAZE
/// </summary>
/// <param name="imagefile"></param>
void FeatureDetection::test (const char* imagefile)
{
    Mat src = imread(imagefile, IMREAD_GRAYSCALE);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return;
    }
    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 400;

#ifdef HAVE_OPENCV_XFEATURES2D
    const char* windowName = "SURF Keypoints";
    Ptr<SURF> detector = SURF::create(minHessian);
#else
    const char* windowName = "SIFT Keypoints";
    Ptr<SIFT> detector = SIFT::create();
#endif

    std::vector<KeyPoint> keypoints;
    detector->detect(src, keypoints);
    //-- Draw keypoints
    Mat img_keypoints;
    drawKeypoints(src, keypoints, img_keypoints);

    //-- Show detected (drawn) keypoints
    imshow(windowName, img_keypoints);
    waitKey();
}
