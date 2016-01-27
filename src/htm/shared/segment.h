#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include <htm/shared/synapse.h>
#include <htm/shared/defs.h>

#include <sys/time.h>

namespace mic {
namespace htm {

typedef struct {

	// synapses per segment
	synapse synapses[SM_SEGMENT_POTENTIAL_POOL];

	float active_duty_cycle;
	
	bool sequence_segment;

	bool sequence_update_queued;

	bool new_segment;

	bool active;
	bool learning;
	bool predicting;
	bool mispredicted;

	unsigned int parent_cell_index;

	struct timeval time_added;

} segment;

} /* namespace htm */
} /* namespace mic */

#endif
