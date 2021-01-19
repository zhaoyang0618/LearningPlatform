#pragma once
#include <opencv2/opencv.hpp>

class MyCropLayer : public cv::dnn::Layer
{
//private:
//    int xstart;
//    int xend;
//    int ystart;
//    int yend;
public:
    MyCropLayer(const cv::dnn::LayerParams& params);

    static cv::Ptr<cv::dnn::Layer> create(cv::dnn::LayerParams& params);

    virtual bool getMemoryShapes(const std::vector<std::vector<int> >& inputs,
        const int requiredOutputs,
        std::vector<std::vector<int> >& outputs,
        std::vector<std::vector<int> >& internals) const override;

    virtual void forward(cv::InputArrayOfArrays inputs, cv::OutputArrayOfArrays outputs, cv::OutputArrayOfArrays internals) override;
};
