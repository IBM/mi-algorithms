/*!
 * \file Batch.hpp
 * \brief 
 * \author tkornut
 * \date Apr 14, 2016
 */

#ifndef SRC_TYPES_BATCH_HPP_
#define SRC_TYPES_BATCH_HPP_

#include <types/Sample.hpp>

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
	Batch() { }

	/*!
	 * Copy constructor.
	 * @param batch_ Batch to be copied.
	 */
	Batch(const mic::types::Batch<DataType, LabelType>& batch_) {
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
	std::vector <std::shared_ptr<DataType> > data() {
		return sample_data;
	}

	/// Returns sample labels.
	std::vector <std::shared_ptr<LabelType> > labels() {
		return sample_labels;
	}

	/// Returns sample numbers (sample "positions" in original dataset).
	std::vector <size_t> indices() {
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
	 * Adds sample to the batch.
	 * @param sample_ Sample to be added.
	 */
	void add(mic::types::Sample<DataType, LabelType> sample_) {
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
	void add(std::shared_ptr<DataType> data_, std::shared_ptr<LabelType> label_, size_t index_) {
		// Add sample to vectors.
		sample_data.push_back(data_);
		sample_labels.push_back(label_);
		sample_indices.push_back(index_);
	}

	/*!
	 * Returns the number of distinctive classes.
	 * @return Number of classes.
	 */
	size_t classes() {
		return number_of_classes;
	}
protected:

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
