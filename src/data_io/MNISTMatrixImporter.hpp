/*!
 * \file MNISTMatrixImporter.hpp
 * \brief 
 * \author tkornut
 * \date Dec 23, 2015
 */


#ifndef SRC_DATA_IO_MNISTMATRIXIMPORTER_HPP_
#define SRC_DATA_IO_MNISTMATRIXIMPORTER_HPP_

#include <data_io/Importer.hpp>
#include <types/MatrixTypes.hpp>

namespace mic {
namespace data_io {

class MNISTMatrixImporter: public mic::data_io::Importer< mic::types::MatrixXf, unsigned int > {
public:
	/*!
	 * Constructor. Sets MNIST image default properties. Registers properties.
	 * @param data_filename_ File (with path) containing MNIST images.
	 * @param labels_filename_ File (with path) containing MNIST labels.
	 * @param node_name_ Name of the node in configuration file.
	 */
	MNISTMatrixImporter(std::string data_filename_ = "", std::string labels_filename_ = "", std::string node_name_ = "mnist_matrix_importer");

	/*!
	 * Virtual destructor. Empty.
	 */
	virtual ~MNISTMatrixImporter() { };

	/*!
	 * Set name and patch of the file containing MNIST images.
	 * @param data_filename_ File (with path) containing MNIST images.
	 */
	void setDataFilename(std::string data_filename_);

	/*!
	 * Set name and patch of the file containing MNIST labels.
	 * @param labels_filename_ File (with path) containing MNIST labels.
	 */
	void setLabelsFilename(std::string labels_filename_);

	/*!
	 * Method responsible for importing/loading the MNIST dataset.
	 * @return TRUE if data loaded successfully, FALSE otherwise.
	 */
	bool importData();

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


namespace mic {
namespace types {

/*!
 * \brief The <MatrixXf-uint> sample type used by e.g. MNISTImporter.
 * \author tkornuta
 */
typedef mic::types::Sample<mic::types::MatrixXf, unsigned int> MNISTSample;

/*!
 * \brief The <MatrixXf-uint> batch type used by e.g. MNISTImporter.
 * \author tkornuta
 */
typedef mic::types::Batch<mic::types::MatrixXf, unsigned int> MNISTBatch;

}//: namespace types
}//: namespace mic


#endif /* SRC_DATA_IO_MNISTMATRIXIMPORTER_HPP_ */
