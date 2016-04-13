/*!
 * \file ELU.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/ELU.hpp>

namespace mic {
namespace mlnn {

ELU::ELU(size_t inputs, size_t outputs, size_t batch_size) :
	Layer(inputs, outputs, batch_size, "elu") {

}

void ELU::forward(bool apply_dropout) {

	y = activation_ELU(x);

}

void ELU::backward() {

	dx.array() = derivative_ELU(y).array() * dy.array();

}

} /* namespace mlnn */
} /* namespace mic */
