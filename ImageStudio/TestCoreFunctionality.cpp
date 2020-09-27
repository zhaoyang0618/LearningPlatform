#include "TestCoreFunctionality.h"
using namespace cv;

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
