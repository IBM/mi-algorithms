/*!
 * \file Dropout.hpp
 * \brief 
 * \author tkornut
 * \date Apr 12, 2016
 */

#ifndef SRC_MLNN_DROPOUT_HPP_
#define SRC_MLNN_DROPOUT_HPP_

#include <mlnn/Layer.hpp>

namespace mic {
namespace mlnn {

/*!
 * \author krocki
 */
class Dropout : public Layer {
public:

	Dropout(size_t inputs, size_t outputs, size_t batch_size, float _ratio);

	virtual ~Dropout() {};

	void forward(bool test = false);

	void backward();

protected:

	const float keep_ratio;

	mic::types::MatrixXf dropout_mask;

};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_DROPOUT_HPP_ */
