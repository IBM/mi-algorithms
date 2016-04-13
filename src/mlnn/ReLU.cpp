/*!
 * \file ReLU.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/ReLU.hpp>

namespace mic {
namespace mlnn {


ReLU::ReLU(size_t inputs, size_t outputs, size_t batch_size) :
	Layer(inputs, outputs, batch_size, "relu") {

}


void ReLU::forward(bool apply_dropout) {

	y = rectify(x);

}

void ReLU::backward() {

	dx.array() = derivative_ReLU(y).array() * dy.array();

}

} /* namespace mlnn */
} /* namespace mic */
