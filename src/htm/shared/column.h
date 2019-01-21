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
