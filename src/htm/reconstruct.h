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
#ifndef __RECONSTRUCT_H__
#define __RECONSTRUCT_H__

#include <htm/layer.h>
#include <stdlib.h>

namespace mic {
namespace htm {


void reset_reconstruction_space(reconstruction* r, size_t vector_length);
/*void allocate_reconstruction_space(reconstruction* r, size_t vector_length);
void deallocate_reconstruction_space(reconstruction* r);*/

void initialize_reconstruction_memory(layer *l);
void destroy_reconstruction_memory(layer *l);
void compute_static_reconstruction(layer* l);
void compute_dynamic_reconstruction(layer* l);
float compute_dynamic_reconstruction_score(layer *l);
float get_permanence(layer* l, unsigned i, unsigned j);


} /* namespace htm */
} /* namespace mic */

#endif
