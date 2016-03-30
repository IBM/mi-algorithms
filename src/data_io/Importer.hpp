/*!
 * \file Importer.hpp
 * \brief Contains declaration (and definition) of base template class of all data importers.
 * \author tkornuta
 * \date Dec 21, 2015
 */

#ifndef SRC_DATA_IO_IMPORTER_HPP_
#define SRC_DATA_IO_IMPORTER_HPP_

#include <vector>
#include <stdexcept>

#include <random>

#include <logger/Log.hpp>
#include <configuration/PropertyTree.hpp>

namespace mic {

/**
 * \namespace mic::data_io
 * \brief Namespace containing data import/export classess and procedures.
 * \author tkornuta
 */
namespace data_io {

/*!
 * @brief Parent class for all data importers.
 *
 * @author tkornuta
 * @tparam dataType Template parameter defining the imported datatype.
 * @tparam labelType Template parameters defining the datatype of label.
 */
template <typename dataType, typename labelType>
class Importer : public configuration::PropertyTree {
public:
	/*!
	 * Constructor. Initializes random generator and sets index to zero. Registers index property.
	 * @param node_name_ Name of the node in configuration file.
	 */
	Importer(std::string node_name_, size_t batch_size_ = 1) : PropertyTree(node_name_),
		index("index",0),
		batch_size("batch_size", batch_size_),
		rng_mt19937_64(rd())
	{
		// Register properties - so their values can be overridden (read from the configuration file).
		registerProperty(index);
		registerProperty(batch_size);
	}

	/*!
	 * Virtual destructor.
	 */
	virtual ~Importer() {
	}

	/*!
	 * Abstract method responsible for importing/loading the data.
	 * @return TRUE if data loaded successfully, FALSE otherwise.
	 */
	virtual bool importData() = 0;

	/*!
	 * Returns the imported data.
	 */
	std::vector< std::shared_ptr<dataType> > & getData() {
		return data;
	}

	/*!
	 * Returns the imported labels.
	 */
	std::vector< std::shared_ptr<labelType> > & getLabels() {
		return labels;
	}

	/*!
	 * Picks a random sample from the dataset (the same sample can be selected many times - n-tuples).
	 * @return Sample - a pair of <shared pointers to samples and labels>.
	 */
	std::pair<std::shared_ptr<dataType>, std::shared_ptr<labelType> > getRandomSample() {

		// Initialize uniform index distribution - integers.
		std::uniform_int_distribution<> index_dist(0, data.size()-1);

		// Pick an index.
		unsigned int tmp_index= index_dist(rng_mt19937_64);

		LOG(LDEBUG) << "data size = " << data.size() << " labels size = " << labels.size() << " index = " << tmp_index;

		// Return data.
		return getSample(tmp_index);
	}

	/*!
	 * Iterates through samples and returns them one by one.
	 * After returning the last element from the dataset the procedure starts from the beginning.
	 * This behaviour can be avoided by manualy calling the isLastSample() method.
	 * @return Sample - a pair of <shared pointers to samples and labels>.
	 */
	std::pair<std::shared_ptr<dataType>, std::shared_ptr<labelType> > getNextSample() {

		// Check index.
		if(index >= data.size()){
			// Reset index.
			index = 0;
		}
		// Return given sample and increment index afterwards.
		std::pair<std::shared_ptr<dataType>, std::shared_ptr<labelType> > sample = getSample(index);
		index += 1; 
		return getSample(sample);
	}

	/*!
	 * Returns sample with given index. If index is out of dataset range throws an "std::out_of_range" exception.
	 * @param index_ Sample index.
	 * @return Sample - a pair of <shared pointers to samples and labels>.
	 */
	std::pair<std::shared_ptr<dataType>, std::shared_ptr<labelType> > getSample(size_t index_) {

		// Check index.
		if ((index_ < 0) || (index_ >= data.size())){
			// Reset index.
			throw std::out_of_range("getSample index out of range");
		}//: if

		// Get data
		std::shared_ptr<dataType> data_ptr = data[index_];
		std::shared_ptr<labelType> label_ptr = labels[index_];

		// Return data.
		return std::make_pair (data_ptr, label_ptr);

	}


	/*!
	 * Returns a batch of random samples.
	 * @return Batch - a pair of vectors of <shared pointers to samples> / vectors of <shared pointers to labels>.
	 */
	std::pair< std::vector< std::shared_ptr<dataType> >, std::vector< std::shared_ptr<labelType> > > getRandomBatch() {

		// Initialize uniform index distribution - integers.
		std::uniform_int_distribution<> index_dist(0, data.size()-1);

		std::vector<size_t> indices;
		for (size_t i=0; i<batch_size; i++) {
			// Pick an index.
			indices.push_back((size_t)index_dist(rng_mt19937_64));

		}//: batch_size

		// Return data.
		return getBatch(indices);
	}


	/*!
	 * Iterates through samples and returns them batch by batch.
	 * After returning the last possible batch from the dataset the procedure starts from the beginning.
	 * This behaviour can be avoided by manualy calling the isLastBatch() method.
	 * @return Batch - a pair of vectors of <shared pointers to samples> / vectors of <shared pointers to labels>.
	 */
	std::pair< std::vector< std::shared_ptr<dataType> >, std::vector< std::shared_ptr<labelType> > > getNextBatch() {

		// Check index.
		if((index+batch_size) >= data.size()){
			// Reset index.
			index = 0;
		}
		// Generate list of indices.
		std::vector<size_t> indices;
		for (size_t i=0; i<batch_size; i++) {
			// Pick an index.
			indices.push_back((size_t)(index+i));

		}//: batch_size

		// Increment index.
		index = (size_t)index + (size_t)batch_size;
		// Return data.
		return getBatch(indices);
	}


	/*!
	 * Returns batch of samples with given indices.
	 * If any of the indices is out of dataset range throws an "std::out_of_range" exception.
	 * @param indices_ Vector of indices
	 * @return Batch - a pair of vectors of <shared pointers to samples> / vectors of <shared pointers to labels>.
	 */
	std::pair< std::vector< std::shared_ptr<dataType> >, std::vector< std::shared_ptr<labelType> > > getBatch(std::vector<size_t> indices_) {

		// Empty two vectors that will be combined into batch at the end.
		std::vector< std::shared_ptr<dataType> > batch_data;
		std::vector< std::shared_ptr<labelType> > batch_labels;

		// For all indices.
		for (size_t local_index: indices_) {
			// Check index.
			if ((local_index < 0) || (local_index >= data.size())){
				// Reset index.
				throw std::out_of_range("getSample index out of range");
			}//: if

			// Get data
			std::shared_ptr<dataType> data_ptr = data[local_index];
			std::shared_ptr<labelType> label_ptr = labels[local_index];

			// Add sample to vectors.
			batch_data.push_back(data_ptr);
			batch_labels.push_back(label_ptr);
		}
		// Return batch.
		return std::make_pair (batch_data, batch_labels);
	}

	/*!
	 * Sets the index of the next returned sample.
	 * @param index_ Sample index.
	 */
	void setSampleIndex(size_t index_ = 0) {
		index = index_;
	}

	/*!
	 * Sets the batch size.
	 * @param batch_size_ Batch size.
	 */
	void setBatchSize(size_t batch_size_ = 1) {
		batch_size = batch_size_;
	}

	/*!
	 * Checks if the returned sample was the last one.
	 * @return True if the sample was the last one.
	 */
	bool isLastSample() {
		return (index >= data.size());
	}

	/*!
	 * Checks if the returned batch was the last possible one.
	 * @return True if the batch was the last one.
	 */
	bool isLastBatch() {
		return ((index+batch_size) >= data.size());
	}

protected:
	/*!
	 * Contains the imported data.
	 */
	std::vector< std::shared_ptr<dataType> > data;

	/*!
	 * Contains the imported labels.
	 */
	std::vector< std::shared_ptr<labelType> > labels;

	/*!
	 * Property: index of the returned sample - it is used ONLY in getNextSample (i.e. iterative, not random sampling) method.
	 */
	mic::configuration::Property<size_t> index;

	/*!
	 * Batch size. As defaults set to one.
	 */
	mic::configuration::Property<size_t> batch_size;

	/*!
	 * Random device used for generation of random numbers.
	 */
	std::random_device rd;

	/*!
	 *  Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits.
	 */
	std::mt19937_64 rng_mt19937_64;


};

} /* namespace data_io */
} /* namespace mic */

#endif /* SRC_DATA_IO_IMPORTER_HPP_ */
