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

	Convolution(size_t inputs, size_t channels, size_t filter_size, size_t filters, size_t batch_size, std::string name_ = "Convolution");

	virtual ~Convolution() {};

	void forward(bool test = false);

	/*!
	 * Outer loop over image locations, all images processed in parallel
	 * @param out
	 * @param in
	 */
	void forwardGemm(mic::types::MatrixXf& out, mic::types::MatrixXf& in);

	void backward();

	void backwardGemm(mic::types::MatrixXf& out, mic::types::MatrixXf& in);

	void backwardFullGemm(mic::types::MatrixXf& out, mic::types::MatrixXf& in);

	void resetGrads();

	void applyGrads(double alpha, double decay = 0);

	//this is mainly for debugging - TODO: proper serialization of layers
	void save_to_files(std::string prefix);

protected:
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


};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_CONVOLUTION_HPP_ */
