/*!
 * \file MultiLayerNeuralNetwork.hpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#ifndef SRC_MLNN_MULTILAYERNEURALNETWORK_HPP_
#define SRC_MLNN_MULTILAYERNEURALNETWORK_HPP_

//#include <iomanip>

#include <types/MatrixTypes.hpp>

#include <mlnn/LayerTypes.hpp>

#include <mlnn/importer.h> // TO BE REMOVED, now required for datapoint type :]


namespace mic {
namespace mlnn {

class MultiLayerNeuralNetwork {
public:

	MultiLayerNeuralNetwork();

	virtual ~MultiLayerNeuralNetwork();

	std::deque<mic::mlnn::Layer*> layers;

	void forward(mic::types::MatrixXf& input_data, bool skip_dropout = false);

	void backward(mic::types::MatrixXf& t);

	void update(float alpha, float decay);

	void train(std::deque<datapoint>& data, float alpha, float decay, size_t iterations, size_t classes, size_t batch_size);

	float test(std::deque<datapoint>& data, size_t classes, size_t batch_size);


	/*!
	 * Trains the neural network with a given batch.
	 * @param encoded_batch_ Batch encoded in the form of matrix of size [sample_size x batch_size].
	 * @param encoded_targets_ Targets (labels) encoded in the form of matrix of size [label_size x batch_size].
	 * @param learning_rate_ The learning rate.
	 * @param weight_decay_ Weight decay.
	 */
	void train(mic::types::MatrixXfPtr encoded_batch_, mic::types::MatrixXfPtr encoded_targets_, float learning_rate_, float weight_decay_);

	/*!
	 * Tests the neural network with a given batch.
	 * @param encoded_batch_ Batch encoded in the form of matrix of size [sample_size x batch_size].
	 * @param encoded_targets_ Targets (labels) encoded in the form of matrix of size [label_size x batch_size].
	 * @return
	 */
	float test(mic::types::MatrixXfPtr encoded_batch_, mic::types::MatrixXfPtr encoded_targets_);

	/*!
	 * Returns the predictions (output of the forward processing).
	 * @param predictions_ Predictions in the form of a matrix of size [label_size x batch_size].
	 */
	mic::types::MatrixXfPtr getPredictions();


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


	/// Returns the size (length) of inputs of the last (i.e. previously added) layer.
	size_t lastLayerInputsSize();

	/// Returns the size (length) of outputs of the last (i.e. previously added) layer.
	size_t lastLayerOutputsSize();

	/// Returns the size (length) of (mini)batch of the last (i.e. previously added) layer.
	size_t lastLayerBatchSize();

	void save_to_files(std::string prefix);


};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_MULTILAYERNEURALNETWORK_HPP_ */
