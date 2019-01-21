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
#ifndef __LAYER_H__
#define __LAYER_H__

#include <htm/shared/column.h>
#include <htm/shared/segment.h>
#include <htm/shared/reconstruction.h>

// Dependency on mic::types;
//#include <types/types.h>
#include <types/SDR.hpp>
using namespace mic::types;

#include <htm/PerformanceStatistics.hpp>

namespace mic {

/**
 * \namespace mic::htm
 * \brief Namespace containing HTM (Hierarchical Temporal Memory) related structures, classes and functions.
 * \author tkornuta
 */
namespace htm {

// 3D indices of the 1D segments' array
// (COLUMN, CELL, SEGMENT)
#define SEGMENTS(X, Y, Z) segments[(X) * SM_CELLS_PER_COLUMN * SM_SEGMENTS_PER_CELL + (Y) * SM_SEGMENTS_PER_CELL + (Z)]
//return the column no if segment z
//#define WHICH_COLUMN(Z) ((Z)/(SM_CELLS_PER_COLUMN * SM_SEGMENTS_PER_CELL))
//#define WHICH_CELL(Z) (((Z)/(SM_SEGMENTS_PER_CELL)) % SM_CELLS_PER_COLUMN)

typedef struct {

	unsigned 		proximal_synapses_per_column;

	/// Number of columns in layer.
	unsigned 		columns;
	float 			initial_column_boost;
	float 			initial_active_duty_cycle;
	float 			initial_min_duty_cycle;
	float 			initial_overlap_duty_cycle;
	unsigned 		min_overlap;
	unsigned 		local_inhibition_radius;
	unsigned 		max_active_columns;
	unsigned 		max_duty_cycle_radius;
	float 			duty_cycle_history_weight;
	float 			boost_step;
	float 			permanence_increment;
	float 			permanence_decrement;

	unsigned long 	input_vector_length;

} spatial_pooler_params;

typedef struct {

	// local, small mem, used by spatial pooler, sequence memory and temporal pooler
	column* columns;
	// global, large mem, not used by spatial pooler
	segment* segments;
	//memory space for storing addresses of synapses available for learning
	synapse* learning_cache;
	//number of valid synapses currently stored in cache
	unsigned int learning_cache_size;

	// SM parameters
	unsigned int sm_segment_activation_threshold;
	float sm_synapse_permanence_decrement;
	float sm_synapse_permanence_increment;
	float sm_negative_reinforcement_factor;

	/// SDR constituting the input vector to spatial pooler.
	mic::types::floatSDR input_float_sdr;


	reconstruction* static_reconstruction;
	reconstruction dynamic_reconstruction;
	reconstruction prediction_reconstruction;

	unsigned long long spatial_memory_usage;
	unsigned long long sequence_memory_usage;
	unsigned long long reconstruction_memory_usage;

	// enable/disable learning
	bool spatial_learning_enabled;
	bool sequence_learning_enabled;
	bool temporal_learning_enabled;

	// Spatial Pooler parameters
	spatial_pooler_params sp_params;
	
	bool init_completed;

	float* input_overlap;

	float max_sum_of_permanences;

	int number_of_columns_active;

	float worst_column_overlap;
	float best_column_overlap;
	
} layer;


PerformanceStatistics collectPerformanceStatistics(layer* l);

void set_default_layer_parameters(layer* l);
void set_layer_parameters(layer *l, unsigned columns, unsigned inputs, unsigned synapses, unsigned iradius, bool spatial_learning_on, bool temporal_learning_on);
//void initialize_bit_vectors(layer* l); - NOT REQUIRED ANYMORE!!TK
void initialize_layer(layer* l);
void destroy_layer(layer* l);

//void get_active_columns(layer* l, bool* out, unsigned range_start, unsigned range_end);


} /* namespace htm */
} /* namespace mic */

#endif
