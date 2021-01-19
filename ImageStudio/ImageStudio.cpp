///
/// 学习OpenCV，代码来源于网络，书本，C++语言使用C++17
/// 

#include <iostream>
#include <cstdio>
#include "TestVideoAnalysis.h"
#include "TestImgProcFunctionality.h"
#include "FileHelper.h"
#include <iostream>
#include <sstream>
#include "TestPCA.h"

const char* params = "{ help h         |           | Print usage }"
"{ input          | vtest.avi | Path to a video or a sequence of image }"
"{ algo           | MOG2      | Background subtraction method (KNN, MOG2) }";

int main()
{
	TestVideoAnalysis::TestReadVideo("D:\\1.avi");
	return 0;
	//TestImgProcFunctionality::testHoughLine("E:\\Images\\airport\\001.JPG");

	//
	//TestPCA::FirstTest();
	TestPCA::SecondTest();

	system("pause");
	return 0;
}
