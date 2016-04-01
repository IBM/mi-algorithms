/*!
 * \file Layer.hpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#ifndef SRC_MLNN_LAYER_HPP_
#define SRC_MLNN_LAYER_HPP_

#include <string>

#include<types/MatrixTypes.hpp>
#include <mlnn/nn_utils.h>

namespace mic {
namespace mlnn {

/*!
 * abstract
 * \author krocki
 */
class Layer {

	public:

		const std::string name;

		//used in forward pass
		mic::types::MatrixXf x; //inputs
		mic::types::MatrixXf y; //outputs

		//grads, used in backward pass
		mic::types::MatrixXf dx;
		mic::types::MatrixXf dy;

		const float dropout;
		mic::types::MatrixXf dropout_mask;

		Layer(size_t inputs, size_t outputs, size_t batch_size, std::string _label = "layer", float _dropout = 1.0f);

		// Dropout paper - http://arxiv.org/pdf/1207.0580.pdf
		void applyDropout();

		//need to override these
		virtual void forward(bool apply_dropout) = 0;

		virtual void backward() = 0;

		//this is mainly for debugging - TODO: proper serialization of layers and object NN
		virtual void save_to_files(std::string prefix) {};

		virtual void resetGrads() {};

		virtual void applyGrads(double alpha, double decay) {};

		virtual ~Layer() {};

		// Duplicated entries fix, TO BE REMOVED when "proper serialization" will be implemented.
		void save_matrix_to_file(Eigen::MatrixXf& m, std::string filename);
};


} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_LAYER_HPP_ */
