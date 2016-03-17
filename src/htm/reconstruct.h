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
