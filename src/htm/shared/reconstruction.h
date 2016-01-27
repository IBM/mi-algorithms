#ifndef __RECONSTRUCTION_H__
#define __RECONSTRUCTION_H__

#include <types/SDR.hpp>

using namespace mic::types;

namespace mic {
namespace htm {

/*!
 * \brief Structure storing the reconstruction vectors.
 * \author krocki
 */
typedef struct {

	/*!
	 * SDR representing the reconstruction vector.
	 */
	floatSDR reconstruction_vector;

	/*!
	 * Reconstruction vector BINARY??
	 */
	floatSDR reconstruction_vector_binary;

} reconstruction;

} /* namespace htm */
} /* namespace mic */

#endif
