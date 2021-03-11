///
/// ѧϰOpenCV��������Դ�����磬�鱾��C++����ʹ��C++17
/// 

#include <iostream>
#include <cstdio>
#include "TestVideoAnalysis.h"
#include "TestImgProcFunctionality.h"
#include "FileHelper.h"
#include <iostream>
#include <sstream>
#include "TestPCA.h"
#include <glog/logging.h>
#include "VideoAnalysisThread.h"
#include <conio.h>

const char* params = "{ help h         |           | Print usage }"
"{ input          | vtest.avi | Path to a video or a sequence of image }"
"{ algo           | MOG2      | Background subtraction method (KNN, MOG2) }";

int testVideoAnalysis();

int main(int argc, char** argv)
{
    //
    //FLAGS_log_dir = "./logs/"; //���ļ���һ��Ҫ����
    FLAGS_max_log_size = 4;     //�����־�ļ���С 4M 
    google::InitGoogleLogging("ImageStudio.exe");
    google::SetLogDestination(google::INFO, "./logs/");
    google::SetLogFilenameExtension(".log");

    //����������
    cv::CommandLineParser parser(argc, argv,
        {
            // ��ʽ������  | Ĭ��ֵ | ��ʾ
            /*�����������������ϸ�ڣ�
            1. @��ʾ����˳�������exe����,�������ݳ���˳��ֵ����������
            2. ��@����ʹ��'='��������磺fps=60
            3. ��@��������ʹ��-����--��ͷ���磺 -fps��--fps
            4. ͬһ���������ж�������ƶ�Ӧ���磺help h usage ?���ĸ���ﶼ������ʾ������Ϣ
            */
            "{help h usage ? |      | print this message   }"
            "{@urlfile       |urls.txt | url list   }"
            "{@outdir        |.     | dir of output images  }"
            //"{fps            | -1.0 | fps for output video }"
            //"{N count        |100   | count of objects     }"
            //"{ts timestamp   |      | use time stamp       }"
        });

    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    //int N = parser.get<int>("N");
    //double fps = parser.get<double>("fps");
    //cv::String path = parser.get<cv::String>("path");

    //bool use_time_stamp = parser.has("timestamp");

    cv::String urlfile = parser.get<cv::String>(0);
    cv::String outdir = parser.get<cv::String>(1);
//    int repeat = parser.get<int>(2);

    std::cout << "urlfile:" << urlfile << std::endl;
    std::cout << "outdir:" << outdir << std::endl;
    //std::cout << "repeat:" << repeat << std::endl;
    //std::cout << "use_time_stamp:" << use_time_stamp << std::endl;

    //std::cout << "path:" << path << std::endl;
    //std::cout << "fps:" << fps << std::endl;

    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    //��Ƶ��������
    //testVideoAnalysis();

    //ͼ��ƴ��
    //TestImgProcFunctionality::testStitcher();
    //ˮӡ
    TestImgProcFunctionality::testWatermark("E:/Images/testaa.jpg");
    //
	//auto size = sizeof(bool);
	//std::cout << size << std::endl;
	//TestVideoAnalysis::TestPlateRecognitionVideo("rtsp://admin:hellosar0@192.168.1.108/cam/realmonitor?channel=1&subtype=0");

	//TestVideoAnalysis::TestReadVideo("D:\\1.avi");
	//TestDNN::TestHED("E:/images/1.jpeg");
	//TestImgProcFunctionality::testMorph("E:/images/1.jpeg");
	//TestImgProcFunctionality::throughoutFolder("E:\\Images\\MV", "E:\\Images\\MV_output");
    google::ShutdownGoogleLogging();
    return 0;
	//TestImgProcFunctionality::testHoughLine("E:\\Images\\airport\\001.JPG");

	//
	//TestPCA::FirstTest();
	TestPCA::SecondTest();

	system("pause");
	return 0;
}

int testVideoAnalysis()
{
    //��ȡtxt�ļ��е�url
    int ret = TestVideoAnalysis::InitAI();
    std::cout << "��ʼ��AI��: " << ret << std::endl;
    if (ret < 0)
    {
        std::cout << "��ʼ��AI��ʧ��" << std::endl;
        return 0;
    }
    //
    std::vector<std::shared_ptr<VideoAnalysisThread>> vec;
    for (int i = 0; i < 2; i++)
    {
        std::shared_ptr<VideoAnalysisThread> v = std::make_shared<VideoAnalysisThread>();
        vec.push_back(v);
    }

    for (auto& v : vec)
    {
        v->Start("rtsp://admin:pushon123@192.168.225.114/cam/realmonitor?channel=1&subtype=0", "E:/Images/plates");
    }

    //�ȴ�����
    std::cout << "�밴ESC���˳�����..." << std::endl;
    while (_getch() != 27)//ESC�˳�
    {
        //�ȴ�
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "�û�������ESC��, �˳�����" << std::endl;
    for (auto& v : vec)
    {
        v->Stop();
    }

    TestVideoAnalysis::ReleaseAI();
    return 0;
}
