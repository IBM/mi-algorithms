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
	Importer(std::string node_name_, size_t batch_size_ = 1) : mic::types::Batch<DataType, LabelType>::Batch(batch_size_), PropertyTree(node_name_)
	{
		// Register properties - so their values can be overridden (read from the configuration file).
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

};

} /* namespace data_io */
} /* namespace mic */

#endif /* SRC_DATA_IO_IMPORTER_HPP_ */
