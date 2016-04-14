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

	void forward(mic::types::MatrixXf& input_data, bool skip_dropout = false);

	void backward(mic::types::MatrixXf& t);

	void update(float alpha, float decay);

	void train(std::deque<datapoint>& data, float alpha, float decay, size_t iterations, size_t classes);

	float test(std::deque<datapoint>& data, size_t classes);

	/*!
	 * Calculates the cross entropy.
	 * @param predictions_ Predictions in the form of a matrix of answers, each encoded as SDR.
	 * @param targets_ Desired results (targets) in the form of a matrix of answers, each encoded as SDR.
	 * @return
	 */
	float calculateCrossEntropy(mic::types::MatrixXf& predictions_, mic::types::MatrixXf& targets_);

	/*!
	 * Calculated difference between the predicted and target classes.
	 * Assumes 1-ouf-of-k encoding of classes.
	 *
	 * @param predictions_ Predictions in the form of a matrix of answers, each encoded as SDR.
	 * @param targets_ Desired results (targets) in the form of a matrix of answers, each encoded as SDR.
	 * @return
	 */
	size_t countCorrectPredictions(mic::types::MatrixXf& predictions_, mic::types::MatrixXf& targets_);


	void save_to_files(std::string prefix);

	/// Returns size (length) of inputs of the last (i.e. previously added) layer.
	size_t lastLayerInputsSize();

	/// Returns size (length) of outputs of the last (i.e. previously added) layer.
	size_t lastLayerOutputsSize();

	/// Returns size (length) of (mini)batch of the last (i.e. previously added) layer.
	size_t lastLayerBatchSize();

};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_MULTILAYERNEURALNETWORK_HPP_ */
