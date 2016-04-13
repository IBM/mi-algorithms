/*!
 * \file Softmax.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/Softmax.hpp>

namespace mic {
namespace mlnn {

Softmax::Softmax(size_t inputs, size_t outputs, size_t batch_size) :
	Layer(inputs, outputs, batch_size, "softmax") {

}

void Softmax::forward(bool test) {

	y = softmax(x);

}

void Softmax::backward() {

	dx = dy - y;
}


} /* namespace mlnn */
} /* namespace mic */
