/*!
 * \file MNISTMatrixImporter.hpp
 * \brief 
 * \author tkornut
 * \date Dec 23, 2015
 */


#ifndef SRC_DATA_IO_MNISTMATRIXIMPORTER_HPP_
#define SRC_DATA_IO_MNISTMATRIXIMPORTER_HPP_

#include <data_io/Importer.hpp>

namespace mic {
namespace data_io {

class MNISTMatrixImporter: public mic::data_io::Importer< mic::types::matrixd_t, unsigned int> {
public:
	/*!
	 * Constructor. Sets MNIST image default properties. Registers properties.
	 * @param node_name_ Name of the node in configuration file.
	 */
	MNISTMatrixImporter(std::string node_name_ = "mnist_matrix_importer");

	/*!
	 * Virtual destructor. Empty.
	 */
	virtual ~MNISTMatrixImporter() { };

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

#endif /* SRC_DATA_IO_MNISTMATRIXIMPORTER_HPP_ */
