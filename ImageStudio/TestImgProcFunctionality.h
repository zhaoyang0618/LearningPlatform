///
/// ͼ����ĸ����㷨����
#pragma once
#include <opencv2/opencv.hpp>

class TestImgProcFunctionality
{
public:
	/// <summary>
	/// ��ֵ��
	/// </summary>
	/// <param name="filepath"></param>
	/// <param name="outputFolder"></param>
	/// <param name="outputPrefix"></param>
	static void testThreshold(const char* filepath, const char* outputFolder, const char* outputPrefix);

	/// <summary>
	/// Houghֱ��
	/// </summary>
	/// <param name="filename"></param>
	static void testHoughLine(const char* filename);

	/// <summary>
	/// ��ɢ����Ҷ�任
	/// </summary>
	static void testDiscreteFourierTransform(const char* filename);

	/// <summary>
	/// ��˹ģ��
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="outfilename"></param>
	static void testResize(const char* filename, const char* outfilename);
	static void testBlur(const char* filename, const char* outfilename);
	static void testGauss(const char* filename, const char* outfilename);
	static void testSharpen(const char* filename, const char* outfilename);
	static void testNoise(const char* filename, const char* outfilename);

	static void testMorph(const char* filepath);
	template<typename FUNC>
	static void findHorizontalAndVerticalLine(cv::Mat& src, FUNC output);

	static void findHorizontalAndVerticalLine(const char* filepath, const char* outDir, int index);

	/// <summary>
	/// �����ļ����µ�ͼƬ����һ������Ĳ���
	/// </summary>
	/// <param name="dir"></param>
	static void throughoutFolder(const char* dir, const char* outDir);

	/// <summary>
	/// ��һ��ͼƬ����һ��ͼƬ���ڻ���ѧϰ(�������ѧϰ)
	/// </summary>
	static void GenerateMLImages();

	static void testStitcher();

    static void testWatermark(const char* filepath);
};

/// <summary>
/// https://blog.csdn.net/chenxiao_ji/article/details/52875199
/// äˮӡ
/// </summary>
class HiddenWatermark
{
private:
    std::vector<cv::Mat> planes;
    cv::Mat complexImage;

public:
    cv::Mat transformImage(cv::Mat image) 
    {
        // planes�����д��ͨ��������ʼ��Ϊ��,�����.
        planes.clear();
        // optimize the dimension of the loaded image
        //cv::Mat padded = optimizeImageDim(image);
        cv::Mat padded = splitSrc(image);
        padded.convertTo(padded, CV_32F);
        // prepare the image planes to obtain the complex image
        planes.push_back(padded);
        planes.push_back(cv::Mat::zeros(padded.size(), CV_32F));
        // prepare a complex image for performing the dft
        merge(planes, complexImage);
        // dft
        dft(complexImage, complexImage);
        // optimize the image resulting from the dft operation
        cv::Mat magnitude = createOptimizedMagnitude(complexImage);
        planes.clear();
        return magnitude;
    }

    cv::Mat transformImageWithText(cv::Mat image, cv::String watermarkText, cv::Point point, double fontSize, cv::Scalar scalar) {
        // planes�����д��ͨ��������ʼ��Ϊ��,�����.
        std::vector<cv::Mat> planes;
        //planes.clear();
        // optimize the dimension of the loaded image
        //cv::Mat padded = optimizeImageDim(image);
        //cv::Mat padded = image;
        cv::Mat padded = splitSrc(image);
        padded.convertTo(padded, CV_32F);
        // prepare the image planes to obtain the complex image
        planes.push_back(padded);
        planes.push_back(cv::Mat::zeros(padded.size(), CV_32F));
        // prepare a complex image for performing the dft
        cv::Mat complexImage;
        merge(planes, complexImage);
        // dft
        dft(complexImage, complexImage);
        // Ƶ��ͼ������ı�
        putText(complexImage, watermarkText, point, cv::FONT_HERSHEY_DUPLEX, fontSize, scalar, 1);
        flip(complexImage, complexImage, -1);
        putText(complexImage, watermarkText, point, cv::FONT_HERSHEY_DUPLEX, fontSize, scalar, 1);
        flip(complexImage, complexImage, -1);
        planes.clear();
        return complexImage;
    }

    cv::Mat antitransformImage(cv::Mat& complexImage) {
        cv::Mat invDFT;
        idft(complexImage, invDFT, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT, 0);
        cv::Mat restoredImage;
        invDFT.convertTo(restoredImage, CV_8U);
        //planes.clear();
        return restoredImage;
    }

    /**
     * Ϊ�ӿ층��Ҷ�任���ٶȣ���Ҫ�����ͼƬ�ߴ�����Ż�
     *
     * @param image
     *            the {@link Mat} to optimize
     * @return the image whose dimensions have been optimized
     */
    cv::Mat optimizeImageDim(cv::Mat image) {
        // init
        cv::Mat padded;
        // get the optimal rows size for dft
        int addPixelRows = cv::getOptimalDFTSize(image.rows);
        // get the optimal cols size for dft
        int addPixelCols = cv::getOptimalDFTSize(image.cols);
        // apply the optimal cols and rows size to the image
        copyMakeBorder(image, padded, 0, addPixelRows - image.rows, 0, addPixelCols - image.cols,
            cv::BORDER_CONSTANT, cv::Scalar::all(0));

        return padded;
    }

    cv::Mat splitSrc(cv::Mat mat) {
        mat = optimizeImageDim(mat);
        std::vector<cv::Mat> allPlanes;
        split(mat, allPlanes);
        cv::Mat padded;
        if (allPlanes.size() > 1) {
            for (int i = 0; i < allPlanes.size(); i++) {
                if (i == 0) {
                    padded = allPlanes[i];
                    break;
                }
            }
        }
        else {
            padded = mat;
        }
        return padded;
    }

    /**
     * Optimize the magnitude of the complex image obtained from the DFT, to
     * improve its visualization
     *
     * @param complexImage
     *            the complex image obtained from the DFT
     * @return the optimized image
     */
    cv::Mat createOptimizedMagnitude(cv::Mat complexImage) {
        // init
        std::vector<cv::Mat> newPlanes;
        cv::Mat mag;
        // split the comples image in two planes
        split(complexImage, newPlanes);
        // compute the magnitude
        magnitude(newPlanes[0], newPlanes[1], mag);

        // move to a logarithmic scale
        add(cv::Mat::ones(mag.size(), CV_32F), mag, mag);
        log(mag, mag);
        // optionally reorder the 4 quadrants of the magnitude image
        shiftDFT(mag);
        // normalize the magnitude image for the visualization since both JavaFX
        // and OpenCV need images with value between 0 and 255
        // convert back to CV_8UC1
        mag.convertTo(mag, CV_8UC1);
        normalize(mag, mag, 0, 255, cv::NORM_MINMAX, CV_8UC1);

        return mag;
    }

    /**
     * Reorder the 4 quadrants of the image representing the magnitude, after
     * the DFT
     *
     * @param image
     *            the {@link Mat} object whose quadrants are to reorder
     */
    void shiftDFT(cv::Mat image) {
        image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));
        int cx = image.cols / 2;
        int cy = image.rows / 2;

        cv::Mat q0(image, cv::Rect(0, 0, cx, cy));
        cv::Mat q1(image, cv::Rect(cx, 0, cx, cy));
        cv::Mat q2(image, cv::Rect(0, cy, cx, cy));
        cv::Mat q3(image, cv::Rect(cx, cy, cx, cy));

        cv::Mat tmp;
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);

        q1.copyTo(tmp);
        q2.copyTo(q1);
        tmp.copyTo(q2);
    }
};


