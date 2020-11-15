#include "TestCoreFunctionality.h"
using namespace cv;
using namespace std;

/// <summary>
/// 一般来说应该使用这个，性能不错
/// </summary>
/// <param name="src"></param>
/// <param name="table"></param>
void TestCoreFunctionality::TestScanImage1(cv::Mat src, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(src.depth() == CV_8U);

    int channels = src.channels();

    int nRows = src.rows;
    int nCols = src.cols * channels;

    if (src.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    int i, j;
    uchar* p;
    for (i = 0; i < nRows; ++i)
    {
        p = src.ptr<uchar>(i);
        for (j = 0; j < nCols; ++j)
        {
            p[j] = table[p[j]];
        }
    }

    //return src;
}

/// <summary>
/// 性能较差，但是安全性比较好, 不需要考虑性能的话，尽量使用这个，容易理解
/// </summary>
/// <param name="src"></param>
/// <param name="table"></param>
void TestCoreFunctionality::TestScanImage2(cv::Mat src, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(src.depth() == CV_8U);

    const int channels = src.channels();
    switch (channels)
    {
    case 1:
    {
        MatIterator_<uchar> it, end;
        for (it = src.begin<uchar>(), end = src.end<uchar>(); it != end; ++it)
            *it = table[*it];
        break;
    }
    case 3:
    {
        MatIterator_<Vec3b> it, end;
        for (it = src.begin<Vec3b>(), end = src.end<Vec3b>(); it != end; ++it)
        {
            (*it)[0] = table[(*it)[0]];
            (*it)[1] = table[(*it)[1]];
            (*it)[2] = table[(*it)[2]];
        }
    }
    }

}

void TestCoreFunctionality::TestScanImage3(cv::Mat src, const uchar* const table)
{

    // accept only char type matrices
    CV_Assert(src.depth() == CV_8U);

    const int channels = src.channels();
    switch (channels)
    {
    case 1:
    {
        for (int i = 0; i < src.rows; ++i)
            for (int j = 0; j < src.cols; ++j)
                src.at<uchar>(i, j) = table[src.at<uchar>(i, j)];
        break;
    }
    case 3:
    {
        Mat_<Vec3b> _I = src;

        for (int i = 0; i < src.rows; ++i)
            for (int j = 0; j < src.cols; ++j)
            {
                _I(i, j)[0] = table[_I(i, j)[0]];
                _I(i, j)[1] = table[_I(i, j)[1]];
                _I(i, j)[2] = table[_I(i, j)[2]];
            }
        src = _I;
        break;
    }
    }

}

/// <summary>
/// 使用lookup table, 这个性能比较好
/// </summary>
/// <param name="src"></param>
/// <param name="table"></param>
void TestCoreFunctionality::TestScanImage4(cv::Mat src, const uchar* const table)
{
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
        p[i] = table[i];

    //
    cv::Mat J;
    LUT(src, lookUpTable, J);
}

/// <summary>
/// 锐化
/// </summary>
/// <param name="myImage"></param>
/// <param name="Result"></param>
void TestCoreFunctionality::Sharpen(const cv::Mat& myImage, cv::Mat& Result)
{
    //
    CV_Assert(myImage.depth() == CV_8U);  // accept only uchar images

    const int nChannels = myImage.channels();
    Result.create(myImage.size(), myImage.type());

    for (int j = 1; j < myImage.rows - 1; ++j)
    {
        const uchar* previous = myImage.ptr<uchar>(j - 1);
        const uchar* current = myImage.ptr<uchar>(j);
        const uchar* next = myImage.ptr<uchar>(j + 1);

        uchar* output = Result.ptr<uchar>(j);

        for (int i = nChannels; i < nChannels * (myImage.cols - 1); ++i)
        {
            *output++ = saturate_cast<uchar>(5 * current[i]
                - current[i - nChannels] - current[i + nChannels] - previous[i] - next[i]);
        }
    }

    Result.row(0).setTo(Scalar(0));
    Result.row(Result.rows - 1).setTo(Scalar(0));
    Result.col(0).setTo(Scalar(0));
    Result.col(Result.cols - 1).setTo(Scalar(0));
}

/// <summary>
/// 
/// </summary>
void TestCoreFunctionality::TestFilter2DAndSharpen(cv::Mat& src)
{
    cv::Mat dst0, dst1;

    namedWindow("Input", WINDOW_AUTOSIZE);
    namedWindow("Output", WINDOW_AUTOSIZE);

    imshow("Input", src);
    double t = (double)getTickCount();

    Sharpen(src, dst0);

    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "Hand written function time passed in seconds: " << t << endl;

    imshow("Output", dst0);
    waitKey();

    Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0,
        -1, 5, -1,
        0, -1, 0);

    t = (double)getTickCount();

    filter2D(src, dst1, src.depth(), kernel);
    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "Built-in filter2D time passed in seconds:     " << t << endl;

    imshow("Output", dst1);

    waitKey();

}

void TestCoreFunctionality::TestBasicOperations()
{
    //输入输出(从文件读取；写入文件)
    std::string filename = "";
    //默认读取3通道
    Mat img = imread(filename);
    //读取灰度图
    //Mat img = imread(filename, IMREAD_GRAYSCALE);
    //写入文件
    imwrite(filename, img);
    //内存操作
    //Use cv::imdecode and cv::imencode to read and write an image from/to memory rather than a file.

    //访问图像数据
    //此时需要知道通道数
    //In order to get pixel intensity value, you have to know the type of an image and the number of channels. 
    //Here is an example for a single channel grey scale image (type 8UC1) and pixel coordinates x and y:
    int x = 0, y = 0;
    Scalar intensity = img.at<uchar>(y, x);
    //C++ version only : intensity.val[0] contains a value from 0 to 255. Note the ordering of x and y.
    //Since in OpenCV images are represented by the same structure as matrices, we use the same convention 
    //for both cases - the 0 - based row index(or y - coordinate) goes first and the 0 - based column index(or x - coordinate) follows it.
    //Alternatively, you can use the following notation(C++ only) :
    //Scalar intensity = img.at<uchar>(Point(x, y));

    //Now let us consider a 3 channel image with BGR color ordering (the default format returned by imread):
    //Vec3b intensity = img.at<Vec3b>(y, x);
    //uchar blue = intensity.val[0];
    //uchar green = intensity.val[1];
    //uchar red = intensity.val[2];

    //You can use the same method for floating - point images(for example, you can get such an image by running Sobel on a 3 channel image) (C++ only) :
    //Vec3f intensity = img.at<Vec3f>(y, x);
    //float blue = intensity.val[0];
    //float green = intensity.val[1];
    //float red = intensity.val[2];

    //也可以用于赋值
    img.at<uchar>(y, x) = 128;

    //There are functions in OpenCV, especially from calib3d module, such as cv::projectPoints, that take an array of 2D or 3D points in the form of Mat.Matrix should contain exactly one column, each row corresponds to a point, matrix type should be 32FC2 or 32FC3 correspondingly.Such a matrix can be easily constructed from std::vector(C++ only) :
    //vector<Point2f> points;
    ////... fill the array
    //Mat pointsMat = Mat(points);

    //One can access a point in this matrix using the same method Mat::at(C++ only) :
    //Point2f point = pointsMat.at<Point2f>(i, 0);

    //Mat中只包含图像数据data的指针，一般情况下不会复制这一部分数据
    //Mat is a structure that keeps matrix / image characteristics(rows and columns number, data type etc) and a pointer to data.
    //So nothing prevents us from having several instances of Mat corresponding to the same data.
    //A Mat keeps a reference count that tells if data has to be deallocated when a particular instance of Mat is destroyed.
    //Here is an example of creating two matrices without copying data(C++ only) :
    //std::vector<Point3f> points;
    //// .. fill the array
    //Mat pointsMat = Mat(points).reshape(1);

    //复制数据需要使用copyTo或clone函数
    //As a result, we get a 32FC1 matrix with 3 columns instead of 32FC3 matrix with 1 column.
    //pointsMat uses data from points and will not deallocate the memory when destroyed.
    //In this particular instance, however, developer has to make sure that lifetime of points is longer than of pointsMat 
    //If we need to copy the data, this is done using, for example, cv::Mat::copyTo or cv::Mat::clone:
    //Mat img = imread("image.jpg");
    //Mat img1 = img.clone();

    //An empty output Mat can be supplied to each function.Each implementation calls Mat::create for a destination matrix.
    //This method allocates data for a matrix if it is empty.If it is not empty and has the correct sizeand type, the method does nothing.
    //If however, size or type are different from the input arguments, the data is deallocated(and lost) and a new data is allocated.
    //For example :
    //Mat img = imread("image.jpg");
    //Mat sobelx;
    //Sobel(img, sobelx, CV_32F, 1, 0);

    //There is a number of convenient operators defined on a matrix.For example, here is how we can make a black image from an existing greyscale image img
    //img = Scalar(0);
    //Selecting a region of interest :
    //Rect r(10, 10, 100, 100);
    //Mat smallImg = img(r);

    //Conversion from color to greyscale :

    //Mat img = imread("image.jpg"); // loading a 8UC3 image
    //Mat grey;
    //cvtColor(img, grey, COLOR_BGR2GRAY);

    //Change image type from 8UC1 to 32FC1:
    //src.convertTo(dst, CV_32F);

    //显示图像
    //It is very useful to see intermediate results of your algorithm during development process.
    //OpenCV provides a convenient way of visualizing images.A 8U image can be shown using:

    //Mat img = imread("image.jpg");
    //namedWindow("image", WINDOW_AUTOSIZE);
    //imshow("image", img);
    //waitKey();

    //A call to waitKey() starts a message passing cycle that waits for a key stroke in the "image" window.
    //A 32F image needs to be converted to 8U type.For example :
    //32F的图像不能直接显示，需要转换
    //Mat img = imread("image.jpg");
    //Mat grey;
    //cvtColor(img, grey, COLOR_BGR2GRAY);
    //Mat sobelx;
    //Sobel(grey, sobelx, CV_32F, 1, 0);
    //double minVal, maxVal;
    //minMaxLoc(sobelx, &minVal, &maxVal); //find minimum and maximum intensities
    //Mat draw;
    //sobelx.convertTo(draw, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
    //namedWindow("image", WINDOW_AUTOSIZE);
    //imshow("image", draw);
    //waitKey();

    //Adding(blending) two images using OpenCV
    double alpha = 0.2;
    auto beta = (1.0 - alpha);
    auto src1 = imread(samples::findFile("LinuxLogo.jpg"));
    auto src2 = imread(samples::findFile("WindowsLogo.jpg"));
    cv::Mat dst;
    addWeighted(src1, alpha, src2, beta, 0.0, dst);


}

