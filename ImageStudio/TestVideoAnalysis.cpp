#include "TestVideoAnalysis.h"
#include <iostream>
#include <sstream>
#include <glog/logging.h>
#include "PlateReconize.h"

using namespace std;
using namespace cv;

void TestVideoAnalysis::TestBackgroundSubtraction(int type, const char* filepath)
{
	//
    Ptr<BackgroundSubtractor> pBackSub;
    if (type == 0)
        pBackSub = createBackgroundSubtractorMOG2();
    else
        pBackSub = createBackgroundSubtractorKNN();

    VideoCapture capture(samples::findFile(filepath));
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open: " << filepath << endl;
        return;
    }

    Mat frame, fgMask;
    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        //update the background model
        pBackSub->apply(frame, fgMask);

        //get the frame number and write it on the current frame
        rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
            cv::Scalar(255, 255, 255), -1);

        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
            FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask", fgMask);

        //get the input from the keyboard
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }

}

void TestVideoAnalysis::TestMeanShift(const char* filename)
{
    VideoCapture capture(filename);
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return;
    }

    Mat frame, roi, hsv_roi, mask;
    // take first frame of the video
    capture >> frame;

    // setup initial location of window
    Rect track_window(300, 200, 100, 50); // simply hardcoded the values

    // set up the ROI for tracking
    roi = frame(track_window);
    cvtColor(roi, hsv_roi, COLOR_BGR2HSV);
    inRange(hsv_roi, Scalar(0, 60, 32), Scalar(180, 255, 255), mask);

    float range_[] = { 0, 180 };
    const float* range[] = { range_ };
    Mat roi_hist;
    int histSize[] = { 180 };
    int channels[] = { 0 };
    calcHist(&hsv_roi, 1, channels, mask, roi_hist, 1, histSize, range);
    normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);

    // Setup the termination criteria, either 10 iteration or move by atleast 1 pt
    TermCriteria term_crit(TermCriteria::EPS | TermCriteria::COUNT, 10, 1);

    while (true) {
        Mat hsv, dst;
        capture >> frame;
        if (frame.empty())
            break;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        calcBackProject(&hsv, 1, channels, roi_hist, dst, range);

        // apply meanshift to get the new location
        meanShift(dst, track_window, term_crit);

        // Draw it on image
        rectangle(frame, track_window, 255, 2);
        imshow("img2", frame);

        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }

}

void TestVideoAnalysis::TestCamShift(const char* filename)
{
    VideoCapture capture(filename);
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return;
    }

    Mat frame, roi, hsv_roi, mask;
    // take first frame of the video
    capture >> frame;

    // setup initial location of window
    Rect track_window(300, 200, 100, 50); // simply hardcoded the values

    // set up the ROI for tracking
    roi = frame(track_window);
    cvtColor(roi, hsv_roi, COLOR_BGR2HSV);
    inRange(hsv_roi, Scalar(0, 60, 32), Scalar(180, 255, 255), mask);

    float range_[] = { 0, 180 };
    const float* range[] = { range_ };
    Mat roi_hist;
    int histSize[] = { 180 };
    int channels[] = { 0 };
    calcHist(&hsv_roi, 1, channels, mask, roi_hist, 1, histSize, range);
    normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);

    // Setup the termination criteria, either 10 iteration or move by atleast 1 pt
    TermCriteria term_crit(TermCriteria::EPS | TermCriteria::COUNT, 10, 1);

    while (true) {
        Mat hsv, dst;
        capture >> frame;
        if (frame.empty())
            break;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        calcBackProject(&hsv, 1, channels, roi_hist, dst, range);

        // apply camshift to get the new location
        RotatedRect rot_rect = CamShift(dst, track_window, term_crit);

        // Draw it on image
        Point2f points[4];
        rot_rect.points(points);
        for (int i = 0; i < 4; i++)
            line(frame, points[i], points[(i + 1) % 4], 255, 2);
        imshow("img2", frame);


        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }

}

void TestVideoAnalysis::TestOpticalFlow(const char* filename)
{
    VideoCapture capture(filename);
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return ;
    }

    // Create some random colors
    vector<Scalar> colors;
    RNG rng;
    for (int i = 0; i < 100; i++)
    {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r, g, b));
    }

    Mat old_frame, old_gray;
    vector<Point2f> p0, p1;

    // Take first frame and find corners in it
    capture >> old_frame;
    cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);
    goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);

    // Create a mask image for drawing purposes
    Mat mask = Mat::zeros(old_frame.size(), old_frame.type());

    while (true) {
        Mat frame, frame_gray;

        capture >> frame;
        if (frame.empty())
            break;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

        // calculate optical flow
        vector<uchar> status;
        vector<float> err;
        TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
        calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, Size(15, 15), 2, criteria);

        vector<Point2f> good_new;
        for (uint i = 0; i < p0.size(); i++)
        {
            // Select good points
            if (status[i] == 1) {
                good_new.push_back(p1[i]);
                // draw the tracks
                line(mask, p1[i], p0[i], colors[i], 2);
                circle(frame, p1[i], 5, colors[i], -1);
            }
        }
        Mat img;
        add(frame, mask, img);

        imshow("Frame", img);

        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;

        // Now update the previous frame and previous points
        old_gray = frame_gray.clone();
        p0 = good_new;
    }

}

void TestVideoAnalysis::TestDenseOpticalFlow(const char* filename)
{
    VideoCapture capture(filename);
    if (!capture.isOpened()) {
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return;
    }

    Mat frame1, prvs;
    capture >> frame1;
    cvtColor(frame1, prvs, COLOR_BGR2GRAY);

    while (true) {
        Mat frame2, next;
        capture >> frame2;
        if (frame2.empty())
            break;
        cvtColor(frame2, next, COLOR_BGR2GRAY);

        Mat flow(prvs.size(), CV_32FC2);
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

        // visualization
        Mat flow_parts[2];
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
        angle *= ((1.f / 360.f) * (180.f / 255.f));

        //build hsv image
        Mat _hsv[3], hsv, hsv8, bgr;
        _hsv[0] = angle;
        _hsv[1] = Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magn_norm;
        merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
        cvtColor(hsv8, bgr, COLOR_HSV2BGR);

        imshow("frame2", bgr);

        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;

        prvs = next;
    }
}

void TestVideoAnalysis::TestReadVideo(const char* filename)
{
    cv::VideoCapture capture;
    bool ret = false;
    if (filename == nullptr)
    {
        ret = capture.open(0);
    }
    else
    {
        ret = capture.open(filename);
    }

    if (!ret)
    {
        std::cout << "Read video failed!" << std::endl;
        return;
    }
    if (!capture.isOpened())
    {
        std::cout << "Read video failed!" << std::endl;
        return ;
    }

    std::cout << "begin read!" << std::endl;
    cv::Mat frame;
    //int m_frameNum = capture.get(cv::CAP_PROP_FRAME_COUNT);
    bool stop = false;
    int count = 0;
    while (!stop)
    {
        if (!capture.read(frame)) //当视频帧数很多，循环读数据 会导致内存泄漏  
        {
            std::cout << "Read frame failed!" << std::endl;
            cv::waitKey(30);
            continue;
        }

        auto a = cv::waitKey(30);
        std::cout << a << std::endl;
        if (a == 27)
            break;
        count++;
    }
    std::cout << "stopped!" << std::endl;
}

int TestVideoAnalysis::InitAI()
{
    int ret = PLATE_InitmnReco(
        "obj_plate_region1226.cfg", 
        "obj_plate_region1226_last.weights", 
        "obj_plate_1225.cfg", 
        "obj_plate_1225_last.weights");
    return ret;
}

int TestVideoAnalysis::ReleaseAI()
{
    int ret = PLATE_DisposemnReco();
    return ret;
}

void TestVideoAnalysis::TestPlateRecognitionVideo(const char* filename)
{
    VideoCapture	cap;
    cap.open(filename);
    if (!cap.isOpened())
    {
        LOG(INFO) << "打开url失败: " << filename << std::endl;
        return;
    }

    int interval = 5;
    int framenum = 0;
    Mat frame;
    while (cap.read(frame)) {
        framenum++;
        if (frame.empty())
        {
            //休息20毫秒
            continue;
        }

        // reco every 5 frames
        if (framenum % interval == 1)
        {
            MNPlateInfoStruct	info;
            int cnt = PLATE_ForwardmnRecoMat((unsigned char*)frame.data, frame.cols, frame.rows, info);
            if (cnt >= 7)
            {
                // display result;
                LOG(INFO) << "车牌: " << info.plate_code << "; X=" << info.x << "; Y=" << info.y << "; W=" << info.w << "; H=" << info.h << std::endl;
                cv::rectangle(frame, cv::Rect(info.x, info.y, info.w, info.h), cv::Scalar(0, 0, 255));
                char outfile[1024] = { 0 };
                sprintf_s(outfile, 1023, "D:/images/AI/%08d.jpg", framenum);
                cv::imwrite(outfile, frame);
            }
        }

        //imshow("video", frame);
        //休息10毫秒

        waitKey(10);
    }
}
