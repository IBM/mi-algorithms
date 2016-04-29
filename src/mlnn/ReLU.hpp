/*!
 * \file ReLU.hpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#ifndef SRC_MLNN_RELU_HPP_
#define SRC_MLNN_RELU_HPP_

#include <mlnn/Layer.hpp>

namespace mic {
namespace mlnn {

/*!
 * \author krocki
 */
class ReLU : public mic::mlnn::Layer {
public:

	ReLU(size_t inputs, size_t outputs, size_t batch_size, std::string name_ = "ReLU");

	virtual ~ReLU() {};

	void forward(bool apply_dropout = false);

	void backward();
};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_RELU_HPP_ */
