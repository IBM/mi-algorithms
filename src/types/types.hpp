/*!
 * \file types.hpp
 * \brief Declaration of state and moment types.
 * \author: krocki
 * \date Somewhere between 2013 and 2015
 */

#ifndef __TYPES_H__
#define __TYPES_H__

namespace mic {
namespace types {

#ifndef __OPENCL_COMPILER__
#include <stdint.h>

#define BYTE uint8_t
#else
#define BYTE unsigned char
#endif

/*!
 * \brief State of a cell.
 */
typedef BYTE state_t;

/*!
 * \brief Moment in time.
 */
typedef enum {IS, WAS} moment_t;


}//: namespace types
}//: namespace mic

#endif
