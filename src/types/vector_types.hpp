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
/**
 * \file vector_types.hpp
 * \brief File containing vector basic types.
 * \author tkornuta/krocki
 * \date Nov/09/2015
 */

#ifndef __VECTOR_TYPES_HPP__
#define __VECTOR_TYPES_HPP__

namespace mic {
namespace types {


/**
 *  \brief Basic structure representing vector of three floats (xyz coordinates).
 *  \author krocki
 */
typedef struct {
	/// X coordinate.
	float x;
	/// Y coordinate.
	float y;
	/// Z coordinate.
	float z;
} v_3f;


/**
 *  \brief Basic structure representing vector of four integers (xyzw coordinates).
 *  \author krocki
 */
typedef struct {
	/// X coordinate.
	uint8_t x;
	/// Y coordinate.
	uint8_t y;
	/// Z coordinate.
	uint8_t z;
	/// W coordinate.
	uint8_t w;
} v_4i;


}//: namespace types
}//: namespace mic


#endif
