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

const char* params = "{ help h         |           | Print usage }"
"{ input          | vtest.avi | Path to a video or a sequence of image }"
"{ algo           | MOG2      | Background subtraction method (KNN, MOG2) }";

int main()
{
	//TestImgProcFunctionality::testHoughLine("E:\\Images\\airport\\001.JPG");

	//遍历文件夹
	std::ofstream ofs;
	ofs.open("E:\\Images\\buttons\\images.txt", std::ios::out);

	//
	int index = 1;
	for (int i = 0; i < 4; i++)
	{
		std::string str = "E:\\Images\\buttons\\" + std::to_string(i);
		std::filesystem::path p(str);
		std::filesystem::path dst("E:\\Images\\buttons\\out\\");
		processPath(p, [&](const std::filesystem::directory_entry& entry) {
			//模糊
			//锐化
			//添加噪声
			auto path = entry.path();
			auto dir = path.parent_path();
			auto extension = path.extension();
			auto ext = extension.string();
			if (ext == ".jpg" || ext == ".png" || ext == ".bmp" || ext == ".tif")
			{
				//首先输出原图路径
				//ofs << path.string() << ", " << dir.filename().string() << std::endl;

				//构建输出路径
				std::stringstream ss;
				ss << std::setw(5) << std::setfill('0') << index;
				index++;

				//构建原图
				auto dstTemp = dst;
				//std::filesystem::path out(dst.string() + dir.filename().string() + "_" + ss.str() + path.extension().string());
				std::string a = dir.filename().string() + "_" + ss.str() + path.extension().string();
				auto& out = dstTemp.append(a);
				ofs << out.string() << ", " << dir.filename().string() << std::endl;
				TestImgProcFunctionality::testResize(path.string().c_str(), out.string().c_str());

				auto temp = out;

				//构建模糊图片的路径
				std::string b = out.stem().string() + "_m" + out.extension().string();
				auto& m = temp.replace_filename(b);
				ofs << m.string() << ", " << dir.filename().string() << std::endl;
				TestImgProcFunctionality::testBlur(path.string().c_str(), m.string().c_str());

				std::string b2 = out.stem().string() + "_g" + out.extension().string();
				auto& g = temp.replace_filename(b2);
				ofs << g.string() << ", " << dir.filename().string() << std::endl;
				TestImgProcFunctionality::testGauss(path.string().c_str(), g.string().c_str());

				//构建锐化图片的路径
				std::string c = out.stem().string() + "_r" + out.extension().string();
				auto& r = temp.replace_filename(c);
				ofs << r.string() << ", " << dir.filename().string() << std::endl;
				TestImgProcFunctionality::testSharpen(path.string().c_str(), r.string().c_str());

				//构建噪声图片的路径
				std::string d = out.stem().string() + "_z" + out.extension().string();
				auto& z = temp.replace_filename(d);
				ofs << z.string() << ", " << dir.filename().string() << std::endl;
				TestImgProcFunctionality::testNoise(path.string().c_str(), z.string().c_str());
			}
			else
			{
				//不是图片; 不做处理
			}
			//std::cout << "parent path: " << dir.string() << std::endl;
			//std::cout << "dir name: " << dir.filename().string() << std::endl;
			//std::cout << "extension: " << path.extension().string() << std::endl;
			//std::cout << "file: " << path.string() << std::endl;

			});
	}

	//关闭文本文件
	ofs.close();
	system("pause");
	return 0;
}
