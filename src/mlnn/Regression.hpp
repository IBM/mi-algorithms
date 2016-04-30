/*!
 * \file Regression.hpp
 * \brief 
 * \author tkornut
 * \date Apr 22, 2016
 */

#ifndef SRC_MLNN_REGRESSION_HPP_
#define SRC_MLNN_REGRESSION_HPP_

#include <mlnn/Layer.hpp>

namespace mic {
namespace mlnn {

/*!
 * \brief Regression layer
 * \author tkornuta
 */
class Regression : public mic::mlnn::Layer {
public:

	Regression(size_t inputs_, size_t outputs_, size_t batch_size_, std::string name_ = "Regression");

	~Regression() {};

	void forward(bool test_ = false);

	void backward();

	/*!
	 * Private constructor, used only during the serialization.
	 */
	Regression() : Layer () { }

};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_REGRESSION_HPP_ */
