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

/*#include <boost/serialization/serialization.hpp>
// include this header to serialize vectors
#include <boost/serialization/vector.hpp>
// include this header to serialize arrays
#include <boost/serialization/array.hpp>
#include <boost/serialization/version.hpp>*/

#include <fstream>
// Include headers that implement a archive in simple text format
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <logger/Log.hpp>

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
	 * @tparam layer_ptr_ Layer type.
	 */
	template <typename LayerType>
	void addLayer( LayerType* layer_ptr_){
		layers.push_back(std::shared_ptr <LayerType> (layer_ptr_));
		connected = false;
	}

	/*!
	 * Returns n-th layer of the neural network.
	 * @param layer_ptr_ Pointer to the newly created layer.
	 * @tparam layer_ptr_ Layer type.
	 */
	template <typename LayerType>
	std::shared_ptr<LayerType> getLayer(size_t index_){
		assert(index_ < layers.size());
		/*std::shared_ptr<Layer> layer = (layers[index_]);
		LayerType* l_ptr = (LayerType*)layer.get();
		return std::shared_ptr <LayerType> (l_ptr);*/

		// Cast the pointer to LayerType.
		return std::dynamic_pointer_cast< LayerType >( layers[index_] );
		//return layers[index_];
	}


	/*!
	 * Removes the last neural network layer.
	 */
	void removeLastLayer( ){
		layers.pop_back();
		connected = false;
	}

	/*!
	 * Removes several last layers of the neural network.
	 * @param number_of_layers_ Number of layers to be removed.
	 */
	void removeLastLayers(size_t number_of_layers_){
		assert(number_of_layers_ <= layers.size());
		//layers.erase(layers.back() - number_of_layers_, layers.back());
		for (size_t i=0; i <number_of_layers_; i++)
			layers.pop_back();
		connected = false;
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
	size_t countCorrectPredictions(mic::types::MatrixXfPtr targets_, mic::types::MatrixXfPtr predictions_);

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


	/*!
	 * Saves network to file using serialization.
	 * @param filename_ Name of the file.
	 */
	bool save(std::string filename_)
	{
		try {
			// Create an output archive
			std::ofstream ofs(filename_);
			boost::archive::text_oarchive ar(ofs);
			// Write data
			ar & (*this);
			LOG(LINFO) << "Network " << name << " properly saved to file " << filename_;
			LOG(LDEBUG) << "Saved network: \n" << (*this);
		} catch(...) {
			LOG(LERROR) << "Could not write neural network " << name << " to file " << filename_ << "!";
			// Clear layers - just in case.
			layers.clear();
			loss_type = LossFunctionType::Undefined;
			return false;
		}
		return true;
	}

	/*!
	 * Loads network from the file using serialization.
	 * @param filename_ Name of the file.
	 */
	bool load(std::string filename_)
	{
		try {
			// Create and input archive
			std::ifstream ifs(filename_);
			boost::archive::text_iarchive ar(ifs);
			// Load data
			ar & (*this);
			LOG(LINFO) << "Network " << name << " properly loaded from file " << filename_;
			LOG(LDEBUG) << "Loaded network: \n" << (*this);
		} catch(...) {
			LOG(LERROR) << "Could not load neural network from file " << filename_ << "!";
			// Clear layers - just in case.
			layers.clear();
			loss_type = LossFunctionType::Undefined;
			return false;
		}
		return true;
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

    /// Flag denoting whether the layers are interconnected, thus no copying between inputs and outputs of the neighbouring layers will be required.
    bool connected;

    /*!
     * Serialization save - saves the neural net object to archive.
     * @param ar Used archive.
     * @param version Version of the neural net class (not used currently).
     */
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const {
        ar & name;
		ar & loss_type;
		// Serialize number of layers.
        size_t size = layers.size();
        ar & size;

		// Serialize layers one by one.
		for (size_t i = 0; i < layers.size(); i++) {
			// Serialize type first - so we can use it in load.
			ar & layers[i]->layer_type;

			// Serialize the layer.
			ar & (*layers[i]);
		}//: for

    }

    /*!
     * Serialization load - loads the neural net object to archive.
     * @param ar Used archive.
     * @param version Version of the neural net class (not used currently).
     */
    template<class Archive>
    void load(Archive & ar, const unsigned int version) {
    	// Clear the layers vector - just in case.
    	layers.clear();
    	connected = false;

    	// Deserialize name and loss function type.
		ar & name;
		ar & loss_type;

		// Deserialize number of layers.
		size_t size;
		ar & size;

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
				LOG(LDEBUG) <<  "Linear";
				break;
			case(LayerTypes::Pooling):
				layer_ptr = std::make_shared<Pooling>(Pooling());
				LOG(LERROR) <<  "Pooling Layer serialization not implemented (some params are not serialized)!";
				break;
			case(LayerTypes::Convolution):
				layer_ptr = std::make_shared<Convolution>(Convolution());
				LOG(LERROR) <<  "Convolution Layer serialization not implemented (some params are not serialized)!";
				break;
			case(LayerTypes::Sigmoid):
				layer_ptr = std::make_shared<Sigmoid>(Sigmoid());
				LOG(LDEBUG) <<  "Sigmoid";
				break;
			case(LayerTypes::Identity):
				layer_ptr = std::make_shared<Identity>(Identity());
				LOG(LDEBUG) <<  "Identity";
				break;
			case(LayerTypes::ReLU):
				layer_ptr = std::make_shared<ReLU>(ReLU());
				LOG(LDEBUG) <<  "ReLU";
				break;
			case(LayerTypes::ELU):
				layer_ptr = std::make_shared<ELU>(ELU());
				LOG(LDEBUG) <<  "ELU";
				break;
			case(LayerTypes::Softmax):
				layer_ptr = std::make_shared<Softmax>(Softmax());
				LOG(LDEBUG) <<  "Softmax";
				break;
			case(LayerTypes::Dropout):
				layer_ptr = std::make_shared<Dropout>(Dropout());
				LOG(LERROR) <<  "Dropout Layer serialization not implemented (some params are not serialized)!";
				break;
			case(LayerTypes::Padding):
				layer_ptr = std::make_shared<Padding>(Padding());
				LOG(LERROR) <<  "Padding Layer serialization not implemented (some params are not serialized)!";
				break;
			case(LayerTypes::Regression):
				layer_ptr = std::make_shared<Regression>(Regression());
				LOG(LDEBUG) <<  "Regression";
				break;
			default:
				LOG(LERROR) <<  "Undefined Layer type detected during deserialization!";
			}//: switch

			ar & (*layer_ptr);
			layers.push_back(layer_ptr);
		}//: for

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
