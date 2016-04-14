/*!
 * \file Pooling.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/Pooling.hpp>

namespace mic {
namespace mlnn {

Pooling::Pooling(size_t inputs_, size_t window_size_, size_t channels_, size_t batch_size_) :
	Layer(inputs_, channels_ * (sqrt(inputs_ / channels_) / window_size_) * (sqrt(inputs_ / channels_) / window_size_), batch_size_, "pool"),
	channels(channels_), window_size(window_size_) {

	// cache = mic::types::MatrixXf::Zero(x.rows(), x.cols());
	cache = mic::types::MatrixXf::Zero(inputs_size, batch_size);

};

void Pooling::forward(bool test_) {

	pooling_forward(channels, *(s['x']), *(s['y']), cache, window_size);

}

void Pooling::backward() {

	pooling_backward(channels, *(g['x']), *(g['y']), cache, window_size);

}

void Pooling::save_to_files(std::string prefix) {

	Layer::save_to_files(prefix);
	save_matrix_to_file(cache, prefix + "_cache.txt");

}

} /* namespace mlnn */
} /* namespace mic */
