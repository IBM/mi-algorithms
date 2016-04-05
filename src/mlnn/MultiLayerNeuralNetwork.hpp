/*!
 * \file MultiLayerNeuralNetwork.hpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#ifndef SRC_MLNN_MULTILAYERNEURALNETWORK_HPP_
#define SRC_MLNN_MULTILAYERNEURALNETWORK_HPP_

//#include <iomanip>

#include <mlnn/LayerTypes.hpp>

#include <mlnn/importer.h> // TO BE REMOVED, now required for datapoint type :]

namespace mic {
namespace mlnn {

class MultiLayerNeuralNetwork {
public:

	MultiLayerNeuralNetwork(size_t minibatch_size);

	virtual ~MultiLayerNeuralNetwork();

	std::deque<mic::mlnn::Layer*> layers;

	const size_t batch_size;

	void forward(mic::types::MatrixXf& input_data, bool apply_dropout = false);


	void backward(mic::types::MatrixXf& t);


	void update(float alpha, float decay);


	void train(std::deque<datapoint>& data, float alpha, float decay, size_t iterations, bool apply_dropout = false);


	/*****	TODO	*****/
	/*		Eigen::MatrixXf compute_numerical_gradients(void) { }*/
	float test(std::deque<datapoint>& data);

	/*TODO: serialization*/
	void save_to_files(std::string prefix);

};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_MULTILAYERNEURALNETWORK_HPP_ */
