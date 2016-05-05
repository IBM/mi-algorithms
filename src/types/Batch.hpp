/*!
 * \file Batch.hpp
 * \brief 
 * \author tkornut
 * \date Apr 14, 2016
 */

#ifndef SRC_TYPES_BATCH_HPP_
#define SRC_TYPES_BATCH_HPP_

#include <types/Sample.hpp>

#include <random>

#include <list>


namespace mic {
namespace types {

/*!
 * \brief Template class storing the sample batches.
 * A batch is stored in fact as three vectors, containing data, labels and sample numbers respectively.
 * @tparam DataType Template parameter defining the sample data type.
 * @tparam LabelType Template parameters defining the sample label label.
 * \author tkornuta
 */
template<typename DataType, typename LabelType>
class Batch {
public:
	/*!
	 * Empty constructor.
	 */
	Batch(size_t batch_size_ = 1) :
		next_sample_index(0),
		batch_size(batch_size_),
		rng_mt19937_64(rd())
	{

	}

	/*!
	 * Copy constructor.
	 * @param batch_ Batch to be copied.
	 */
	Batch(const mic::types::Batch<DataType, LabelType>& batch_) :
		rng_mt19937_64(rd())
	{
		// Copy parameters.
		next_sample_index = batch_.next_sample_index;
		batch_size = batch_.batch_size;

		// Copy "samples".
		this->sample_data = batch_.sample_data;
		this->sample_labels = batch_.sample_labels;
		this->sample_indices = batch_.sample_indices;
		// And number of classes.
		this->number_of_classes = batch_.number_of_classes;
	}

	/*!
	 * Assignment operator.
	 * @param batch_ Batch to be copied.
	 * @return An exact copy of the input batch.
	 */
	mic::types::Batch<DataType, LabelType>& operator =(const mic::types::Batch<DataType, LabelType>& batch_) {
		// Copy "samples".
		this->sample_data = batch_.sample_data;
		this->sample_labels = batch_.sample_labels;
		this->sample_indices = batch_.sample_indices;
		// And number of classes.
		this->number_of_classes = batch_.number_of_classes;
		// Return the object.
		return *this;
	}

	/*!
	 * Destructor. Empty for now.
	 */
	virtual ~Batch() { };

	/// Returns sample data.
	std::vector <std::shared_ptr<DataType> > & data() {
		return sample_data;
	}

	/// Returns sample labels.
	std::vector <std::shared_ptr<LabelType> > & labels() {
		return sample_labels;
	}

	/// Returns sample numbers (sample "positions" in original dataset).
	std::vector <size_t> & indices() {
		return sample_indices;
	}

	/*!
	 * Returns data from selected sample.
	 * @param index_ Index of the sample from the batch.
	 * @return Data.
	 */
	std::shared_ptr<DataType> data(size_t index_) {
		return sample_data[index_];
	}

	/*!
	 * Returns label of the selected sample.
	 * @param index_ Index of the sample from the batch.
	 * @return Sample label
	 */
	std::shared_ptr<LabelType> labels(size_t index_) {
		return sample_labels[index_];
	}

	/*!
	 * Returns sample number (sample "position" in original dataset).
	 * @param index_ Index of the sample from the batch.
	 * @return Sample number.
	 */
	std::vector <size_t> indices(size_t index_) {
		return sample_indices[index_];
	}

	/*!
	 * Returns the size of the batch.
	 * @return Batch size.
	 */
	size_t size() {
		return sample_data.size();
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
	 * Returns batch of samples with given indices.
	 * If any of the indices is out of dataset range throws an "std::out_of_range" exception.
	 * @param indices_ Vector of indices
	 * @return Batch - a pair of vectors of <shared pointers to samples> / vectors of <shared pointers to labels>, supplemented by third vector containing sample numbers.
	 */
	mic::types::Batch<DataType, LabelType> getBatch(std::vector<size_t> indices_) {

		// New empty batch.
		mic::types::Batch<DataType, LabelType> batch;
		// Set number of classes.
		batch.number_of_classes = number_of_classes;

		// For all indices.
		for (size_t local_index: indices_) {
			// Try to add sample to batch.
			batch.add(getSample(local_index));
		}
		// Return batch.
		return batch;
	}


	/*!
	 * Returns batch of samples with given positions in the batch (thus they can have different indices i.e. positions in the original dataset!).
	 * If any of the indices is out of dataset range throws an "std::out_of_range" exception.
	 * @param indices_ Vector of indices
	 * @return Batch - a pair of vectors of <shared pointers to samples> / vectors of <shared pointers to labels>, supplemented by third vector containing sample numbers.
	 */
	mic::types::Batch<DataType, LabelType> getBatchDirect(std::vector<size_t> indices_) {

		// New empty batch.
		mic::types::Batch<DataType, LabelType> batch;

		// For all indices.
		for (size_t local_index: indices_) {
			// Try to add sample to batch.
			batch.add(getSampleDirect(local_index));
		}
		// Return batch.
		return batch;
	}


	/*!
	 * Sets the batch size.
	 * @param batch_size_ Batch size.
	 */
	void setBatchSize(size_t batch_size_ = 1) {
		batch_size = batch_size_;
	}

	/*!
	 * Checks if the returned batch was the last possible one.
	 * @return True if the batch was the last one.
	 */
	bool isLastBatch() {
		return ((next_sample_index+batch_size) >= this->sample_data.size());
	}


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
	 * Returns sample with given index. If index is out of batch range throws an "std::out_of_range" exception.
	 * @param index_ Sample index.
	 * @return Sample containing shared pointer to sample data, its label and sample number.
	 */
	mic::types::Sample<DataType, LabelType> getSample(size_t index_) {

		// Check index.
		if ((index_ < 0) || (index_ >= sample_data.size())){
			// Reset index.
			throw std::out_of_range("Sample index out of range!");
		}//: if

		// Check sample index.
		size_t sample_index = sample_indices[index_];
		if ((sample_index < 0) || (sample_index >= sample_data.size())){
			// Reset index.
			throw std::out_of_range("Invalid sample index!");
		}//: if

		// Get data
		std::shared_ptr<DataType> data_ptr = sample_data[sample_index];
		std::shared_ptr<LabelType> label_ptr = sample_labels[sample_index];

		// Return data.
		return Sample<DataType, LabelType> (data_ptr, label_ptr, sample_index);
	}

	/*!
	 * Returns sample being in a given position in the batch (thus it can have different index i.e. position in the original dataset!).
	 * If index is out of batch range throws an "std::out_of_range" exception.
	 * @param index_ Sample index.
	 * @return Sample containing shared pointer to sample data, its label and sample number.
	 */
	mic::types::Sample<DataType, LabelType> getSampleDirect(size_t index_) {

		// Check index.
		if ((index_ < 0) || (index_ >= sample_data.size())){
			// Reset index.
			throw std::out_of_range("Sample index out of range!");
		}//: if

		// Get data
		std::shared_ptr<DataType> data_ptr = sample_data[index_];
		std::shared_ptr<LabelType> label_ptr = sample_labels[index_];
		size_t sample_index = sample_indices[index_];

		// Return data.
		return Sample<DataType, LabelType> (data_ptr, label_ptr, sample_index);
	}


	/*!
	 * Sets the index of the next returned sample.
	 * @param index_ Sample index.
	 */
	void setNextSampleIndex(size_t index_ = 0) {
		next_sample_index = index_;
	}

	/*!
	 * Checks if the returned sample was the last one.
	 * @return True if the sample was the last one.
	 */
	bool isLastSample() {
		return (next_sample_index >= this->sample_data.size());
	}


	/*!
	 * Adds sample to the batch.
	 * @param sample_ Sample to be added.
	 */
	virtual void add(mic::types::Sample<DataType, LabelType> sample_) {
		// Add sample to vectors.
		sample_data.push_back(sample_.data());
		sample_labels.push_back(sample_.label());
		sample_indices.push_back(sample_.index());
	}

	/*!
	 * Adds sample to the batch.
	 * @param data_ Sample data.
	 * @param label_ Sample label.
	 * @param number_ Sample number.
	 */
	virtual void add(std::shared_ptr<DataType> data_, std::shared_ptr<LabelType> label_, size_t index_) {
		// Add sample to vectors.
		sample_data.push_back(data_);
		sample_labels.push_back(label_);
		sample_indices.push_back(index_);
	}

	/*!
	 * Adds sample to the batch and generates the index of the added sample (sets it to the previous size).
	 * @param data_ Sample data.
	 * @param label_ Sample label.
	 */
	virtual void add(std::shared_ptr<DataType> data_, std::shared_ptr<LabelType> label_) {
		// Add sample to vectors.
		sample_data.push_back(data_);
		sample_labels.push_back(label_);
		sample_indices.push_back(sample_indices.size());
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

	/*!
	 * Returns the number of distinctive classes.
	 * @return Number of classes.
	 */
	size_t classes() {
		return number_of_classes;
	}

protected:
	/*!
	 * Index of the returned sample - it is used ONLY in getNextSample (i.e. iterative, not random sampling) method.
	 */
	size_t next_sample_index;

	/*!
	 * Batch size. As defaults set to one.
	 */
	size_t batch_size;

	/*!
	 * Random device used for generation of random numbers.
	 */
	std::random_device rd;

	/*!
	 *  Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits.
	 */
	std::mt19937_64 rng_mt19937_64;


	/// Stores the data.
	std::vector <std::shared_ptr<DataType> > sample_data;

	/// Stores labels.
	std::vector <std::shared_ptr<LabelType> > sample_labels;

	/// Stores sample indices (sample "positions" in original dataset).
	std::vector <size_t> sample_indices;

	/*!
	 * Number of distinctive classes in the (main) dataset.
	 */
	size_t number_of_classes;

};


} /* namespace types */
} /* namespace mic */




#endif /* SRC_TYPES_BATCH_HPP_ */
