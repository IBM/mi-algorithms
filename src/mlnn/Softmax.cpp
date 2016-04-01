/*!
 * \file Softmax.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/Softmax.hpp>

namespace mic {
namespace mlnn {

void Softmax::forward(bool apply_dropout) {

	y = softmax(x);

}

void Softmax::backward() {

	dx = dy - y;
}


Softmax::Softmax(size_t inputs, size_t outputs, size_t batch_size) :
	Layer(inputs, outputs, batch_size, "softmax") {

}

} /* namespace mlnn */
} /* namespace mic */
