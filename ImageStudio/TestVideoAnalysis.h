#pragma once
#include <opencv2/opencv.hpp>

class TestVideoAnalysis
{
public:
	static void TestBackgroundSubtraction(int type, const char* filepath);
	static void TestMeanShift(const char* filename);
	static void TestCamShift(const char* filename);
	static void TestOpticalFlow(const char* filename);
	static void TestDenseOpticalFlow(const char* filename);
	static void TestReadVideo(const char* filename);
	static int InitAI();
	static int ReleaseAI();
	static void TestPlateRecognitionVideo(const char* filename);
};

