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


#include <mlnn/nn_utils.h> // TODO: dispatch and destroy!

namespace mic {
namespace mlnn {

/// Forward declaration of MultiLayerNeuralNetwork
class MultiLayerNeuralNetwork;

/*!
 * abstract
 * \author krocki
 */
class Layer {
public:
	Layer(size_t inputs_size_, size_t outputs_size_, size_t batch_size_, std::string label_ = "layer");

	//need to override these

	/*		TODO: allow 2 versions of forward (with 0 and 1 params),
			currently you must override version with 1 				*/

	virtual void forward(bool test = false) = 0;
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

	/// Returns Id (name) of the layer.
	std::string id() {
		return name;
	}

protected:

/*	//used in forward pass
	mic::types::MatrixXf x; //inputs
	mic::types::MatrixXf y; //outputs

	//grads, used in backward pass
	mic::types::MatrixXf dx;
	mic::types::MatrixXf dy;
*/

	/// Name (identifier of the type) of the layer.
	const std::string name;

	/// Size (length) of inputs.
	const size_t inputs_size;

	/// Size (length) of outputs.
	const size_t outputs_size;

	/// Size (length) of (mini)batch.
	const size_t batch_size;

	/// States - contains input [x] and output [y] matrices.
	mic::types::MatrixArray<float> s;

	/// Gradients - contains input [x] and output [y] matrices.
	mic::types::MatrixArray<float> g;

	/// Parameters - parameters of the layer, to be used by the derived classes.
	mic::types::MatrixArray<float> p;

	/// Memory - a list of temporal parameters, to be used by the derived classes.
	mic::types::MatrixArray<float> m;

	// Add access to protected fields to the nn class.
	friend class MultiLayerNeuralNetwork;

};


} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_LAYER_HPP_ */
