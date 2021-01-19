#include "MyCropLayer.h"

MyCropLayer::MyCropLayer(const cv::dnn::LayerParams& params)
	: Layer(params)
    //, xstart(0)
    //, xend(0)
    //, ystart(0)
    //, yend(0)
{

}

cv::Ptr<cv::dnn::Layer> MyCropLayer::create(cv::dnn::LayerParams& params)
{
	return cv::Ptr<cv::dnn::Layer>(new MyCropLayer(params));
}

bool MyCropLayer::getMemoryShapes(const std::vector<std::vector<int> >& inputs,
    const int requiredOutputs,
    std::vector<std::vector<int> >& outputs,
    std::vector<std::vector<int> >& internals) const
{
    //inputShape, targetShape = inputs[0], inputs[1]
    //batchSize, numChannels = inputShape[0], inputShape[1]
    //height, width = targetShape[2], targetShape[3]

    //self.ystart = (inputShape[2] - targetShape[2]) / 2
    //self.xstart = (inputShape[3] - targetShape[3]) / 2
    //self.yend = self.ystart + height
    //self.xend = self.xstart + width

    //return [[batchSize, numChannels, height, width]]
    auto& inputShape = inputs[0];
    auto& targetShape = inputs[1];
    auto batchSize = inputShape[0];
    auto numChannels = inputShape[1];
    auto height = targetShape[2];
    auto width = targetShape[3];
    int ystart = (inputShape[2] - targetShape[2]) / 2;
    int xstart = (inputShape[3] - targetShape[3]) / 2;
    int yend = ystart + height;
    int xend = xstart + width;

    std::vector<int> outShape(4);
    outShape[0] = batchSize;
    outShape[1] = numChannels;
    outShape[2] = height;
    outShape[3] = width;
    outputs.assign(1, outShape);

    std::cout << "xstart:=" << xstart << "; ystart=" << ystart << "; xend=" << xend << "; yend=" << yend << std::endl;
    std::vector<int> extra(4);
    extra[0] = xstart;
    extra[1] = ystart;
    extra[2] = xend;
    extra[3] = yend;
    internals.assign(1, extra);
    return false;
}

void MyCropLayer::forward(cv::InputArrayOfArrays inputs_arr, cv::OutputArrayOfArrays outputs_arr, cv::OutputArrayOfArrays internals)
{
    std::vector <cv::Mat> inputs, outputs;
    inputs_arr.getMatVector(inputs);
    outputs_arr.getMatVector(outputs);
    cv::Mat& inp = inputs[0];
    cv::Mat& out = outputs[0];
    const float* inpData = (float*)inp.data;
    float* outData = (float*)out.data;

    const int inpHeight = inp.size[2];
    const int inpWidth = inp.size[3];

    const int numChannels = out.size[1];
    const int outHeight = out.size[2];
    const int outWidth = out.size[3];

    int scale = int(outHeight / inpHeight);
    int count = 0;

    for (int ch = 0; ch < numChannels; ch++)
    {
        for (int y = 0; y < outHeight; y++)
        {
            for (int x = 0; x < outWidth; x++)
            {
                int x_coord = static_cast<int>(floor((y / scale)));
                int y_coord = static_cast<int>(floor((x / scale)));
                int c_coord = numChannels * scale * (y % scale) + numChannels * (x % scale) + ch;

                int index = (((c_coord * inpHeight) + x_coord) * inpWidth) + y_coord;

                outData[count++] = inpData[index];
            }
        }
    }
}
