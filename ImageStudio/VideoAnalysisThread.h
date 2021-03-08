#pragma once
#include <string>
#include <atomic>
#include <opencv2/opencv.hpp>

class VideoAnalysisThread
{
	std::atomic_bool _isExited;
	std::string _imageFolder;
	std::unique_ptr<cv::VideoCapture> _videoCapture;

public:
	VideoAnalysisThread();
	~VideoAnalysisThread();

public:
	bool Start(const char* url, const char* imgFolder);
	bool Stop();

private:
	void RunThread();
};

