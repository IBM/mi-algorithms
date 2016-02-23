/*!
 * \file MNISTImageImporter.hpp
 * \brief 
 * \author tkornuta
 * \date Dec 21, 2015
 */

#ifndef SRC_DATA_IO_MNISTIMAGEIMPORTER_HPP_
#define SRC_DATA_IO_MNISTIMAGEIMPORTER_HPP_

#include <data_io/Importer.hpp>

// Dependency on core types.
#include <types/image_types.hpp>

namespace mic {
namespace data_io {

/*!
 * \brief Importer responsible for importing/loading MNIST images with labels.
 * \author tkornuta
 */
class MNISTImageImporter : public mic::data_io::Importer<mic::types::image, unsigned int> {
public:
	/*!
	 * Constructor. Sets MNIST image default properties. Registers properties.
	 * @param node_name_ Name of the node in configuration file.
	 */
	MNISTImageImporter(std::string node_name_ = "mnist_image_importer");

	/*!
	 * Virtual destructor. Empty.
	 */
	virtual ~MNISTImageImporter() { };

	/*!
	 * Method responsible for importing/loading the MNIST dataset.
	 * @return TRUE if data loaded successfully, FALSE otherwise.
	 */
	bool importData();

	/*!
	 * Sets data filename (with path). DEPRICATED (should be used only for testing purposes).
	 * @param data_filename_ Path and filename
	 */
	void setDataFilename(std::string data_filename_) {
		data_filename = data_filename_;
	}

	/*!
	 * Sets labels filename (with path). DEPRICATED (should be used only for testing purposes).
	 * @param labels_filename_ Path and filename
	 */
	void setLabelsFilename(std::string labels_filename_) {
		labels_filename = labels_filename_;
	}

	/*!
	 * Method responsible for initialization of all variables that are property-dependent - here not required, yet empty.
	 */
	virtual void initializePropertyDependentVariables() { };

private:
	/*!
	 * Width of MNIST image.
	 */
	int image_width;

	/*!
	 * Height of MNIST image.
	 */
	int image_height;

	/*!
	 * Property: directory/Name of file containing images (binary datafile).
	 */
	mic::configuration::Property<std::string> data_filename;

	/*!
	 * Property: directory/Name of file containing labels.
	 */
	mic::configuration::Property<std::string> labels_filename;

};

} /* namespace data_io */
} /* namespace mic */

/*!
 * \brief The type returned by MNIST getRandomSample method.
 * \author tkornuta
 */
typedef std::pair< std::shared_ptr<mic::types::image>, std::shared_ptr<unsigned int> > mnist_pair_t;

#endif /* SRC_DATA_IO_MNISTIMAGEIMPORTER_HPP_ */