///
/// 图像处理的各种算法测试
#pragma once
#include <opencv2/opencv.hpp>

class TestImgProcFunctionality
{
public:
	/// <summary>
	/// 二值化
	/// </summary>
	/// <param name="filepath"></param>
	/// <param name="outputFolder"></param>
	/// <param name="outputPrefix"></param>
	static void testThreshold(const char* filepath, const char* outputFolder, const char* outputPrefix);

	/// <summary>
	/// Hough直线
	/// </summary>
	/// <param name="filename"></param>
	static void testHoughLine(const char* filename);

	/// <summary>
	/// 离散傅里叶变换
	/// </summary>
	static void testDiscreteFourierTransform(const char* filename);

	/// <summary>
	/// 高斯模糊
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="outfilename"></param>
	static void testResize(const char* filename, const char* outfilename);
	static void testBlur(const char* filename, const char* outfilename);
	static void testGauss(const char* filename, const char* outfilename);
	static void testSharpen(const char* filename, const char* outfilename);
	static void testNoise(const char* filename, const char* outfilename);

	static void testMorph(const char* filepath);
	template<typename FUNC>
	static void findHorizontalAndVerticalLine(cv::Mat& src, FUNC output);

	static void findHorizontalAndVerticalLine(const char* filepath, const char* outDir, int index);

	/// <summary>
	/// 遍历文件夹下的图片，做一次上面的操作
	/// </summary>
	/// <param name="dir"></param>
	static void throughoutFolder(const char* dir, const char* outDir);

	/// <summary>
	/// 从一张图片生成一批图片用于机器学习(包括深度学习)
	/// </summary>
	static void GenerateMLImages();
};

