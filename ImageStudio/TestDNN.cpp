#include "TestDNN.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn/layer.details.hpp>
#include <opencv2/dnn/all_layers.hpp>
#include "MyCropLayer.h"

void TestDNN::TestHED(const char* filepath)
{
	cv::Mat src = cv::imread(filepath);
	if (src.empty())
		return;

	//统一大小
	cv::Size reso(500, 500);
	cv::resize(src, src, reso);

	//
	//https://blog.csdn.net/matt45m/article/details/104513323
	CV_DNN_REGISTER_LAYER_CLASS(Crop, MyCropLayer);
	cv::Mat img = src.clone();
	double threshold = 2.2;
	cv::Mat blob = cv::dnn::blobFromImage(img, threshold, reso, cv::Scalar(104.00698793, 116.66876762, 122.67891434), false, false);
	cv::dnn::Net net = cv::dnn::readNet("E:/python/hed.prototxt", "E:/python/hed.caffemodel");
	net.setInput(blob);
	cv::Mat out = net.forward();
	cv::resize(out.reshape(1, reso.height), out, img.size());
	cv::Mat out2;
	cv::Mat dst;
	out.convertTo(dst, CV_8UC3, 255);

	//
	cv::namedWindow("原图", 0);
	cv::imshow("原图", src);

	cv::namedWindow("HED", 0);
	cv::imshow("HED", dst);
	cv::waitKey(0);
}
