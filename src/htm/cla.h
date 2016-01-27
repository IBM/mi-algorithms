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
