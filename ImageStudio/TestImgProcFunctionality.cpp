#include "TestImgProcFunctionality.h"
#include <vector>
using namespace std;
using namespace cv;

void TestImgProcFunctionality::testThreshold(const char* filepath, const char* outputFolder, const char* outputPrefix)
{
    cv::Mat frame = cv::imread(filepath);
    if (frame.empty())
        return;

    cv::Mat edges;
    cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
    cv::medianBlur(edges, edges, 5);

    cv::Mat back;
    cv::blur(edges, back, cv::Size(255, 255));

    cv::Mat result;
    cv::absdiff(back, edges, result);

    //二值化
    cv::threshold(result, result, 55, 255, cv::THRESH_BINARY);
    char filepath1[1024] = { 0 };
    sprintf_s(filepath1, 1023, "%s/%s_二值化.jpg", outputFolder, outputPrefix);
    cv::imwrite(filepath1, result);

    //形态
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
        cv::Size(7, 7), cv::Point(-1, -1));

    /// 运行指定形态学操作
    morphologyEx(result, result, cv::MORPH_DILATE, element);

    //轮廓
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(result, contours, hierarchy,
        cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0;
    for (; idx >= 0; idx = hierarchy[idx][0])
    {
        auto length = cv::arcLength(contours[idx], true);
        auto area = cv::contourArea(contours[idx]);
        if (area < 100)
            continue;

        cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
        drawContours(frame, contours, idx, color, cv::FILLED, 8, hierarchy);
    }

    char filepath2[1024] = { 0 };
    sprintf_s(filepath2, 1023, "%s/%s_边框.jpg", outputFolder, outputPrefix);
    cv::imwrite(filepath2, frame);
}

void TestImgProcFunctionality::testHoughLine(const char* filename)
{
    // Loads an image
    Mat src = imread(samples::findFile(filename), -1);

    // Check if image is loaded fine
    if (src.empty()) {
        printf(" Error opening image\n");
        return;
    }


    ////
    //std::vector<cv::Mat> vec;
    //cv::split(src, vec);
    //imshow("Blue", vec[0]);
    //imshow("Green", vec[1]);
    //imshow("Red", vec[2]);

    //转化为GRAY
    cvtColor(src, src, COLOR_BGR2GRAY);
    imshow("Source", src);
    imwrite("E:\\Images\\airport\\gray001.JPG", src);

    Mat g;
    cv::threshold(src, g, 75, 255, cv::THRESH_BINARY_INV | THRESH_OTSU);
    imshow("Threshold", g);
    imwrite("E:\\Images\\airport\\threshold001.JPG", g);

    Mat dst;// = g.clone();
    // Edge detection
    Canny(src, dst, 50, 200, 5);
    imwrite("E:\\Images\\airport\\canny001.JPG", dst);

    
    imshow("Canny", dst);
    //waitKey(0);

    Mat cdst, cdstP;
    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();

    // Standard Hough Line Transform
    vector<Vec2f> lines; // will hold the results of the detection
    HoughLines(dst, lines, 1, CV_PI / 180, 250, 0, 0); // runs the actual detection
    // Draw the lines
    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
    }

    // Probabilistic Line Transform
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(dst, linesP, 1, CV_PI / 180, 80, 150, 10); // runs the actual detection
    // Draw the lines
    for (size_t i = 0; i < linesP.size(); i++)
    {
        Vec4i l = linesP[i];
        line(cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
    }

    // Show results
    imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);

    // Wait and Exit
    waitKey();

}

void TestImgProcFunctionality::testDiscreteFourierTransform(const char* filename)
{
    Mat I = imread(samples::findFile(filename), IMREAD_GRAYSCALE);
    if (I.empty()) {
        cout << "Error opening image" << endl;
        return;
    }

    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize(I.rows);
    int n = getOptimalDFTSize(I.cols); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).

    imshow("Input Image", I);    // Show the result
    imshow("spectrum magnitude", magI);
    waitKey();

}

void TestImgProcFunctionality::testResize(const char* filename, const char* outfilename)
{
    //对原图像做一个高斯模糊; 然后输出
    cv::Mat img = cv::imread(filename);
    if (img.empty())
        return;

    cv::Mat out;
    cv::resize(img, out, cv::Size(256, 256));

    cv::imwrite(outfilename, out);
}

void TestImgProcFunctionality::testBlur(const char* filename, const char* outfilename)
{
    //对原图像做一个高斯模糊; 然后输出
    cv::Mat img = cv::imread(filename);
    if (img.empty())
        return;

    cv::Mat dst;
    cv::blur(img, dst, Size(3, 3), Point(-1, -1));

    cv::Mat out;
    cv::resize(dst, out, cv::Size(256, 256));

    cv::imwrite(outfilename, out);
}

void TestImgProcFunctionality::testGauss(const char* filename, const char* outfilename)
{
    //对原图像做一个高斯模糊; 然后输出
    cv::Mat img = cv::imread(filename);
    if (img.empty())
        return;

    cv::Mat dst;
    cv::GaussianBlur(img, dst, Size(3, 3), 11, 11);

    cv::Mat out;
    cv::resize(dst, out, cv::Size(256, 256));

    cv::imwrite(outfilename, out);
}

void TestImgProcFunctionality::testSharpen(const char* filename, const char* outfilename)
{
    //对原图像做一个锐化; 然后输出
    cv::Mat img = cv::imread(filename);
    if (img.empty())
        return;

    cv::Mat kernel1 = (cv::Mat_<float>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
    cv::Mat imgLaplance;
    cv::Mat imgSharpen;
    filter2D(img, imgLaplance, CV_32F, kernel1, Point(-1, -1), 0, BORDER_DEFAULT);
    img.convertTo(imgSharpen, CV_32F);
    cv::Mat imgResult = imgSharpen - imgLaplance;
    imgResult.convertTo(imgResult, CV_8UC3);

    cv::Mat out;
    cv::resize(imgResult, out, cv::Size(256, 256));

    cv::imwrite(outfilename, out);
}

void TestImgProcFunctionality::testNoise(const char* filename, const char* outfilename)
{
    //对原图像做一个添加一个噪声; 然后输出
    cv::Mat img = cv::imread(filename);
    if (img.empty())
        return;

    //
    int n = 128 * 128;
    Mat dstImage = img.clone();
    for (int k = 0; k < n; k++)
    {
        //随机取值行列
        int i = rand() % dstImage.rows;
        int j = rand() % dstImage.cols;

        //图像通道判定
        if (dstImage.channels() == 1)
        {
            dstImage.at<uchar>(i, j) = 255;		//盐噪声
        }
        else
        {
            dstImage.at<Vec3b>(i, j)[0] = 255;
            dstImage.at<Vec3b>(i, j)[1] = 255;
            dstImage.at<Vec3b>(i, j)[2] = 255;
        }
    }
    for (int k = 0; k < n; k++)
    {
        //随机取值行列
        int i = rand() % dstImage.rows;
        int j = rand() % dstImage.cols;
        //图像通道判定
        if (dstImage.channels() == 1)
        {
            dstImage.at<uchar>(i, j) = 0;		//椒噪声
        }
        else
        {
            dstImage.at<Vec3b>(i, j)[0] = 0;
            dstImage.at<Vec3b>(i, j)[1] = 0;
            dstImage.at<Vec3b>(i, j)[2] = 0;
        }
    }

    cv::Mat out;
    cv::resize(dstImage, out, cv::Size(256, 256));

    cv::imwrite(outfilename, out);
}

//生成高斯噪声
double generateGaussianNoise(double mu, double sigma)
{
    //定义小值
    const double epsilon = numeric_limits<double>::min();
    static double z0, z1;
    static bool flag = false;
    flag = !flag;
    //flag为假构造高斯随机变量X
    if (!flag)
        return z1 * sigma + mu;
    double u1, u2;
    //构造随机变量
    do
    {
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    } while (u1 <= epsilon);
    //flag为真构造高斯随机变量
    z0 = sqrt(-2.0 * log(u1)) * cos(2 * CV_PI * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(2 * CV_PI * u2);
    return z0 * sigma + mu;
}

//为图像添加高斯噪声
Mat addGaussianNoise(Mat& srcImag)
{
    Mat dstImage = srcImag.clone();
    int channels = dstImage.channels();
    int rowsNumber = dstImage.rows;
    int colsNumber = dstImage.cols * channels;
    //判断图像的连续性
    if (dstImage.isContinuous())
    {
        colsNumber *= rowsNumber;
        rowsNumber = 1;
    }
    for (int i = 0; i < rowsNumber; i++)
    {
        for (int j = 0; j < colsNumber; j++)
        {
            //添加高斯噪声
            int val = static_cast<int>(dstImage.ptr<uchar>(i)[j] + generateGaussianNoise(2, 0.8) * 32);
            if (val < 0)
                val = 0;
            if (val > 255)
                val = 255;
            dstImage.ptr<uchar>(i)[j] = (uchar)val;
        }
    }
    return dstImage;
}


