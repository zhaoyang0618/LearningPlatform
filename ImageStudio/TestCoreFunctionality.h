///
///OpenCV的一些基本用法，例如遍历图像
/// 
#pragma once
#include <opencv2/opencv.hpp>

class TestCoreFunctionality
{
public:
	///
	/// https://docs.opencv.org/master/db/da5/tutorial_how_to_scan_images.html
	/// 
	static void TestScanImage1(cv::Mat src, const uchar* const table);
	static void TestScanImage2(cv::Mat src, const uchar* const table);
	static void TestScanImage3(cv::Mat src, const uchar* const table);
	static void TestScanImage4(cv::Mat src, const uchar* const table);
};

