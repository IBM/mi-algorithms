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
#ifndef __CELL_H__
#define __CELL_H__

// Dependency on mic::types.
#include <types/types.hpp>
using namespace mic::types;

#include <htm/shared/defs.h>
#include <htm/shared/bitwise.h>

namespace mic {
namespace htm {

#define ACTIVE									BIT_0
#define LEARNING								BIT_1
#define	PREDICTING								BIT_2

#define CHECK_CELL_STATE(s, WHEN, STATE) 		((s).state & (STATE << (WHEN << 2))) != 0
#define SET_CELL_STATE(s, WHEN, STATE) 			((s).state |= (STATE << (WHEN << 2)))
#define UNSET_CELL_STATE(s, WHEN, STATE) 		((s).state &= ~(STATE << (WHEN << 2)))
#define RESET_CELL_STATE(s)						((s).state &= 0x00)
#define RESET_PAST_CELL_STATE(s)				((s).state &= 0x0F)
#define RESET_CURRENT_CELL_STATE(s)				((s).state &= 0xF0)

// move cell state 'in time'
#define SHIFT_CELL_STATE(s)						((s).state = (state_t)((s).state << (WAS << 2)))


/*!
 * Class storing synapse parameters.
 * @author krocki/tkornuta
 */
class cell {
public:
	/// Cell state.
	state_t state;

	/// ?
	bool segment_change;

	/// Number of cell's segments.
	unsigned int number_of_segments;

};

} /* namespace htm */
} /* namespace mic */

#endif
