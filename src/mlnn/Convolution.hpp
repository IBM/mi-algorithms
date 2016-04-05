/*!
 * \file Convolution.hpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#ifndef SRC_MLNN_CONVOLUTION_HPP_
#define SRC_MLNN_CONVOLUTION_HPP_

#include <mlnn/Layer.hpp>

namespace mic {
namespace mlnn {

/*!
 * \author krocki
 */
class Convolution : public mic::mlnn::Layer {
	public:

		mic::types::MatrixXf W;
		mic::types::VectorXf b;

		mic::types::MatrixXf dW;
		mic::types::MatrixXf db;

		mic::types::MatrixXf mW;
		mic::types::MatrixXf mb;

		const size_t input_channels;
		const size_t output_channels;
		const size_t kernel_size;
		const size_t output_map_size;

		void forward(bool apply_dropout = false);

		void backward();

		//this is mainly for debugging - TODO: proper serialization of layers
		void save_to_files(std::string prefix);

		Convolution(size_t inputs, size_t channels, size_t filter_size, size_t filters, size_t batch_size);

		void resetGrads();

		void applyGrads(double alpha, double decay = 0);

		virtual ~Convolution() {};

};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_CONVOLUTION_HPP_ */
