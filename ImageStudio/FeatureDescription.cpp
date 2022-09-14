#include "FeatureDescription.h"
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

void FeatureDescription::test(const char* imagefile1, const char* imagefile2)
{
    Mat img1 = imread(imagefile1, IMREAD_GRAYSCALE);
    Mat img2 = imread(imagefile2, IMREAD_GRAYSCALE);
    if (img1.empty() || img2.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return;
    }
    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int minHessian = 400;
    
#ifdef HAVE_OPENCV_XFEATURES2D
    Ptr<SURF> detector = SURF::create(minHessian);
#else
    Ptr<SIFT> detector = SIFT::create();
#endif

    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
    detector->detectAndCompute(img2, noArray(), keypoints2, descriptors2);
    //-- Step 2: Matching descriptor vectors with a brute force matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
    std::vector< DMatch > matches;
    matcher->match(descriptors1, descriptors2, matches);
    //-- Draw matches
    Mat img_matches;
    drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches);
    //-- Show detected matches
    imshow("Matches", img_matches);
    waitKey();
}
