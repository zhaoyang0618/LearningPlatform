#include "VideoAnalysisThread.h"
#include <glog/logging.h>
#include <thread>
#include <chrono>
#include "PlateReconize.h"
#include "../Common/TimeLost.h"

VideoAnalysisThread::VideoAnalysisThread()
	: _isExited(false)
	, _videoCapture(nullptr)
{
}

VideoAnalysisThread::~VideoAnalysisThread()
{
	Stop();
}

bool VideoAnalysisThread::Start(const char* url, const char* imgFolder)
{
	//首先退出
	Stop();

	//
	_isExited = false;
	_videoCapture = std::make_unique<cv::VideoCapture>(url);
	_imageFolder = imgFolder;
	if (_videoCapture == nullptr)
		return false;
	//
	if (!_videoCapture->isOpened())
		return false;

	//然后开启新的线程
	auto t = std::thread(&VideoAnalysisThread::RunThread, this);
	t.detach();

	return true;
}

bool VideoAnalysisThread::Stop()
{
	_isExited = true;
	//等待
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	return true;
}


void VideoAnalysisThread::RunThread()
{
	if (_videoCapture == nullptr)
		return;

	LOG(INFO) << "线程开始: " << std::this_thread::get_id() << std::endl;
	int interval = 25;
	int framenum = 0;
	cv::Mat frame;
	while (true)
	{
		if (_isExited)
		{
			LOG(INFO) << "线程退出: " << std::this_thread::get_id() << std::endl;
			break;
		}

		//读取视频
		if (!_videoCapture->read(frame))
		{
			//读取失败
			LOG(ERROR) << "视频读取失败" << std::this_thread::get_id() << std::endl;
			break;
		}

		framenum++;
		if (frame.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			continue;
		}

		if (framenum % interval == 1)
		{
			//不需要每一帧都分析
			LOG(INFO) << "开始分析: " << std::this_thread::get_id() << std::endl;

			try
			{
				char outfile[1024] = { 0 };
				sprintf_s(outfile, 1023, "%s/%08d.jpg", _imageFolder.c_str(), framenum);
				//cv::imwrite(outfile, frame);
				TimeLost tl;

				MNPlateInfoStruct	info;
				int cnt = PLATE_ForwardmnRecoMat((unsigned char*)frame.data, frame.cols, frame.rows, info);
				//int cnt = PLATE_ForwardmnReco(outfile, info);
				if (cnt >= 7)
				{
					// display result;
					LOG(INFO) << "车牌: " << info.plate_code << "; X=" << info.x << "; Y=" << info.y << "; W=" << info.w << "; H=" << info.h << std::endl;
					cv::rectangle(frame, cv::Rect(info.x, info.y, info.w, info.h), cv::Scalar(0, 0, 255));
					//char outfile[1024] = { 0 };
					//sprintf_s(outfile, 1023, "%s/%08d.jpg", _imageFolder.c_str(), framenum);
					cv::imwrite(outfile, frame);
				}
				
				LOG(INFO) << "花费时间: " << tl.elspsed() << " microseconds; "  << std::this_thread::get_id() << std::endl;
			}
			catch (std::exception& exp)
			{
				LOG(ERROR) << exp.what() << std::this_thread::get_id() << std::endl;
			}
			catch (...)
			{
				LOG(ERROR) << "未知错误: " << std::this_thread::get_id() << std::endl;
			}

			LOG(INFO) << "结束分析: " << std::this_thread::get_id() << std::endl;
		}

		//休息一下
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
