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
