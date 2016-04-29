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
#include<types/MatrixArray.hpp>

namespace mic {
namespace mlnn {

/*!
 * \brief Enumeration of possible layer types.
 * \author tkornuta
 */
enum class LayerTypes : short
{
	Linear = 0,
	Pooling,
	Convolution,
	Sigmoid,
	Identity,
	ReLU,
	ELU,
	Softmax,
	Dropout,
	Padding,
	Regression
};


/// Forward declaration of MultiLayerNeuralNetwork
class MultiLayerNeuralNetwork;

inline float sqrt_eps(const float x) {
	return sqrtf(x + 1e-6);
}

/*!
 * abstract
 * \author krocki/tkornuta
 */
class Layer {
public:
	/*!
	 *
	 * @param inputs_size_
	 * @param outputs_size_
	 * @param batch_size_
	 * @param layer_type_
	 * @param label_
	 */
	Layer(size_t inputs_size_, size_t outputs_size_, size_t batch_size_, LayerTypes layer_type_, std::string label_ = "layer");

	/*!
	 * Abstract method responsible for processing the data from the inputs to outputs. To be overridden in the derived classes.
	 * @param test Test mode - used for dropout-alike regularization techniques.
	 */
	virtual void forward(bool test = false) = 0;

	/*!
	 * Abstract method responsible for processing the gradients from outputs to inputs (i.e. in the opposite direction). To be overridden in the derived classes.
	 */
	virtual void backward() = 0;

	//this is mainly for debugging - TODO: proper serialization of layers and object NN
	virtual void save_to_files(std::string prefix);

	virtual void resetGrads() {};


	virtual void applyGrads(double alpha_, double decay_) {};

	virtual ~Layer() {};

	// Duplicated entries fix, TO BE REMOVED when "proper serialization" will be implemented.
	void save_matrix_to_file(Eigen::MatrixXf& m, std::string filename);

	/// Returns size (length) of inputs.
	size_t inputsSize();

	/// Returns size (length) of outputs.
	size_t outputsSize();

	/// Returns size (length) of (mini)batch.
	size_t batchSize();

	/// Returns name of the layer.
	const std::string name() const {
		return layer_name;
	}

	/*!
	 * Returns the type of layer.
	 */
	const std::string type() const {
		switch(layer_type) {
		case(LayerTypes::Linear):
			return "Linear";
		case(LayerTypes::Pooling):
			return "Pooling";
		case(LayerTypes::Convolution):
			return "Convolution";
		case(LayerTypes::Sigmoid):
			return "Sigmoid";
		case(LayerTypes::Identity):
			return "Identity";
		case(LayerTypes::ReLU):
			return "ReLU";
		case(LayerTypes::ELU):
			return "ELU";
		case(LayerTypes::Softmax):
			return "Softmax";
		case(LayerTypes::Dropout):
			return "Dropout";
		case(LayerTypes::Padding):
			return "Padding";
		case(LayerTypes::Regression):
			return "Regression";
		default:
			return "Undefined";
		}//: switch
	}

	/*!
	 * Stream operator enabling to print neural network.
	 * @param os_ Ostream object.
	 * @param obj_ Tensor object.
	 */
	friend std::ostream& operator<<(std::ostream& os_, Layer& obj_) {
		// Display dimensions.
		os_ << "  [" << obj_.type() << "]: " << obj_.layer_name << ":\n";
		// Display inputs.
		os_ << "    [" << obj_.s.name() << "]:\n";
		for (auto& i: obj_.s.keys()) {
			// Display elements.
			os_ << "      [" << i.first << "]: ";
			os_ << (obj_.s[i.second])->cols() << "x" << (obj_.s[i.second])->rows() << std::endl;
		}//: for keys

		// Display gradients.
		os_ << "    [" << obj_.g.name() << "]:\n";
		for (auto& i: obj_.g.keys()) {
			// Display elements.
			os_ << "      [" << i.first << "]: ";
			os_ << (obj_.g[i.second])->cols() << "x" << (obj_.g[i.second])->rows() << std::endl;
		}//: for keys

		// Display parameters.
		os_ << "    [" << obj_.p.name() << "]:\n";
		for (auto& i: obj_.p.keys()) {
			// Display elements.
			os_ << "      [" << i.first << "]: ";
			os_ << (obj_.p[i.second])->cols() << "x" << (obj_.p[i.second])->rows() << std::endl;
		}//: for keys

		// Display gradients.
		os_ << "    [" << obj_.m.name() << "]:\n";
		for (auto& i: obj_.m.keys()) {
			// Display elements.
			os_ << "      [" << i.first << "]: ";
			os_ << (obj_.m[i.second])->cols() << "x" << (obj_.m[i.second])->rows() << std::endl;
		}//: for keys

		return os_;
	}

protected:

/*	//used in forward pass
	mic::types::MatrixXf x; //inputs
	mic::types::MatrixXf y; //outputs

	//grads, used in backward pass
	mic::types::MatrixXf dx;
	mic::types::MatrixXf dy;
*/

	/// Size (length) of inputs.
	const size_t inputs_size;

	/// Size (length) of outputs.
	const size_t outputs_size;

	/// Size (length) of (mini)batch.
	const size_t batch_size;

	/// Type of the layer.
	const LayerTypes layer_type;

	/// Name (identifier of the type) of the layer.
	const std::string layer_name;


	/// States - contains input [x] and output [y] matrices.
	mic::types::MatrixArray<float> s;

	/// Gradients - contains input [x] and output [y] matrices.
	mic::types::MatrixArray<float> g;

	/// Parameters - parameters of the layer, to be used by the derived classes.
	mic::types::MatrixArray<float> p;

	/// Memory - a list of temporal parameters, to be used by the derived classes.
	mic::types::MatrixArray<float> m;

	// Adds the nn class the access to protected fields of class layer.
	friend class MultiLayerNeuralNetwork;

};


} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_LAYER_HPP_ */
