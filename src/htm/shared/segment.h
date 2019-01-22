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
#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include <htm/shared/synapse.h>
#include <htm/shared/defs.h>

//#include <sys/time.h>
 #include <boost/date_time/posix_time/posix_time.hpp>

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

	boost::posix_time::ptime time_added;

} segment;

} /* namespace htm */
} /* namespace mic */

#endif
