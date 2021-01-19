///
/// ͼ����ĸ����㷨����
#pragma once
#include <opencv2/opencv.hpp>

class TestImgProcFunctionality
{
public:
	/// <summary>
	/// ��ֵ��
	/// </summary>
	/// <param name="filepath"></param>
	/// <param name="outputFolder"></param>
	/// <param name="outputPrefix"></param>
	static void testThreshold(const char* filepath, const char* outputFolder, const char* outputPrefix);

	/// <summary>
	/// Houghֱ��
	/// </summary>
	/// <param name="filename"></param>
	static void testHoughLine(const char* filename);

	/// <summary>
	/// ��ɢ����Ҷ�任
	/// </summary>
	static void testDiscreteFourierTransform(const char* filename);

	/// <summary>
	/// ��˹ģ��
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="outfilename"></param>
	static void testResize(const char* filename, const char* outfilename);
	static void testBlur(const char* filename, const char* outfilename);
	static void testGauss(const char* filename, const char* outfilename);
	static void testSharpen(const char* filename, const char* outfilename);
	static void testNoise(const char* filename, const char* outfilename);

	static void testMorph(const char* filepath);

	/// <summary>
	/// ��һ��ͼƬ����һ��ͼƬ���ڻ���ѧϰ(�������ѧϰ)
	/// </summary>
	static void GenerateMLImages();
};

