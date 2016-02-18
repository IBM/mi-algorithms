/*!
 * \file IBMFontMatrixImporter.hpp
 * \brief Contains declaration of an importer responsible for importing/loading characters from IBM VGA font dataset.
 * \author tkornut
 * \date Dec 22, 2015
 */

#ifndef SRC_DATA_IO_IBMFONTMATRIXIMPORTER_HPP_
#define SRC_DATA_IO_IBMFONTMATRIXIMPORTER_HPP_

#include <data_io/Importer.hpp>

namespace mic {
namespace data_io {

/*!
 * \brief IBM font type.
 * \author tkornuta
 */
enum IBMfont_t
{
	font8x8_type = 8, ///< Loads characters of size 8x8.
	font16x16_type  = 16 ///< Loads characters of size 16x16.
};

} /* namespace data_io */
} /* namespace mic */


/*!
 * \brief Class template specialization used for lexical casting between string and IBMfont_t type.
 * \author tkornuta
 */
template<>
class mic::configuration::LexicalTranslator<mic::data_io::IBMfont_t> {
public:
	static std::string toStr(const mic::data_io::IBMfont_t & val) {
		try {
			return boost::lexical_cast<std::string>((unsigned int)val);
		} catch(...) {
			std::stringstream ss;
			ss << val;
			return ss.str();
		}
		return "";
	}

	static mic::data_io::IBMfont_t fromStr(const std::string & str) {
		try {
			return (mic::data_io::IBMfont_t) boost::lexical_cast<unsigned int>(str);
		} catch (...) {
			std::stringstream ss(str);
			unsigned int ret;
			ss >> ret;
			return (mic::data_io::IBMfont_t) ret;
		}
		// If failed, return 8x8.
		return mic::data_io::font8x8_type;
	}
};


namespace mic {
namespace data_io {


/*!
 * \brief Importer responsible for importing/loading characters from IBM VGA font dataset.
 * \author tkornuta
 */
class IBMFontMatrixImporter: public mic::data_io::Importer< mic::types::MatrixXf, char> {
public:
	/*!
	 * Constructor. Sets MNIST image default properties. Registers properties.
	 * @param node_name_ Name of the node in configuration file.
	 */
	IBMFontMatrixImporter(std::string node_name_ = "ibm_font_matrix_importer");

	/*!
	 * Virtual destructor. Empty.
	 */
	virtual ~IBMFontMatrixImporter() { };

	/*!
	 * Method responsible for importing/loading the IBM VGA fonts.
	 * @return TRUE if data loaded successfully, FALSE otherwise.
	 */
	bool importData();

	/*!
	 * Method responsible for initialization of all variables that are property-dependent - here not required, yet empty.
	 */
	virtual void initializePropertyDependentVariables() { };

private:
	/*!
	 * Selected font type.
	 */
	mic::configuration::Property<IBMfont_t> font_type;
};

} /* namespace data_io */
} /* namespace mic */

#endif /* SRC_DATA_IO_IBMFONTMATRIXIMPORTER_HPP_ */
