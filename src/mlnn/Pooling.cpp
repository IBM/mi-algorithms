/*!
 * \file Pooling.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/Pooling.hpp>

namespace mic {
namespace mlnn {

Pooling::Pooling(size_t inputs, size_t _window_size, size_t _channels, size_t batch_size) :
	Layer(inputs, _channels * (sqrt(inputs / _channels) / _window_size) * (sqrt(inputs / _channels) / _window_size), batch_size, "pool"),
	channels(_channels), window_size(_window_size) {

	cache = mic::types::MatrixXf::Zero(x.rows(), x.cols());

};

void Pooling::forward(bool test) {

	pooling_forward(channels, x, y, cache, window_size);

}

void Pooling::backward() {

	pooling_backward(channels, dx, dy, cache, window_size);

}

void Pooling::save_to_files(std::string prefix) {

	Layer::save_to_files(prefix);
	save_matrix_to_file(cache, prefix + "_cache.txt");

}

} /* namespace mlnn */
} /* namespace mic */
