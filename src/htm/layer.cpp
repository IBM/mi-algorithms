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
#include <htm/spatial.h>
#include <htm/sequence.h>
#include <htm/reconstruct.h>
#include <htm/cla.h>
#include <htm/layer.h>

#include <logger/Log.hpp>

//#include <system_utils/system_info.hpp>
//using namespace mic::system_utils;

#include <data_utils/input.h>
using namespace mic::data_utils;


namespace mic {
namespace htm {

void set_default_layer_parameters(layer* l) {

	l->sp_params.proximal_synapses_per_column = DEFAULT_SP_PROXIMAL_SYNAPSES_PER_COLUMN;
	l->sp_params.columns = DEFAULT_SP_COLUMNS;
	l->sp_params.initial_column_boost = DEFAULT_SP_INITIAL_COLUMN_BOOST;
	l->sp_params.initial_active_duty_cycle = DEFAULT_SP_INITIAL_ACTIVE_DUTY_CYCLE;
	l->sp_params.initial_min_duty_cycle =  DEFAULT_SP_INITIAL_MIN_DUTY_CYCLE;
	l->sp_params.initial_overlap_duty_cycle = DEFAULT_SP_INITIAL_OVERLAP_DUTY_CYCLE;
	l->sp_params.min_overlap = DEFAULT_SP_MIN_OVERLAP;
	l->sp_params.local_inhibition_radius = DEFAULT_SP_LOCAL_INHIBITION_RADIUS;
	l->sp_params.max_active_columns = DEFAULT_SP_MAX_COLUMNS;
	l->sp_params.max_duty_cycle_radius = DEFAULT_SP_MAX_DUTY_CYCLE_RADIUS;
	l->sp_params.duty_cycle_history_weight = DEFAULT_SP_DUTY_CYCLE_HISTORY_WEIGHT;
	l->sp_params.boost_step = DEFAULT_SP_BOOST_STEP;
	l->sp_params.input_vector_length = DEFAULT_SP_INPUT_VECTOR_LENGTH;
	l->sp_params.permanence_increment = DEFAULT_SP_SYNAPSE_PERMANENCE_INCREMENT;
	l->sp_params.permanence_decrement = -DEFAULT_SP_SYNAPSE_PERMANENCE_DECREMENT;

}

void set_layer_parameters(layer *l, unsigned columns, unsigned inputs, unsigned synapses, unsigned iradius, bool spatial_learning_on, bool temporal_learning_on) {

	l->sp_params.columns = columns;
	l->sp_params.proximal_synapses_per_column = synapses;
	l->sp_params.input_vector_length = inputs;
	l->sp_params.local_inhibition_radius = iradius;
	l->spatial_learning_enabled = spatial_learning_on;
    l->temporal_learning_enabled = temporal_learning_on;

    //tracelog("Non-default layer parameters: C %u, P %u, I %lu, H %u, S %d, T %d\n", l->sp_params.columns, l->sp_params.proximal_synapses_per_column,
    //	l->sp_params.input_vector_length, l->sp_params.local_inhibition_radius, l->spatial_learning_enabled, l->temporal_learning_enabled);

}
void initialize_layer(layer *l) {

/*	tracelog("Initializing Bit Vectors...\n");
	initialize_bit_vectors(l);
	tracelog("Done.\n");*/ // NOT REQUIRED ANYMORE - SDR TK!

	LOG(LDEBUG) << "Initializing Spatial Pooler...";
	initialize_spatial_pooler(l);
	//tracelog("Done. Memory Allocated: %.2f MB\n", (float)l->spatial_memory_usage/(float)(1 << 20));
    
	LOG(LDEBUG) << "Initializing Sequence Memory...";
    initialize_sequence_memory(l);
    //tracelog("Done. Memory Allocated: %.2f MB\n", (float)l->sequence_memory_usage/(float)(1 << 20));

    LOG(LDEBUG) << "Initializing Reconstruction Memory...";
	initialize_reconstruction_memory(l);
	//tracelog("Done. Memory Allocated: %.2f MB\n", (float)l->reconstruction_memory_usage/(float)(1 << 20));

	l->input_overlap = (float*) malloc(l->sp_params.input_vector_length * sizeof(float));
	l->max_sum_of_permanences = 0.0f;
    // learning on at start?
    // l->spatial_learning_enabled = false;
    // l->temporal_learning_enabled = false;


    l->init_completed = true;

}

void destroy_layer(layer* l) {

	destroy_sequence_memory(l);
	LOG(LDEBUG) << "SM Memory deallocated.";

	destroy_spatial_pooler(l);
	LOG(LDEBUG) << "SP Memory deallocated.";

	destroy_reconstruction_memory(l);
	LOG(LDEBUG) << "Reconstruction Memory Deallocated.";

/*	FREE(l->input_float_sdr);
	FREE(l->original_input_float_sdr);*/

	LOG(LDEBUG) << "Bit Vector Memory Deallocated.";

	free(l->input_overlap);

}

/*void initialize_bit_vectors(layer *l) {

	l->input_float_sdr = (float*)ALLOC(l->sp_params.input_vector_length * sizeof(float));
	l->original_input_float_sdr = (float*)ALLOC(l->sp_params.input_vector_length * sizeof(float));

}*/

/*void get_active_columns(layer* l, bool* out, unsigned range_start, unsigned range_end) {

	CHECK(out != NULL);

	if (out) {

			for (unsigned int c = range_start; c < range_end; c++) {

				if (c < l->sp_params.columns) {



				}

			}


	}
}*/

mic::htm::PerformanceStatistics collectPerformanceStatistics(layer* l) {

	mic::htm::PerformanceStatistics data;

	data.min_segment_age = -1.0;
	data.max_segment_age = -1.0;

    //gettimeofday (&(data.collection_time), NULL);
    //data.time_since_program_start = get_time_diff (&start_time, &data.collection_time);

	for (unsigned long c = 0; c < l->sp_params.columns; c++) {

		if (l->columns[c].active[IS]) {

			data.active_columns++;

			if (l->columns[c].active[WAS])
				data.shared_sdr_columns++;

			if (l->columns[c].predicting[WAS])
				data.predicted_columns++;
			else
				data.unpredicted_columns++;

		} else {

			if (l->columns[c].predicting[WAS])
				data.mispredicted_columns++;
		}

		if (l->columns[c].predicting[IS])
			data.predicting_columns++;

		for (unsigned int p = 0; p < l->sp_params.proximal_synapses_per_column; p++) {

			if (IS_BIT_SET_01(l->input_float_sdr, l->columns[c].synapses[p].column_connection) &&
				(l->columns[c].synapses[p].permanence >= SYNAPSE_CONNECTED_PERMANENCE))
					data.active_connected_proximal_synapses++;

			if (IS_BIT_SET_01(l->input_float_sdr, l->columns[c].synapses[p].column_connection) &&
				(l->columns[c].synapses[p].permanence < SYNAPSE_CONNECTED_PERMANENCE))
					data.active_disconnected_proximal_synapses++;

			if (l->columns[c].synapses[p].permanence >= SYNAPSE_CONNECTED_PERMANENCE)
				data.total_connected_proximal_synapses++;

			data.sum_of_permanences_proximal_synapses += l->columns[c].synapses[p].permanence;

		}

		for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {

			if (CHECK_CELL_STATE(l->columns[c].cells[i], IS, ACTIVE)) {

				data.active_cells++;

				if (CHECK_CELL_STATE(l->columns[c].cells[i], WAS, ACTIVE))	
					data.shared_context_cells++;

				if (CHECK_CELL_STATE(l->columns[c].cells[i], WAS, PREDICTING))
					data.predicted_cells++;
				else
					data.unpredicted_cells++;

			} else {

				if (CHECK_CELL_STATE(l->columns[c].cells[i], WAS, PREDICTING))
					data.mispredicted_cells++;
			}

			if (CHECK_CELL_STATE(l->columns[c].cells[i], IS, PREDICTING))
				data.predicting_cells++;

			if (CHECK_CELL_STATE(l->columns[c].cells[i], IS, LEARNING))
				data.learning_cells++;

			if (l->columns[c].cells[i].segment_change)
				data.segment_change_cells++;

			for (unsigned int seg = 0; seg < l->columns[c].cells[i].number_of_segments; seg++) {

				data.total_segments++;

				segment* sp = &(l->SEGMENTS(c, i, seg));

				if (sp) {
					// Get age in seconds.
					double age = (sp->time_added - data.collection_time).total_seconds();

					if (age < data.min_segment_age || data.min_segment_age < 0)
						data.min_segment_age = age;

					if (age > data.max_segment_age || data.max_segment_age < 0)
						data.max_segment_age = age;

						data.avg_segment_age += age;

						data.avg_active_duty_cycle += sp->active_duty_cycle;

					if (check_segment_state(l, sp, IS, ACTIVE, true)) {
						data.connected_active_segments++;
						data.active_segments++;
					}

					if (check_segment_state(l, sp, IS, ACTIVE, false)) {
						data.disconnected_active_segments++;
						data.active_segments++;
					}

					if (sp->sequence_segment)
						data.sequence_segments++;

					if (sp->new_segment)
						data.new_segments++;

					for (unsigned syn = 0; syn < SM_SEGMENT_POTENTIAL_POOL; syn++) { 

						if (sp->synapses[syn].permanence < SYNAPSE_MINIMUM_PERMANENCE)
							data.invalid_distal_synapses++;
						else
							data.valid_distal_synapses++;

						if (sp->synapses[syn].permanence >= SYNAPSE_CONNECTED_PERMANENCE) {
							data.total_connected_distal_synapses++;

							if (CHECK_CELL_STATE(l->columns[sp->synapses[syn].column_connection].cells[sp->synapses[syn].cell_connection], IS, ACTIVE)) {
								data.active_connected_distal_synapses++;
								data.active_distal_synapses++;
							}
						
						} else {

							if (CHECK_CELL_STATE(l->columns[sp->synapses[syn].column_connection].cells[sp->synapses[syn].cell_connection], IS, ACTIVE)) {
								data.active_disconnected_distal_synapses++;
								data.active_distal_synapses++;
							}

						}

						if (sp->synapses[syn].mispredicted)
							data.mispredicting_distal_synapses++;

						if (sp->synapses[syn].update_queued)
							data.altered_distal_synapses++;

						data.cached_distal_synapses = l->learning_cache_size;

					}

				} // if (sp)
			}
		}
	}

	data.avg_segment_age /= (long double)data.total_segments;
	data.avg_active_duty_cycle /= (long double)data.total_segments;
	data.spatial_memory_preallocated = l->spatial_memory_usage;
	data.segment_memory_preallocated = l->sequence_memory_usage;
	data.reconstruction_memory_preallocated = l->reconstruction_memory_usage;
	data.segment_memory_usage = data.total_segments * sizeof(segment);

	return data;

}

} /* namespace htm */
} /* namespace mic */
