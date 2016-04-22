/*!
 * \file MultiLayerNeuralNetwork.hpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#ifndef SRC_MLNN_MULTILAYERNEURALNETWORK_HPP_
#define SRC_MLNN_MULTILAYERNEURALNETWORK_HPP_

#include <types/MatrixTypes.hpp>

#include <mlnn/LayerTypes.hpp>


namespace mic {
namespace mlnn {

/*!
 * \brief Enumeration of possible loss function types.
 * \author tkornuta
 */
enum class LossFunctionType : short
{
	Undefined = -1, ///< Loss function undefined!
	RegressionQuadratic = 0, ///< Quadratic cost function used in regression
	ClassificationEntropy = 1, ///< Entropy used in classification
};

/*!
 * \brief Class representing a multi-layer neural network.
 * \author krocki/tkornuta
 */
class MultiLayerNeuralNetwork {
public:

	/*!
	 * Constructor. Sets the neural network name.
	 * @param name_ Name of the network.
	 */
	MultiLayerNeuralNetwork(std::string name_ = "mlnn");

	/*!
	 * Destructor.
	 */
	virtual ~MultiLayerNeuralNetwork();

	/*!
	 * Adds layer to neural network.
	 * @param layer_ptr_ Pointer to the newly created layer.
	 * @tparam layer_ptr_ Pointer to the newly created layer.
	 */
	template <typename LayerType>
	void addLayer( LayerType* layer_ptr_){
		layers.push_back(std::shared_ptr <LayerType> (layer_ptr_));
	}

	/*!
	 * Passes the data in a feed-forward manner through all consecutive layers, from the input to the output layer.
	 * @param input_data Input data - a matrix containing [sample_size x batch_size].
	 * @param skip_dropout Flag for skipping dropouts - which should be set to true during testing.
	 */
	void forward(mic::types::MatrixXf& input_data, bool skip_dropout = false);

	/*!
	 * Performs the back propagation
	 * @param targets_ The targer matrix, containing target (desired) outputs of the network [encoded_label_size x batch_size]
	 */
	void backward(mic::types::MatrixXf& targets_);

	/*!
	 * Performs the network training by update all layers parameters according to gradients computed by backprob.
	 * @param alpha
	 * @param decay
	 */
	void update(float alpha, float decay);

	/*!
	 * Trains the neural network with a given batch.
	 * @param encoded_batch_ Batch encoded in the form of matrix of size [sample_size x batch_size].
	 * @param encoded_targets_ Targets (labels) encoded in the form of matrix of size [label_size x batch_size].
	 * @param learning_rate_ The learning rate.
	 * @param weight_decay_ Weight decay.
	 * @return Loss computed according to the selected loss function. If function not set - returns INF.
	 */
	float train(mic::types::MatrixXfPtr encoded_batch_, mic::types::MatrixXfPtr encoded_targets_, float learning_rate_, float weight_decay_);

	/*!
	 * Tests the neural network with a given batch.
	 * @param encoded_batch_ Batch encoded in the form of matrix of size [sample_size x batch_size].
	 * @param encoded_targets_ Targets (labels) encoded in the form of matrix of size [label_size x batch_size].
	 * @return Loss computed according to the selected loss function. If function not set - returns INF.
	 */
	float test(mic::types::MatrixXfPtr encoded_batch_, mic::types::MatrixXfPtr encoded_targets_);

	/*!
	 * Calculates the loss function according to the selected function type.
	 * @param encoded_targets_ Targets (labels) encoded in the form of pointer to matrix of size [label_size x batch_size].
	 * @param encoded_predictions_ Predicted outputs of the network encoded in the form of pointer to matrix of size [label_size x batch_size].
	 * @return Loss computed according to the selected loss function. If function not set - returns INF.
	 */
	float calculateLossFunction(mic::types::MatrixXfPtr encoded_targets_, mic::types::MatrixXfPtr encoded_predictions_);

	/*!
	 * Returns the predictions (output of the forward processing).
	 * @param predictions_ Predictions in the form of a matrix of size [label_size x batch_size].
	 */
	mic::types::MatrixXfPtr getPredictions();

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

protected:
	/*!
	 * Contains a list of consecutive layers.
	 */
	std::vector<std::shared_ptr <mic::mlnn::Layer> > layers;

	/*!
	 * Name of the neural network.
	 */
	std::string name;

	/*!
	 * Type of the used loss function.
	 */
	LossFunctionType loss_type;
};


/*!
 * Adds layer to neural network - template method specialization for the Regression layer - sets the adequate loss function.
 * @param layer_ptr_ Pointer to the newly created layer.
 * @tparam layer_ptr_ Pointer to the newly created layer.
 */
template <>
void MultiLayerNeuralNetwork::addLayer<Regression>( mic::mlnn::Regression* layer_ptr_){
	layers.push_back(std::shared_ptr <Regression> (layer_ptr_));
	loss_type = LossFunctionType::RegressionQuadratic;
}

/*!
 * Adds layer to neural network - template method specialization for the Softmax layer - sets the adequate loss function.
 * @param layer_ptr_ Pointer to the newly created layer.
 * @tparam layer_ptr_ Pointer to the newly created layer.
 */
template <>
void MultiLayerNeuralNetwork::addLayer<Softmax>( mic::mlnn::Softmax* layer_ptr_){
	layers.push_back(std::shared_ptr <Softmax> (layer_ptr_));
	loss_type = LossFunctionType::ClassificationEntropy;
}


} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_MULTILAYERNEURALNETWORK_HPP_ */
