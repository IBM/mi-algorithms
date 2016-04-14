/*!
 * \file Padding.cpp
 * \brief 
 * \author tkornut
 * \date Apr 12, 2016
 */

#include <mlnn/Padding.hpp>

namespace mic {
namespace mlnn {

Padding::Padding(size_t inputs, size_t _channels, size_t batch_size, size_t _padding) :
	Layer(inputs, _channels * (sqrt(inputs / _channels) + _padding * 2) * (sqrt(inputs / _channels) + _padding * 2), batch_size, "padding"), channels(_channels), padding(_padding) {

};


void Padding::forward(bool test) {

	//pad(x, y, channels, padding);

}

void Padding::backward() {

	//unpad(dx, dy, channels, padding);

}


} /* namespace mlnn */
} /* namespace mic */
