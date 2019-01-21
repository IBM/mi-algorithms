/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
