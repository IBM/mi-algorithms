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
