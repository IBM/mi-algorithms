/*!
 * \file Dropout.cpp
 * \brief 
 * \author tkornut
 * \date Apr 12, 2016
 */

#include <mlnn/Dropout.hpp>

namespace mic {
namespace mlnn {

Dropout::Dropout(size_t inputs, size_t outputs, size_t batch_size, float _ratio) :
	Layer(inputs, outputs, batch_size, "dropout"),  keep_ratio(_ratio) {

};


void Dropout::forward(bool test) {

	if (test) { // skip at test time

		y = x;

	} else {

		mic::types::MatrixXf rands = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(y.rows(), y.cols());
		rand(rands, 0.0f, 1.0f);

		//dropout mask - 1s - preserved elements
		dropout_mask = (rands.array() < keep_ratio).cast <float> ();

		// y = y .* dropout_mask, discard elements where mask is 0
		y.array() = x.array() * dropout_mask.array();

		// normalize, so that we don't have to do anything at test time
		y /= keep_ratio;

	}
}

void Dropout::backward() {

	dx.array() = dy.array() * dropout_mask.array();
}

} /* namespace mlnn */
} /* namespace mic */
