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

#include <boost/serialization/serialization.hpp>
// include this header to serialize vectors
#include <boost/serialization/vector.hpp>
// include this header to serialize arrays
#include <boost/serialization/array.hpp>
#include <boost/serialization/version.hpp>

// Forward declaration of class boost::serialization::access
namespace boost {
namespace serialization {
class access;
}//: serialization
}//: access


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

	/*!
	 * Stream operator enabling to print neural network.
	 * @param os_ Ostream object.
	 * @param obj_ Tensor object.
	 */
	friend std::ostream& operator<<(std::ostream& os_, const MultiLayerNeuralNetwork& obj_) {
		// Display dimensions.
		os_ << "[" << obj_.name << "]:\n";
		// Display layers one by one.
		for (size_t i = 0; i < obj_.layers.size(); i++)
			os_ << (*obj_.layers[i]) << std::endl;

		return os_;
	}


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

private:
	// Friend class - required for using boost serialization.
    friend class boost::serialization::access;

    /*!
     * Serialization save - saves the neural net object to archive.
     * @param ar Used archive.
     * @param version Version of the neural net class (not used currently).
     */
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const {
        ar & name;
        // Serialize number of layers.
        size_t size = layers.size();
		std::cout << size << std::endl;
        ar & size;

		// Serialize layers one by one.
		for (size_t i = 0; i < layers.size(); i++) {
			// Serialize type first - so we can use it in load.
			ar & layers[i]->layer_type;

			// Register the type, so it can be used during the deserialization.
			switch(layers[i]->layer_type) {
			case(LayerTypes::Linear):
				ar.template register_type<mic::mlnn::Linear>();
				std::cout <<  "Linear\n"; break;
			case(LayerTypes::Pooling):
				std::cout <<  "Pooling"; break;
			case(LayerTypes::Convolution):
				std::cout <<  "Convolution"; break;
			case(LayerTypes::Sigmoid):
				std::cout <<  "Sigmoid"; break;
			case(LayerTypes::Identity):
				std::cout <<  "Identity"; break;
			case(LayerTypes::ReLU):
				std::cout <<  "ReLU"; break;
			case(LayerTypes::ELU):
				std::cout <<  "ELU"; break;
			case(LayerTypes::Softmax):
				std::cout <<  "Softmax"; break;
			case(LayerTypes::Dropout):
				std::cout <<  "Dropout"; break;
			case(LayerTypes::Padding):
				std::cout <<  "Padding"; break;
			case(LayerTypes::Regression):
				std::cout <<  "Regression"; break;
			default:
				std::cout <<  "Undefined";
			}//: switch


			// Serialize the layer.
			ar & (*layers[i]);
		}//: for

//        ar & boost::serialization::make_array<T>(data_ptr, elements);
    }

    /*!
     * Serialization load - loads the neural net object to archive.
     * @param ar Used archive.
     * @param version Version of the neural net class (not used currently).
     */
    template<class Archive>
    void load(Archive & ar, const unsigned int version) {
		ar & name;
		// Deserialize number of layers
		size_t size;
		ar & size;

		std::cout << "Odczytany: " << size << std::endl;

		// Serialize layers one by one.
		for (size_t i = 0; i < size; i++) {
			LayerTypes lt;
			// Get layer type
			ar & lt;

			std::shared_ptr<Layer> layer_ptr;
			switch(lt) {
			case(LayerTypes::Linear):
				//ar.template register_type<mic::mlnn::Linear>();
				layer_ptr = std::make_shared<Linear>(Linear());
				std::cout <<  "Linear\n"; break;
			case(LayerTypes::Pooling):
				std::cout <<  "Pooling\n"; break;
			case(LayerTypes::Convolution):
				std::cout <<  "Convolution"; break;
			case(LayerTypes::Sigmoid):
				std::cout <<  "Sigmoid\n"; break;
			case(LayerTypes::Identity):
				std::cout <<  "Identity\n"; break;
			case(LayerTypes::ReLU):
				layer_ptr = std::make_shared<ReLU>(ReLU());
				std::cout <<  "ReLU\n"; break;
			case(LayerTypes::ELU):
				std::cout <<  "ELU\n"; break;
			case(LayerTypes::Softmax):
				std::cout <<  "Softmax\n"; break;
			case(LayerTypes::Dropout):
				std::cout <<  "Dropout\n"; break;
			case(LayerTypes::Padding):
				std::cout <<  "Padding\n"; break;
			case(LayerTypes::Regression):
				layer_ptr = std::make_shared<Regression>(Regression());
				std::cout <<  "Regression\n"; break;
			default:
				std::cout <<  "Undefined\n";
			}//: switch

			ar & (*layer_ptr);
			layers.push_back(layer_ptr);
		}//: for

/*		ar & dimensions;
		// Allocate memory.
		if (data_ptr != nullptr)
			delete (data_ptr);
		data_ptr = new T[elements];
		ar & boost::serialization::make_array<T>(data_ptr, elements);*/
    }

     // The serialization must be splited as load requires to allocate the memory.
     BOOST_SERIALIZATION_SPLIT_MEMBER()

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
