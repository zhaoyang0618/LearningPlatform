#include "TestPCA.h"
#include <fstream>

using namespace cv;
using namespace std; 

/// <summary>
/// �����ϵĴ��룻��Ȼ������
/// </summary>
class MyPCA 
{
public:
    vector<Mat> featuresImg;
    vector<Mat> images;
    Mat data;
    int maxComponents;
    int ch;
    int rows;
    string winName;
    Mat result;
    PCA pca;
    int show;

    MyPCA(const string& filename, int show = 0, int maxComponents = 10, const string winName = "Features")
    {
        this->show = show;
        this->maxComponents = maxComponents;
        this->winName = winName;
        try
        {
            read_imgList(filename, images);
        }
        catch (cv::Exception& e)
        {
            cerr << "Error opening file \"" << filename << "\". Reason: " << e.msg << endl;
            exit(1);
        }
    }

    // show the result feature images 
    void showAllFeatures()
    {
        Mat img;
        for (int i = 0; i < result.rows; i++)
        {
            img = result.row(i);
            img = img.reshape(images[0].channels(), images[0].rows);
            featuresImg.push_back(img);
        }
        if (show)
        {
            for (int i = 0; i < featuresImg.size(); i++)
            {
                imshow(winName + (char)i, featuresImg[i]);
            }
        }
    }

    void read_imgList(const string& filename, vector<Mat>& images)
    {
        std::ifstream file(filename.c_str(), ifstream::in);
        if (!file)
        {
            string error_message = "No valid input file was given, please check the given filename.";
            CV_Error(Error::StsBadArg, error_message);
        }

        string line;
        while (getline(file, line))
        {
            images.push_back(imread(line, 0));
        }

        cout << "Read success. " << images.size() << " pictures." << endl;
    }

    Mat formatImagesForPCA(const vector<Mat>& data)
    {
        Mat dst(static_cast<int>(data.size()), data[0].rows * data[0].cols, CV_32F);
        for (unsigned int i = 0; i < data.size(); i++)
        {
            Mat image_row = data[i].clone().reshape(1, 1);
            Mat row_i = dst.row(i);
            image_row.convertTo(row_i, CV_32F);
        }
        return dst;
    }

    Mat toGrayscale(InputArray _src)
    {
        Mat src = _src.getMat();
        // only allow one channel 
        if (src.channels() != 1)
        {
            CV_Error(Error::StsBadArg, "Only Matrices with one channel are supported");
        }
        // create and return normalized image 
        Mat dst;
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        return dst;
    }

    void test()
    {
        // Read in the data. This can fail if not valid 
        // Quit if there are not enough images for this demo. 
        if (images.size() <= 1)
        {
            string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
            CV_Error(Error::StsError, error_message);
        }
        data = formatImagesForPCA(images);
        // Reshape and stack images into a rowMatrix 
        // perform PCA 
        pca = PCA(data, cv::Mat(), PCA::DATA_AS_COL, maxComponents);
        pca.project(data, result);
        showAllFeatures();
        cout << "Data size " << data.size() << endl;
        cout << "Result size " << result.size() << endl;
        cout << "Features size " << images.size() << endl;
    };
};

void TestPCA::FirstTest()
{
    string filename = "num.dat"; 
    MyPCA pca(filename, 1);
    pca.test(); 
    int key = 0; 
    while (key != 'q') 
        key = waitKey(); 
}

/// <summary>
/// ������������
/// https://blog.csdn.net/zmdsjtu/article/details/77932176?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_title-3&spm=1001.2101.3001.4242
/// </summary>
void TestPCA::SecondTest()
{
    Mat frame = imread("E:/pic/test.jpg");

    cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
    //	double ��ֵ = 99.999999999999 / 100.0;
    //	PCA pca(frame, Mat(), CV_PCA_DATA_AS_COL, ��ֵ);
    /*
    #define CV_PCA_DATA_AS_ROW 0
    #define CV_PCA_DATA_AS_COL 1
    #define CV_PCA_USE_AVG 2
    */
    int layers = 120;
    PCA pca(frame, Mat(), PCA::DATA_AS_COL, layers);
    //ͼƬ��СΪ400*362
    //���ﰴCOL�ķ�ʽ��ά����֤�������㣬�������͵�120��
    //���Կ��Է��ִ�ӡ�ľ�ֵ�Ĺ��Ϊ1*362
    cout << "��ֵ�Ĺ��" << pca.mean.size() << endl;//��ֵ
    cout << "����ֵ�Ĺ��" << pca.eigenvalues.size() << endl;//����ֵ
    cout << "���������Ĺ��" << pca.eigenvectors.size() << endl;//��������

    Mat dst = pca.project(frame);//ӳ���¿ռ�
                                 //cout << dst;
    Mat src = pca.backProject(dst);//��ӳ�����
                                   //	cout << src;

    imshow("ԭͼ", frame);
    imshow("PCA��ά��", dst);
    imshow("��ӳ��", src);
}

