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
#ifndef __CLA_H__
#define __CLA_H__

#include <htm/layer.h>

namespace mic {
namespace htm {

void compute_active_state(layer* l);
void compute_predictive_state(layer* l);
void update_synapses(layer* l);

// move forward in time, update states t-1
void time_step(layer* l);


} /* namespace htm */
} /* namespace mic */

#endif
