#ifndef __COLUMN_H__
#define __COLUMN_H__

#include <htm/shared/cell.h>
#include <htm/shared/defs.h>
#include <htm/shared/synapse.h>

namespace mic {
namespace htm {

// TODO: move cells/synapses to separate arrays or convers columns' array into SaA
typedef struct {

	// cells in a column
	cell cells[SM_CELLS_PER_COLUMN];

	// potential synapses, proximal dendrites
	synapse* synapses;

	float* full_input_vector;
	
	bool active[2];
	bool predicting[2];

	bool unpredicted;
	bool predicted;
	bool mispredicted;
	
	// boost value
	float boost;
	float overlap;

	float active_duty_cycle;
	float min_duty_cycle;
	float overlap_duty_cycle;

	float length;

	float sum_of_permanences;
	
} column;

} /* namespace htm */
} /* namespace mic */

#endif
