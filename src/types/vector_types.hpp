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
