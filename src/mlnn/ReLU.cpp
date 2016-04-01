/*!
 * \file ReLU.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/ReLU.hpp>

namespace mic {
namespace mlnn {

void ReLU::forward(bool apply_dropout) {

	y = rectify(x);

}

void ReLU::save_to_files(std::string prefix) {

	save_matrix_to_file(y, prefix + "_y.txt");

}

void ReLU::backward() {

	dx.array() = derivative_ReLU(y).array() * dy.array();

}

ReLU::ReLU(size_t inputs, size_t outputs, size_t batch_size) :
	Layer(inputs, outputs, batch_size, "relu") {

}

} /* namespace mlnn */
} /* namespace mic */
