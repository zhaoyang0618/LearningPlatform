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
	//�����˳�
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

	//Ȼ�����µ��߳�
	auto t = std::thread(&VideoAnalysisThread::RunThread, this);
	t.detach();

	return true;
}

bool VideoAnalysisThread::Stop()
{
	_isExited = true;
	//�ȴ�
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	return true;
}


void VideoAnalysisThread::RunThread()
{
	if (_videoCapture == nullptr)
		return;

	LOG(INFO) << "�߳̿�ʼ: " << std::this_thread::get_id() << std::endl;
	int interval = 25;
	int framenum = 0;
	cv::Mat frame;
	while (true)
	{
		if (_isExited)
		{
			LOG(INFO) << "�߳��˳�: " << std::this_thread::get_id() << std::endl;
			break;
		}

		//��ȡ��Ƶ
		if (!_videoCapture->read(frame))
		{
			//��ȡʧ��
			LOG(ERROR) << "��Ƶ��ȡʧ��" << std::this_thread::get_id() << std::endl;
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
			//����Ҫÿһ֡������
			LOG(INFO) << "��ʼ����: " << std::this_thread::get_id() << std::endl;

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
					LOG(INFO) << "����: " << info.plate_code << "; X=" << info.x << "; Y=" << info.y << "; W=" << info.w << "; H=" << info.h << std::endl;
					cv::rectangle(frame, cv::Rect(info.x, info.y, info.w, info.h), cv::Scalar(0, 0, 255));
					//char outfile[1024] = { 0 };
					//sprintf_s(outfile, 1023, "%s/%08d.jpg", _imageFolder.c_str(), framenum);
					cv::imwrite(outfile, frame);
				}
				
				LOG(INFO) << "����ʱ��: " << tl.elspsed() << " microseconds; "  << std::this_thread::get_id() << std::endl;
			}
			catch (std::exception& exp)
			{
				LOG(ERROR) << exp.what() << std::this_thread::get_id() << std::endl;
			}
			catch (...)
			{
				LOG(ERROR) << "δ֪����: " << std::this_thread::get_id() << std::endl;
			}

			LOG(INFO) << "��������: " << std::this_thread::get_id() << std::endl;
		}

		//��Ϣһ��
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
