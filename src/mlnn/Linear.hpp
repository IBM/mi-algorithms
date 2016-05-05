/*!
 * \file Linear.hpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#ifndef SRC_MLNN_LINEAR_HPP_
#define SRC_MLNN_LINEAR_HPP_

#include <mlnn/Layer.hpp>

namespace mic {
namespace mlnn {

/*!
 * \brief Class implementing a linear, fully connecteed layer.
 * \author krocki
 */
class Linear : public mic::mlnn::Layer {
public:
	/*!
	 * Default constructor used for creation of the layer.
	 * @param inputs_
	 * @param outputs_
	 * @param batch_size_
	 * @param name_
	 */
	Linear(size_t inputs_, size_t outputs_, size_t batch_size_, std::string name_ = "Linear");

	virtual ~Linear() {};

	void forward(bool test_ = false);

	void backward();

	void resetGrads();

	void applyGrads(double alpha_, double decay_ = 0);

private:

	// Adds the nn class the access to protected fields of class layer.
	friend class MultiLayerNeuralNetwork;

	/*!
	 * Private constructor, used only during the serialization.
	 */
	Linear() : Layer () { }

};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_LINEAR_HPP_ */
