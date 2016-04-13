/*!
 * \file Layer.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/Layer.hpp>

namespace mic {
namespace mlnn {

Layer::Layer(size_t inputs, size_t outputs, size_t batch_size, std::string _label) : name(_label) {

	x = mic::types::MatrixXf(inputs, batch_size);
	y = mic::types::MatrixXf(outputs, batch_size);
	dx = mic::types::MatrixXf(inputs, batch_size);
	dy = mic::types::MatrixXf(outputs, batch_size);

};

//this is mainly for debugging - TODO: proper serialization of layers and object NN
void Layer::save_to_files(std::string prefix) {

	save_matrix_to_file(x, prefix + "_x");
	save_matrix_to_file(y, prefix + "_y");
	save_matrix_to_file(dx, prefix + "_dx");
	save_matrix_to_file(dy, prefix + "_dy");

};


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
