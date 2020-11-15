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

	///
	/// https://docs.opencv.org/master/d7/d37/tutorial_mat_mask_operations.html
	/// 
	static void Sharpen(const cv::Mat& myImage, cv::Mat& Result);
	/// <summary>
	/// 对比自己的函数实现和OpenCV函数之间的性能差异
	/// </summary>
	static void TestFilter2DAndSharpen(cv::Mat& src);

	/// <summary>
	/// https://docs.opencv.org/master/d5/d98/tutorial_mat_operations.html
	/// 一些基本操作
	/// </summary>
	static void TestBasicOperations();
};

