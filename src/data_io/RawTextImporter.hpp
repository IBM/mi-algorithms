/*!
 * \file RawTextImporter.hpp
 * \brief 
 * \author tkornut
 * \date Dec 22, 2015
 */

#ifndef SRC_DATA_IO_RAWTEXTIMPORTER_HPP_
#define SRC_DATA_IO_RAWTEXTIMPORTER_HPP_

#include <data_io/Importer.hpp>

namespace mic {
namespace data_io {

/*!
 * \brief Importer responsible for importing/loading raw text files and returning characters one by one, the character denotes the label on its own.
 * \author tkornuta
 */
class RawTextImporter: public mic::data_io::Importer<char, char> {
public:
	/*!
	 * Constructor. Sets MNIST image default properties. Registers properties.
	 * @param node_name_ Name of the node in configuration file.
	 */
	RawTextImporter(std::string node_name_ = "raw_text_importer");

	/*!
	 * Virtual destructor. Empty.
	 */
	virtual ~RawTextImporter() { };

	/*!
	 * Method responsible for importing/loading the raw text files.
	 * @return TRUE if data loaded successfully, FALSE otherwise.
	 */
	bool importData();

	/*!
	 * Sets data filename (with path). DEPRICATED (should be used only for testing purposes).
	 * @param data_filename_ Path and filename
	 */
	void setDataFilename(std::string data_filename_);

	/*!
	 * Method responsible for initialization of all variables that are property-dependent - here not required, yet empty.
	 */
	virtual void initializePropertyDependentVariables() { };

private:
	/*!
	 * Property: directory/Name of file containing data - raw text.
	 */
	mic::configuration::Property<std::string> data_filename;
};


} /* namespace data_io */
} /* namespace mic */

namespace mic {
namespace types {

/*!
 * \brief The <char-char> pair type used by e.g. RawTextImporter.
 * \author tkornuta
 */
typedef mic::types::Sample<char, char> CharSample;

/*!
 * \brief The <char-char> pair type used by e.g. RawTextImporter.
 * \author tkornuta
 */
typedef mic::types::Batch<char, char> CharBatch;

}//: namespace types
}//: namespace mic

#endif /* SRC_DATA_IO_RAWTEXTIMPORTER_HPP_ */
