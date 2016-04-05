/*!
 * \file Sigmoid.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/Sigmoid.hpp>

namespace mic {
namespace mlnn {

void Sigmoid::forward(bool apply_dropout) {

	y = logistic(x);

}

void Sigmoid::backward() {

	dx.array() = dy.array() * y.array() * (1.0 - y.array()).array();

}

Sigmoid::Sigmoid(size_t inputs, size_t outputs, size_t batch_size) :
	Layer(inputs, outputs, batch_size, "sigmoid") {

};


} /* namespace mlnn */
} /* namespace mic */
