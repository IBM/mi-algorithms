/*!
 * \file Identity.hpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#ifndef SRC_MLNN_IDENTITY_HPP_
#define SRC_MLNN_IDENTITY_HPP_

#include <mlnn/Layer.hpp>

namespace mic {
namespace mlnn {

/*!
 * \author krocki
 */
class Identity : public mic::mlnn::Layer {

	public:

		void forward(bool apply_dropout = false);

		void backward();

		Identity(size_t inputs, size_t outputs, size_t batch_size);

		virtual ~Identity() {};

};
} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_IDENTITY_HPP_ */
