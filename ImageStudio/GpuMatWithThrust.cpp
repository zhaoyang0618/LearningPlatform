#include "GpuMatWithThrust.h"
#include <opencv2/core/cuda_stream_accessor.hpp>

#include <thrust/transform.h>
#include <thrust/random.h>
#include <thrust/sort.h>
#include <thrust/count.h>
#include <thrust/sequence.h>
#include <thrust/system/cuda/execution_policy.h>

#include <thrust/for_each.h>
#include <thrust/device_vector.h>
#include <thrust/execution_policy.h>


////! [prg]
//struct prg
//{
//    float a, b;
//
//    __host__ __device__
//        prg(float _a = 0.f, float _b = 1.f) : a(_a), b(_b) {};
//
//    __host__ __device__
//        float operator()(const unsigned int n) const
//    {
//        thrust::default_random_engine rng;
//        thrust::uniform_real_distribution<float> dist(a, b);
//        rng.discard(n);
//        return dist(rng);
//    }
//};
////! [prg]
//
//
////! [pred_greater]
//template<typename T> struct pred_greater
//{
//    T value;
//    __host__ __device__ pred_greater(T value_) : value(value_) {}
//    __host__ __device__ bool operator()(const T& val) const
//    {
//        return val > value;
//    }
//};
////! [pred_greater]

struct printf_functor
{
    __host__ __device__
        void operator()(int x)
    {
        // note that using printf in a __device__ function requires
        // code compiled for a GPU with compute capability 2.0 or
        // higher (nvcc --arch=sm_20)
        printf("%d\n", x);
    }
};

void GpuMatWithThrust::test()
{
    //// Generate a 2 channel row matrix with 100 elements.  Set the first channel to be the element index, and the second to be a randomly
    //// generated value.  Sort by the randomly generated value while maintaining index association.
    ////! [sort]
    //{
    //    cv::cuda::GpuMat d_data(1, 100, CV_32SC2);
    //    // Thrust compatible begin and end iterators to channel 1 of this matrix
    //    auto keyBegin = GpuMatBeginItr<int>(d_data, 1);
    //    auto keyEnd = GpuMatEndItr<int>(d_data, 1);
    //    // Thrust compatible begin and end iterators to channel 0 of this matrix
    //    auto idxBegin = GpuMatBeginItr<int>(d_data, 0);
    //    auto idxEnd = GpuMatEndItr<int>(d_data, 0);
    //    // Fill the index channel with a sequence of numbers from 0 to 100
    //    thrust::sequence(idxBegin, idxEnd);
    //    // Fill the key channel with random numbers between 0 and 10.  A counting iterator is used here to give an integer value for each location as an input to prg::operator()
    //    thrust::transform(thrust::make_counting_iterator(0), thrust::make_counting_iterator(d_data.cols), keyBegin, prg(0, 10));
    //    // Sort the key channel and index channel such that the keys and indecies stay together
    //    thrust::sort_by_key(keyBegin, keyEnd, idxBegin);

    //    cv::Mat h_idx(d_data);
    //}
    ////! [sort]

    //// Randomly fill a row matrix with 100 elements between -1 and 1
    ////! [random]
    //{
    //    cv::cuda::GpuMat d_value(1, 100, CV_32F);
    //    auto valueBegin = GpuMatBeginItr<float>(d_value);
    //    auto valueEnd = GpuMatEndItr<float>(d_value);
    //    thrust::transform(thrust::make_counting_iterator(0), thrust::make_counting_iterator(d_value.cols), valueBegin, prg(-1, 1));

    //    cv::Mat h_value(d_value);
    //}
    ////! [random]

    //// OpenCV has count non zero, but what if you want to count a specific value?
    ////! [count_value]
    //{
    //    cv::cuda::GpuMat d_value(1, 100, CV_32S);
    //    d_value.setTo(cv::Scalar(0));
    //    d_value.colRange(10, 50).setTo(cv::Scalar(15));
    //    auto count = thrust::count(GpuMatBeginItr<int>(d_value), GpuMatEndItr<int>(d_value), 15);
    //    std::cout << count << std::endl;
    //}
    ////! [count_value]

    //// Randomly fill an array then copy only values greater than 0.  Perform these tasks on a stream.
    ////! [copy_greater]
    //{
    //    cv::cuda::GpuMat d_value(1, 100, CV_32F);
    //    auto valueBegin = GpuMatBeginItr<float>(d_value);
    //    auto valueEnd = GpuMatEndItr<float>(d_value);
    //    cv::cuda::Stream stream;
    //    //! [random_gen_stream]
    //    // Same as the random generation code from before except now the transformation is being performed on a stream
    //    thrust::transform(thrust::system::cuda::par.on(cv::cuda::StreamAccessor::getStream(stream)), thrust::make_counting_iterator(0), thrust::make_counting_iterator(d_value.cols), valueBegin, prg(-1, 1));
    //    //! [random_gen_stream]
    //    // Count the number of values we are going to copy
    //    int count = thrust::count_if(thrust::system::cuda::par.on(cv::cuda::StreamAccessor::getStream(stream)), valueBegin, valueEnd, pred_greater<float>(0.0));
    //    // Allocate a destination for copied values
    //    cv::cuda::GpuMat d_valueGreater(1, count, CV_32F);
    //    // Copy values that satisfy the predicate.
    //    thrust::copy_if(thrust::system::cuda::par.on(cv::cuda::StreamAccessor::getStream(stream)), valueBegin, valueEnd, GpuMatBeginItr<float>(d_valueGreater), pred_greater<float>(0.0));
    //    cv::Mat h_greater(d_valueGreater);
    //}
    ////! [copy_greater]

    //for_each没有实现，无法使用
    //thrust::device_vector<int> d_vec(3);
    //d_vec[0] = 0; d_vec[1] = 1; d_vec[2] = 2;
    //thrust::for_each(thrust::device, d_vec.begin(), d_vec.end(), printf_functor());

    //
    //// initialize all ten integers of a device_vector to 1
    //thrust::device_vector<int> D(10, 1);

    //// set the first seven elements of a vector to 9
    //thrust::fill(D.begin(), D.begin() + 7, 9);

    //// initialize a host_vector with the first five elements of D
    //thrust::host_vector<int> H(D.begin(), D.begin() + 5);

    //// set the elements of H to 0, 1, 2, 3, ...
    //thrust::sequence(H.begin(), H.end());

    //// copy all of H back to the beginning of D
    //thrust::copy(H.begin(), H.end(), D.begin());

    //// print D
    //for (int i = 0; i < D.size(); i++)
    //    std::cout << "D[" << i << "] = " << D[i] << std::endl;
}
