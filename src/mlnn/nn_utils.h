/*
* @Author: kmrocki
* @Date:   2016-02-24 10:47:03
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-03-10 12:43:26
*/

#ifndef __NN_UTILS_H__
#define __NN_UTILS_H__

#include <iostream>
#include <iomanip>
#include <random>

#include <mlnn/importer.h> // TO BE REMOVED, now required for datapoint type :]

#include <types/MatrixTypes.hpp>

inline float sqrt_eps(const float x) {
	return sqrtf(x + 1e-6);
}

//f(x) = sigm(x)
/*inline float __logistic(const float x) {
	return 1.0f / (1.0f +::expf(-x));
}*/

/*inline float __exponential(const float x) {
	return expf(x);
}*/

//mic::types::MatrixXf rectify(mic::types::MatrixXf& x);

// Exponential Linear Unit
// http://arxiv.org/pdf/1511.07289v5.pdf
//mic::types::MatrixXf activation_ELU(mic::types::MatrixXf& x);

//mic::types::MatrixXf derivative_ELU(mic::types::MatrixXf& x);

//mic::types::MatrixXf derivative_ReLU(mic::types::MatrixXf& x);

//mic::types::MatrixXf logistic(mic::types::MatrixXf& x);

//mic::types::MatrixXf softmax(mic::types::MatrixXf& x);

//float cross_entropy(mic::types::MatrixXf& predictions, mic::types::MatrixXf& targets);

//generate an array of random numbers in range
void randi(mic::types::VectorXi& m, int range_min, int range_max);

//void __unpool_disjoint_2D(mic::types::MatrixXf& dx, mic::types::MatrixXf& cache, mic::types::MatrixXf& dy, size_t kernel_size);

//void __pool_disjoint_2D(mic::types::MatrixXf& out, mic::types::MatrixXf& cache, mic::types::MatrixXf& image, size_t kernel_size);

//void pad(mic::types::MatrixXf& x, mic::types::MatrixXf& x_padded, size_t kernel_size, size_t input_channels);

//outer loop over image locations, all images processed in parallel
//void convolution_forward_gemm(size_t input_channels, mic::types::MatrixXf& out, mic::types::MatrixXf& W, mic::types::MatrixXf& in, mic::types::VectorXf& b);

//void convolution_backward_full_gemm(size_t input_channels, mic::types::MatrixXf& out, mic::types::MatrixXf& W, mic::types::MatrixXf& in);

//void convolution_backward_gemm(size_t input_channels, mic::types::MatrixXf& out, mic::types::MatrixXf& W, mic::types::MatrixXf& in, mic::types::MatrixXf& b);

//mic::types::MatrixXf pooling_forward_channel(mic::types::MatrixXf& x, mic::types::MatrixXf& cache, size_t window_size);

//mic::types::MatrixXf pooling_backward_channel(mic::types::MatrixXf& dy, mic::types::MatrixXf& cache, size_t window_size);

//void pooling_forward(size_t channels, mic::types::MatrixXf& x, mic::types::MatrixXf& y, mic::types::MatrixXf& cache, size_t window_size);

//void pooling_backward(size_t channels, mic::types::MatrixXf& dx, mic::types::MatrixXf& dy, mic::types::MatrixXf& cache, size_t window_size);

void randn(mic::types::MatrixXf& m, float mean, float stddev);

void rand(mic::types::MatrixXf& m, float range_min, float range_max);

void linspace(mic::types::VectorXi& m, int range_min, int range_max);

mic::types::MatrixXf make_batch(std::deque<datapoint>& data, mic::types::VectorXi& random_numbers);

mic::types::MatrixXf make_targets(std::deque<datapoint>& data, mic::types::VectorXi& random_numbers, size_t classes);

/*mic::types::VectorXi colwise_max_index(mic::types::MatrixXf& m);

size_t count_zeros(mic::types::VectorXi& m);

size_t count_correct_predictions(mic::types::MatrixXf& p, mic::types::MatrixXf& t);*/

#endif
