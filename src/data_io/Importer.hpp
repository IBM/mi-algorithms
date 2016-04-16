/*!
 * \file Importer.hpp
 * \brief Contains declaration (and definition) of base template class of all data importers.
 * \author tkornuta
 * \date Dec 21, 2015
 */

#ifndef SRC_DATA_IO_IMPORTER_HPP_
#define SRC_DATA_IO_IMPORTER_HPP_

#include <vector>
#include <list>
#include <stdexcept>

#include <random>

#include <logger/Log.hpp>
#include <configuration/PropertyTree.hpp>

#include <types/Batch.hpp>

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
 * @tparam DataType Template parameter defining the imported datatype.
 * @tparam LabelType Template parameters defining the datatype of label.
 */
template <typename DataType, typename LabelType>
class Importer : public mic::types::Batch<DataType, LabelType>, public mic::configuration::PropertyTree {
public:
	/*!
	 * Constructor. Initializes random generator and sets index to zero. Registers index property.
	 * @param node_name_ Name of the node in configuration file.
	 * @param batch_size_ Size of the batch.
	 */
	Importer(std::string node_name_, size_t batch_size_ = 1) : mic::types::Batch<DataType, LabelType>::Batch(), PropertyTree(node_name_),
		next_sample_index("next_sample_index",0),
		batch_size("batch_size", batch_size_),
		rng_mt19937_64(rd())
	{
		// Register properties - so their values can be overridden (read from the configuration file).
		registerProperty(next_sample_index);
		registerProperty(batch_size);
	}

	/*!
	 * Virtual destructor.
	 */
	virtual ~Importer() {
	}

	/*!
	 * Abstract method responsible for importing/loading the samples.
	 * @return TRUE if data loaded successfully, FALSE otherwise.
	 */
	virtual bool importData() = 0;

	/*!
	 * Picks a random sample from the dataset (the same sample can be selected many times - n-tuples).
	 * @return Sample containing shared pointer to sample data, its label and sample number.
	 */
	mic::types::Sample<DataType, LabelType> getRandomSample() {

		// Initialize uniform index distribution - integers.
		std::uniform_int_distribution<> index_dist(0, this->sample_data.size()-1);

		// Pick an index.
		unsigned int tmp_index= index_dist(rng_mt19937_64);

		LOG(LDEBUG) << "data size = " << this->sample_data.size() << " labels size = " << this->sample_labels.size() << " index = " << tmp_index;

		// Return data.
		return this->getSampleDirect(tmp_index);
	}

	/*!
	 * Iterates through samples and returns them one by one.
	 * After returning the last element from the dataset the procedure starts from the beginning.
	 * This behaviour can be avoided by manualy calling the isLastSample() method.
	 * @return Sample containing shared pointer to sample data, its label and sample number.
	 */
	mic::types::Sample<DataType, LabelType> getNextSample() {

		// Check index.
		if(next_sample_index >= this->sample_data.size()){
			// Reset index.
			next_sample_index = 0;
		}
		// Return given sample and increment index afterwards.
		mic::types::Sample<DataType, LabelType > sample = this->getSampleDirect(next_sample_index);
		next_sample_index += 1;
		return sample;
	}


	/*!
	 * Returns a batch of random samples.
	 * @return Batch - a pair of vectors of <shared pointers to samples> / vectors of <shared pointers to labels>, supplemented by third vector containing sample numbers.
	 */
	mic::types::Batch<DataType, LabelType> getRandomBatch() {

		// Initialize uniform index distribution - integers.
		std::uniform_int_distribution<> index_dist(0, this->sample_data.size()-1);

		std::vector<size_t> tmp_indices;
		for (size_t i=0; i<batch_size; i++) {
			// Pick an index.
			tmp_indices.push_back((size_t)index_dist(rng_mt19937_64));

		}//: batch_size

		// Return data.
		return this->getBatchDirect(tmp_indices);
	}


	/*!
	 * Iterates through samples and returns them batch by batch.
	 * After returning the last possible batch from the dataset the procedure starts from the beginning.
	 * This behaviour can be avoided by manually calling the isLastBatch() method.
	 * @return Batch - a pair of vectors of <shared pointers to samples> / vectors of <shared pointers to labels>, supplemented by third vector containing sample numbers.
	 */
	mic::types::Batch<DataType, LabelType> getNextBatch() {

		// Check index.
		if((next_sample_index+batch_size) >= this->sample_data.size()){
			// Reset index.
			next_sample_index = 0;
		}
		// Generate list of indices.
		std::vector<size_t> indices;
		for (size_t i=0; i<batch_size; i++) {
			// Pick an index.
			indices.push_back((size_t)(next_sample_index+i));

		}//: batch_size

		// Increment index.
		next_sample_index = (size_t)next_sample_index + (size_t)batch_size;
		// Return data.
		return this->getBatchDirect(indices);
	}


	/*!
	 * Sets the index of the next returned sample.
	 * @param index_ Sample index.
	 */
	void setNextSampleIndex(size_t index_ = 0) {
		next_sample_index = index_;
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
		return (next_sample_index >= this->sample_data.size());
	}

	/*!
	 * Checks if the returned batch was the last possible one.
	 * @return True if the batch was the last one.
	 */
	bool isLastBatch() {
		return ((next_sample_index+batch_size) >= this->sample_data.size());
	}

	/*!
	 * Counts the distinctive classes.
	 * Quite slow method, but works.
	 */
	void countClasses() {
		std::list<LabelType> classes;
		for(auto label: this->sample_labels) {
			classes.push_back(*label);
		}//: for
		classes.sort();
		classes.unique();
		this->number_of_classes = classes.size();
	}


protected:
	/*!
	 * Property: index of the returned sample - it is used ONLY in getNextSample (i.e. iterative, not random sampling) method.
	 */
	mic::configuration::Property<size_t> next_sample_index;

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
