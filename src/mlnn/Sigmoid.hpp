/*!
 * \file Sigmoid.hpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#ifndef SRC_MLNN_SIGMOID_HPP_
#define SRC_MLNN_SIGMOID_HPP_

#include <mlnn/Layer.hpp>

namespace mic {
namespace mlnn {

/*!
 * \author krocki
 */
class Sigmoid : public mic::mlnn::Layer {
	public:

		void forward(bool apply_dropout = false);

		void backward();

		Sigmoid(size_t inputs, size_t outputs, size_t batch_size);

		virtual ~Sigmoid() {};

};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_SIGMOID_HPP_ */
