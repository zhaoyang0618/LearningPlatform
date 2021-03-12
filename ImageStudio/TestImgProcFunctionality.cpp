#include "TestImgProcFunctionality.h"
#include "FileHelper.h"
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

void show_wait_destroy(const char* winname, cv::Mat img) {
    char file[1024] = { 0 };
    sprintf_s(file, 1024, "E:\\Images\\airport\\%s.jpg", winname);
    cv::imwrite(file, img);
    cv::imshow(winname, img);
    cv::moveWindow(winname, 500, 0);
    cv::waitKey(0);
    cv::destroyWindow(winname);
}

/// <summary>
/// https://docs.opencv.org/master/dd/dd7/tutorial_morph_lines_detection.html
/// </summary>
void TestImgProcFunctionality::testMorph(const char* filepath)
{
    Mat src = imread(filepath, IMREAD_COLOR);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return;
    }

    // Show source image
    imshow("src", src);
    findHorizontalAndVerticalLine(src, show_wait_destroy);
    //// Transform source image to gray if it is not already
    //Mat gray;
    //if (src.channels() == 3)
    //{
    //    cvtColor(src, gray, COLOR_BGR2GRAY);
    //}
    //else
    //{
    //    gray = src;
    //}
    //// Show gray image
    //show_wait_destroy("gray", gray);
    //// Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
    //Mat bw;
    //adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
    //// Show binary image
    //show_wait_destroy("binary", bw);
    //// Create the images that will use to extract the horizontal and vertical lines
    //Mat horizontal = bw.clone();
    //Mat vertical = bw.clone();
    //// Specify size on horizontal axis
    //int horizontal_size = horizontal.cols / 30;
    //// Create structure element for extracting horizontal lines through morphology operations
    //Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1));
    //// Apply morphology operations
    //erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    //dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    //// Show extracted horizontal lines
    //show_wait_destroy("horizontal", horizontal);
    //// Specify size on vertical axis
    //int vertical_size = vertical.rows / 30;
    //// Create structure element for extracting vertical lines through morphology operations
    //Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, vertical_size));
    //// Apply morphology operations
    //erode(vertical, vertical, verticalStructure, Point(-1, -1));
    //dilate(vertical, vertical, verticalStructure, Point(-1, -1));
    //// Show extracted vertical lines
    //show_wait_destroy("vertical", vertical);
    //// Inverse vertical image
    //bitwise_not(vertical, vertical);
    //show_wait_destroy("vertical_bit", vertical);
    //// Extract edges and smooth image according to the logic
    //// 1. extract edges
    //// 2. dilate(edges)
    //// 3. src.copyTo(smooth)
    //// 4. blur smooth img
    //// 5. smooth.copyTo(src, edges)
    //// Step 1
    //Mat edges;
    //adaptiveThreshold(vertical, edges, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
    //show_wait_destroy("edges", edges);
    //// Step 2
    //Mat kernel = Mat::ones(2, 2, CV_8UC1);
    //dilate(edges, edges, kernel);
    //show_wait_destroy("dilate", edges);
    //// Step 3
    //Mat smooth;
    //vertical.copyTo(smooth);
    //// Step 4
    //blur(smooth, smooth, Size(2, 2));
    //// Step 5
    //smooth.copyTo(vertical, edges);
    //// Show final result
    //show_wait_destroy("smooth - final", vertical);
}

template<typename FUNC>
void TestImgProcFunctionality::findHorizontalAndVerticalLine(cv::Mat& src, FUNC output)
{
    // Transform source image to gray if it is not already
    Mat gray;
    if (src.channels() == 3)
    {
        cvtColor(src, gray, COLOR_BGR2GRAY);
    }
    else
    {
        gray = src;
    }

    // Show gray image
    output("1_gray", gray);
    // Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
    Mat bw;
    adaptiveThreshold(~gray, bw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
    // Show binary image
    output("2_binary", bw);
    // Create the images that will use to extract the horizontal and vertical lines
    Mat horizontal = bw.clone();
    Mat vertical = bw.clone();
    // Specify size on horizontal axis
    int horizontal_size = horizontal.cols / 30;
    // Create structure element for extracting horizontal lines through morphology operations
    Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1));
    // Apply morphology operations
    erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
    // Show extracted horizontal lines
    output("3_horizontal", horizontal);
    // Specify size on vertical axis
    int vertical_size = vertical.rows / 30;
    // Create structure element for extracting vertical lines through morphology operations
    Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, vertical_size));
    // Apply morphology operations
    erode(vertical, vertical, verticalStructure, Point(-1, -1));
    dilate(vertical, vertical, verticalStructure, Point(-1, -1));
    // Show extracted vertical lines
    output("4_vertical", vertical);
    // Inverse vertical image
    bitwise_not(vertical, vertical);
    output("5_vertical_bit", vertical);
    // Extract edges and smooth image according to the logic
    // 1. extract edges
    // 2. dilate(edges)
    // 3. src.copyTo(smooth)
    // 4. blur smooth img
    // 5. smooth.copyTo(src, edges)
    // Step 1
    Mat edges;
    adaptiveThreshold(vertical, edges, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
    output("6_edges", edges);
    // Step 2
    Mat kernel = Mat::ones(2, 2, CV_8UC1);
    dilate(edges, edges, kernel);
    output("7_dilate", edges);
    // Step 3
    Mat smooth;
    vertical.copyTo(smooth);
    // Step 4
    blur(smooth, smooth, Size(2, 2));
    // Step 5
    smooth.copyTo(vertical, edges);
    // Show final result
    output("8_smooth - final", vertical);
}

void TestImgProcFunctionality::findHorizontalAndVerticalLine(const char* filepath, const char* outDir, int index)
{
    //读取文件
    cv::Mat src = cv::imread(filepath, IMREAD_COLOR);
    if (src.empty())
        return;
    findHorizontalAndVerticalLine(src, 
        [&](const char* name, cv::Mat& img) {
            //构建输出路径
            std::filesystem::path path(filepath);
            auto extension = path.extension();

            std::stringstream ss;
            ss << name << "_" << std::setw(5) << std::setfill('0') << index;

            //需要输出文件夹
            std::filesystem::path dst(outDir);
            std::string filename = ss.str() + path.extension().string();
            auto& out = dst.append(filename);
            std::string outfilepath = out.string();
            cv::imwrite(outfilepath, img);
        });
}

void TestImgProcFunctionality::throughoutFolder(const char* dir, const char* outDir)
{
    std::string str(dir);
    std::filesystem::path p(str);
    int index = 0;
    processPath(p, [&](const std::filesystem::directory_entry& entry) {
        auto path = entry.path();
        auto dir = path.parent_path();
        auto extension = path.extension();
        auto ext = extension.string();
        if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp" || ext == ".tif" || ext == ".tiff")
        {
            findHorizontalAndVerticalLine(path.string().c_str(), outDir, index);
            index++;
        }
        else
        {
            //不是图片; 不做处理
        }
        });
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

/// <summary>
/// 代码来源
/// https://www.sohu.com/a/342264044_100007727
/// </summary>
void TestImgProcFunctionality::GenerateMLImages()
{
    //遍历文件夹
    std::ofstream ofs;
    ofs.open("E:\\Images\\buttons\\images.txt", std::ios::out);

    //
    int index = 1;
    for (int i = 0; i < 4; i++)
    {
    	std::string str = "E:\\Images\\buttons\\" + std::to_string(i);
    	std::filesystem::path p(str);
    	std::filesystem::path dst("E:\\Images\\buttons\\out\\");
    	processPath(p, [&](const std::filesystem::directory_entry& entry) {
    		//模糊
    		//锐化
    		//添加噪声
    		auto path = entry.path();
    		auto dir = path.parent_path();
    		auto extension = path.extension();
    		auto ext = extension.string();
    		if (ext == ".jpg" || ext == ".png" || ext == ".bmp" || ext == ".tif")
    		{
    			//首先输出原图路径
    			//ofs << path.string() << ", " << dir.filename().string() << std::endl;

    			//构建输出路径
    			std::stringstream ss;
    			ss << std::setw(5) << std::setfill('0') << index;
    			index++;

    			//构建原图
    			auto dstTemp = dst;
    			//std::filesystem::path out(dst.string() + dir.filename().string() + "_" + ss.str() + path.extension().string());
    			std::string a = dir.filename().string() + "_" + ss.str() + path.extension().string();
    			auto& out = dstTemp.append(a);
    			ofs << out.string() << ", " << dir.filename().string() << std::endl;
    			TestImgProcFunctionality::testResize(path.string().c_str(), out.string().c_str());

    			auto temp = out;

    			//构建模糊图片的路径
    			std::string b = out.stem().string() + "_m" + out.extension().string();
    			auto& m = temp.replace_filename(b);
    			ofs << m.string() << ", " << dir.filename().string() << std::endl;
    			TestImgProcFunctionality::testBlur(path.string().c_str(), m.string().c_str());

    			std::string b2 = out.stem().string() + "_g" + out.extension().string();
    			auto& g = temp.replace_filename(b2);
    			ofs << g.string() << ", " << dir.filename().string() << std::endl;
    			TestImgProcFunctionality::testGauss(path.string().c_str(), g.string().c_str());

    			//构建锐化图片的路径
    			std::string c = out.stem().string() + "_r" + out.extension().string();
    			auto& r = temp.replace_filename(c);
    			ofs << r.string() << ", " << dir.filename().string() << std::endl;
    			TestImgProcFunctionality::testSharpen(path.string().c_str(), r.string().c_str());

    			//构建噪声图片的路径
    			std::string d = out.stem().string() + "_z" + out.extension().string();
    			auto& z = temp.replace_filename(d);
    			ofs << z.string() << ", " << dir.filename().string() << std::endl;
    			TestImgProcFunctionality::testNoise(path.string().c_str(), z.string().c_str());
    		}
    		else
    		{
    			//不是图片; 不做处理
    		}
    		//std::cout << "parent path: " << dir.string() << std::endl;
    		//std::cout << "dir name: " << dir.filename().string() << std::endl;
    		//std::cout << "extension: " << path.extension().string() << std::endl;
    		//std::cout << "file: " << path.string() << std::endl;

    		});
    }

    //关闭文本文件
    ofs.close();
}

void TestImgProcFunctionality::testStitcher()
{
    vector< string> files;
    glob("E:/images/patrol/1", files);
    vector<Mat> images;
    for (int i = 0; i < files.size(); i++) {
        printf("image file : %s n", files[i].c_str());
        auto img = imread(files[i]);
        cv::rotate(img, img, ROTATE_90_CLOCKWISE);
        cv::imshow(files[i].c_str(), img);
        cv::waitKey(0);
        images.push_back(img);
    }
    // 设置拼接模式与参数
    Mat result1, result2, result3;
    Stitcher::Mode mode = Stitcher::PANORAMA;
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    // 拼接方式-多通道融合
    auto blender = detail::Blender::createDefault(detail::Blender::MULTI_BAND);
    stitcher->setBlender(blender);
    // 拼接
    Stitcher::Status status = stitcher->stitch(images, result1);
    // 平面曲翘拼接
    auto plane_warper = makePtr<cv::PlaneWarper>();
    stitcher->setWarper(plane_warper);
    status = stitcher->stitch(images, result2);
    // 鱼眼拼接
    auto fisheye_warper = makePtr<cv::FisheyeWarper>();
    stitcher->setWarper(fisheye_warper);
    status = stitcher->stitch(images, result3);
    // 检查返回
    if (status != Stitcher::OK)
    {
        cout << "Can't stitch images, error code = " << int(status) << endl;
        return;
    }
    imwrite("E:/images/patrol/result1.png", result1);
    imwrite("E:/images/patrol/result2.png", result2);
    imwrite("E:/images/patrol/result3.png", result3);
    waitKey(0);
}

void TestImgProcFunctionality::testWatermark(const char* filepath)
{
    //
    cv::Mat src = cv::imread(filepath);
    if (src.empty())
        return;
    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    //
    HiddenWatermark hw1;
    cv::Mat temp = hw1.transformImageWithText(src, "Hello world", cv::Point(40, 80), 0.5, cv::Scalar(0, 0, 0));
    //cv::Mat show;
    //temp.convertTo(show, CV_8U);
    //cv::imshow("Temp", show);
    //cv::waitKey(0);
    cv::Mat dst = hw1.antitransformImage(temp);
    cv::imwrite("E:/Images/mark.jpg", dst);
    cv::imshow("WaterMark", dst);
    cv::waitKey(0);

    cv::Mat src2 = cv::imread("E:/Images/mark.jpg");
    if (src2.empty())
        return;
    cv::cvtColor(src2, src2, cv::COLOR_BGR2GRAY);
    HiddenWatermark hw2;
    cv::Mat dst2 = hw2.transformImage(src2);
    cv::imshow("anti", dst2);
    cv::waitKey(0);
}

void TestImgProcFunctionality::testRotate(const char* srcfilepath, const char* dstfilepath)
{
    auto src = cv::imread(srcfilepath);
    if (src.empty())
        return;

    cv::Mat dst;
    cv::rotate(src, dst, cv::ROTATE_90_COUNTERCLOCKWISE);
    cv::imwrite(dstfilepath, dst);
}

void TestImgProcFunctionality::testRotateAngle(const char* srcfilepath, const char* dstfilepath, double angle)
{
    auto src = cv::imread(srcfilepath);
    if (src.empty())
        return;
    //首先使用中心点作为旋转中心
    cv::Point center(src.cols / 2, src.rows / 2);
    //计算旋转矩阵
    cv::Mat tm = cv::getRotationMatrix2D(center, angle, 1);//第三个参数为1说明不作缩放
    //仿射变换
    cv::Mat dst;
    cv::warpAffine(src, dst, tm, cv::Size(src.cols, src.rows));

    cv::imwrite(dstfilepath, dst);
}





