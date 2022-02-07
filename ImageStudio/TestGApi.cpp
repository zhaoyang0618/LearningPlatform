#include "TestGApi.h"
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/gapi.hpp>
#include <opencv2/gapi/core.hpp>
#include <opencv2/gapi/imgproc.hpp>

std::string image_path = "1.png";
cv::Mat GAPITEST(const cv::Mat& input_frame)
{
    cv::Mat output_frame;

    cv::GMat in;
    cv::GMat vga = cv::gapi::resize(in, cv::Size(), 0.5, 0.5);
    cv::GMat gray = cv::gapi::BGR2Gray(vga);
    cv::GMat blurred = cv::gapi::blur(gray, cv::Size(5, 5));
    cv::GMat out = cv::gapi::Canny(blurred, 32, 128, 3);
    cv::GComputation ac(in, out);

    int64 t0 = cv::getTickCount();
    for (int i = 0; i < 200; i++)
        ac.apply(input_frame, output_frame);
    int64 t1 = cv::getTickCount();
    std::cout << __func__ << "\t seconds:" << (t1 - t0) / cv::getTickFrequency() << std::endl;

    return output_frame;
}

cv::Mat TraditionalTEST(const cv::Mat& input_frame)
{
    cv::Mat output_frame;
    cv::Mat vga;
    cv::Mat gray;
    cv::Mat blurred;

    int64 t0 = cv::getTickCount();
    for (int i = 0; i < 200; i++)
    {
        cv::resize(input_frame, vga, cv::Size(), 0.5, 0.5);
        cv::cvtColor(vga, gray, cv::COLOR_BGR2GRAY);
        cv::blur(gray, blurred, cv::Size(5, 5));
        cv::Canny(blurred, output_frame, 32, 128, 3);
    }
    int64 t1 = cv::getTickCount();
    std::cout << __func__ << "\t seconds:" << (t1 - t0) / cv::getTickFrequency() << std::endl;
    return output_frame;
}

void TestGApi::test1(const std::string &filepath)
{
    cv::VideoCapture cap;
    if (!filepath.empty()) cap.open(filepath.c_str());
    else cap.open(0);
    CV_Assert(cap.isOpened());
    cv::GMat in;
    cv::GMat vga = cv::gapi::resize(in, cv::Size(), 0.5, 0.5);
    cv::GMat gray = cv::gapi::BGR2Gray(vga);
    cv::GMat blurred = cv::gapi::blur(gray, cv::Size(5, 5));
    cv::GMat edges = cv::gapi::Canny(blurred, 32, 128, 3);
    cv::GMat b, g, r;
    std::tie(b, g, r) = cv::gapi::split3(vga);
    cv::GMat out = cv::gapi::merge3(b, g | edges, r);
    cv::GComputation ac(in, out);
    cv::Mat input_frame;
    cv::Mat output_frame;
    CV_Assert(cap.read(input_frame));
    do
    {
        ac.apply(input_frame, output_frame);
        cv::imshow("output", output_frame);
    } while (cap.read(input_frame) && cv::waitKey(30) < 0);
}

void test2()
{
    cv::Mat input_frame = cv::imread(image_path);
    cv::imshow("input_frame", input_frame);
    cv::waitKey(100);
    auto result1 = GAPITEST(input_frame);
    auto result2 = TraditionalTEST(input_frame);
    //check result whether identical or not.
    bool eq = cv::countNonZero(result1 != result2) == 0;
    std::cout << "result equal  " << eq;
}
