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

//#include <types/MatrixTypes.hpp>

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
	Importer(std::string node_name_) : PropertyTree(node_name_),
		index("index",0),
		rng_mt19937_64(rd())
	{
		// Register properties - so their values can be overridden (read from the configuration file).
		registerProperty(index);

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
	 * Returns a pair consisting of <data, label>.
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
	 * Iterates through samples and returns them one by one. After returning the last element from the dataset the procedure starts from the beginning.
	 * @return
	 */
	std::pair<std::shared_ptr<dataType>, std::shared_ptr<labelType> > getNextSample() {

		// Check index.
		if(index >= data.size()-1){
			// Reset index.
			index = 0;
//			throw std::exception
		}
		// Return given sample and increment index afterwards.
		std::pair<std::shared_ptr<dataType>, std::shared_ptr<labelType> > sample = getSample(index);
		index += 1; 
		return getSample(sample);
	}

	/*!
	 * Returns sample with given index. If index is out of dataset range throws an "std::out_of_range" exception.
	 * @param index_ Sample index.
	 * @return Sample.
	 */
	std::pair<std::shared_ptr<dataType>, std::shared_ptr<labelType> > getSample(unsigned long index_) {

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
	 * Sets the index of the next returned sample.
	 * @param index_ Sample index.
	 */
	void setSampleIndex(size_t index_ = 0) {
		index = index_;
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
