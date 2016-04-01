/*!
 * \file Layer.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/Layer.hpp>

namespace mic {
namespace mlnn {

Layer::Layer(size_t inputs, size_t outputs, size_t batch_size, std::string _label, float _dropout) : name(_label), dropout(_dropout) {

	x = mic::types::MatrixXf(inputs, batch_size);
	y = mic::types::MatrixXf(outputs, batch_size);
	dx = mic::types::MatrixXf(inputs, batch_size);
	dy = mic::types::MatrixXf(outputs, batch_size);

	//for dropout implementation
	dropout_mask = mic::types::MatrixXf::Ones(y.rows(), y.cols());

};

void Layer::applyDropout() {

	mic::types::MatrixXf rands = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(y.rows(), y.cols());
	rand(rands, 0.0f, 1.0f);
	//dropout mask - 1s - preserved elements
	dropout_mask = (rands.array() < dropout).cast <float> ();
	// y = y .* dropout_mask, discard elements where mask is 0
	y.array() = y.array() * dropout_mask.array();
	// normalize, so that we don't have to do anything at test time
	y /= dropout;

}

void Layer::save_matrix_to_file(Eigen::MatrixXf& m, std::string filename) {


	std::cout << "Saving a matrix to " << filename << "... " << std::endl;
	std::ofstream file(filename.c_str());

	if (file.is_open()) {

		file << m;
		file.close();

	} else {

		std::cout << "file save error: (" << filename << ")" << std::endl;

	}

}


} /* namespace mlnn */
} /* namespace mic */
