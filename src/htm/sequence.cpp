#include <htm/sequence.h>

#include <logger/Log.hpp>

#include <data_utils/inline.h>
using namespace mic::data_utils;

namespace mic {
namespace htm {

int initialize_sequence_memory(layer* l) {

	l->segments = (segment*)malloc(l->sp_params.columns * SM_CELLS_PER_COLUMN * SM_SEGMENTS_PER_CELL * sizeof(segment));

	if (!l->segments) {

		fprintf(stderr, "SM initialize_sequence_memory: out of memory! (Requested %lu B)\n", l->sp_params.columns * SM_CELLS_PER_COLUMN * SM_SEGMENTS_PER_CELL * sizeof(segment));
		return 0;

	}

	l->sequence_memory_usage += l->sp_params.columns * SM_CELLS_PER_COLUMN * SM_SEGMENTS_PER_CELL * sizeof(segment);
	
	l->sequence_learning_enabled = true;
	l->sm_segment_activation_threshold = SM_SEGMENT_ACTIVATION_THRESHOLD;
	l->sm_synapse_permanence_decrement = SM_SYNAPSE_PERMANENCE_DECREMENT;
	l->sm_synapse_permanence_increment = SM_SYNAPSE_PERMANENCE_INCREMENT;
	l->sm_negative_reinforcement_factor = 1.0f;

	initialize_cells(l);
	//initialize_segments(l);
	initialize_learning_cache(l);

	return 1;
}

void initialize_cells(layer* l) {

	if (l->columns)
		for (unsigned int c = 0; c < l->sp_params.columns; c++)
			for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {
				RESET_CELL_STATE(l->columns[c].cells[i]);
				l->columns[c].cells[i].number_of_segments = 0;
				l->columns[c].cells[i].segment_change = false;
			}

}

void initialize_segments(layer* l) {

	for (unsigned long c = 0; c < l->sp_params.columns; c++) {
		for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {
			for (unsigned int seg = 0; seg < SM_SEGMENTS_PER_CELL; seg++) {

				segment* s = &(l->SEGMENTS(c, i, seg));
				reset_segment(s, i, l);

			}
		}
	}
		
}

int initialize_learning_cache(layer* l) {

	l->learning_cache = (synapse*)malloc(SM_SYNAPSE_LEARNING_CACHE_MAX_SIZE * sizeof(synapse));

	if (!l->learning_cache) {

		fprintf(stderr, "SM initialize_learning_cache: out of memory! (Requested %lu B)\n", SM_SYNAPSE_LEARNING_CACHE_MAX_SIZE * sizeof(synapse));
		return 0;

	}

	l->sequence_memory_usage += SM_SYNAPSE_LEARNING_CACHE_MAX_SIZE * sizeof(synapse);

	reset_learning_cache(l);

	return 1;

}

void reset_learning_cache(layer* l) {

	memset(l->learning_cache, 0, SM_SYNAPSE_LEARNING_CACHE_MAX_SIZE * sizeof(synapse));
	l->learning_cache_size = 0;

}

void cache_learning_synapse(layer* l, synapse s) {

	if (l->learning_cache_size < SM_SYNAPSE_LEARNING_CACHE_MAX_SIZE)
		l->learning_cache[l->learning_cache_size++] = s;

}

synapse retrieve_learning_synapse(layer* l, bool remove_from_cache) {

	unsigned selected = rand_int() % l->learning_cache_size;


	if (remove_from_cache) {
		l->learning_cache[selected] = l->learning_cache[l->learning_cache_size - 1];
		l->learning_cache_size--;
	}

	return l->learning_cache[selected];

}

void fill_learning_cache(layer* l) {

	synapse s;

	for (unsigned int c = 0; c < l->sp_params.columns; c++) {

		for (unsigned int n = 0; n < SM_CELLS_PER_COLUMN; n++) {

			if (CHECK_CELL_STATE(l->columns[c].cells[n], IS, LEARNING)) {

				reset_synapse(&s, l); 
				s.column_connection = c;
				s.cell_connection = n;

				cache_learning_synapse(l, s);

			}

		}

	}

}

void reset_segment(segment* seg, unsigned int i, layer* l) {

	seg->active_duty_cycle = 0.1f + (float)get_rand_range(0.0f, 0.9f);
	seg->sequence_segment = false;
	seg->active = false;
	seg->new_segment = true;
	seg->predicting = false;
	seg->mispredicted = false;
	seg->parent_cell_index = i;

	for (unsigned int s = 0; s < SM_SEGMENT_POTENTIAL_POOL; s++)
		reset_synapse(&seg->synapses[s], l);

}

void reset_synapse(synapse* s, layer* l) {

	s->permanence = (float) SYNAPSE_CONNECTED_PERMANENCE - 0.1f + (float)get_rand_range(0.0f, 0.2f);

	// proposed update due to learning
	s->update_queued = false;

	// random column connection
	s->column_connection = (unsigned int) rand_int() % l->sp_params.columns; //(unsigned int) rand_int_radius(l->sp_params.columns, 128, l->sp_params.columns);
	
	// random cell connection
	s->cell_connection = (unsigned int) rand_int() % SM_CELLS_PER_COLUMN;

	s->mispredicted = false;
	s->fresh = false;

}

void destroy_sequence_memory(layer* l) {

	free(l->segments);

	l->sequence_memory_usage -= l->sp_params.columns * SM_CELLS_PER_COLUMN * SM_SEGMENTS_PER_CELL * sizeof(segment*);

	free(l->learning_cache);

	l->sequence_memory_usage -= SM_SYNAPSE_LEARNING_CACHE_MAX_SIZE * sizeof(synapse*);

}

bool check_segment_state(layer* l, segment* seg, moment_t when, BYTE state, bool only_connected_synapses) {

	if (seg) {

		return (segment_activation_level(l, seg, when, state, only_connected_synapses) >= l->sm_segment_activation_threshold);

	}

	else return false;
}

segment* add_new_segment(layer* l, unsigned long c) {

	segment* added_segment = NULL;

	unsigned int fewest_segments = 0;
	unsigned int selected_cell = 0;

	unsigned int initial_cell_no = rand_int() % SM_CELLS_PER_COLUMN;

	for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {

		unsigned int k = (i + initial_cell_no) % SM_CELLS_PER_COLUMN;

		if ((l->columns[c].cells[k].number_of_segments < fewest_segments && 
			l->columns[c].cells[k].number_of_segments < SM_SEGMENTS_PER_CELL) || i == 0) {

			fewest_segments = l->columns[c].cells[k].number_of_segments;
		
			selected_cell = k;
		}
	}

	added_segment = add_new_segment_to_cell(l, c, selected_cell);

	return added_segment;
}

segment* add_new_segment_to_cell(layer* l, unsigned long c, unsigned int i) {

	segment* seg = NULL;

	CHECK(l->columns[c].cells[i].number_of_segments < SM_SEGMENTS_PER_CELL);

	if (l->columns[c].cells[i].number_of_segments < SM_SEGMENTS_PER_CELL) {

		seg = &(l->SEGMENTS(c, i, l->columns[c].cells[i].number_of_segments));
		reset_segment(seg, i, l);
		gettimeofday (&(seg->time_added), NULL);
		l->columns[c].cells[i].number_of_segments++;
	}

	return seg;

}

unsigned int segment_activation_level(layer* l, segment* seg, moment_t when, BYTE state, bool only_connected_synapses) {

	unsigned int activation = 0;

	for (unsigned syn = 0; syn < SM_SEGMENT_POTENTIAL_POOL; syn++) {

		if (!seg->synapses[syn].fresh && CHECK_CELL_STATE(l->columns[seg->synapses[syn].column_connection].cells[seg->synapses[syn].cell_connection], when, state)) {

			if ((seg->synapses[syn].permanence >= SYNAPSE_CONNECTED_PERMANENCE) || !only_connected_synapses)
				activation++;
		}

	}

	return activation;
}

segment* get_best_matching_segment(layer* l, unsigned long c, moment_t when) {

	unsigned int initial_cell_no = rand_int() % SM_CELLS_PER_COLUMN;

	segment* bms = NULL;

	unsigned int activation = 0;
	unsigned int highest_activation = 0;

	for (unsigned int n = 0; n < SM_CELLS_PER_COLUMN; n++) {

		unsigned int i = (n + initial_cell_no) % SM_CELLS_PER_COLUMN;

		if (l->columns[c].cells[i].number_of_segments > 0) {

			unsigned int initial_seg_no = rand_int() % l->columns[c].cells[i].number_of_segments;

			for (unsigned int seg = 0; seg < l->columns[c].cells[i].number_of_segments; seg++) {

				unsigned int k = (seg + initial_seg_no) % l->columns[c].cells[i].number_of_segments;

				segment* current_segment = &(l->SEGMENTS(c, i, k));

				CHECK (current_segment != NULL);

				if (current_segment) {

					activation = segment_activation_level(l, current_segment, when, ACTIVE, false);

					if (activation > highest_activation && activation >= l->sm_segment_activation_threshold) {

						highest_activation = activation;
						bms = current_segment;

					}

				}
			}
		}
	}

	return bms;
}

segment* get_active_segment(layer* l, unsigned long c, unsigned int i, moment_t when, BYTE state) {

	unsigned int initial_seg_no = rand_int() % l->columns[c].cells[i].number_of_segments;
	
	unsigned int activation = 0;
	unsigned int highest_activation = 0;
	unsigned int sequence_highest_activation = 0;

	segment* active_segment = NULL;
	segment* sequence_segment = NULL;

	for (unsigned int seg = 0; seg < l->columns[c].cells[i].number_of_segments; seg++) {

		unsigned int k = (seg + initial_seg_no) % l->columns[c].cells[i].number_of_segments;

		segment* current_segment = &(l->SEGMENTS(c, i, k));

		CHECK (current_segment != NULL);

		if (current_segment) {

			activation = segment_activation_level(l, current_segment, when, state, true);

			if (current_segment->sequence_segment) {

				if (activation >= l->sm_segment_activation_threshold && activation > sequence_highest_activation) {

					sequence_segment = current_segment;
					sequence_highest_activation = activation;
				}

			} 

			if (activation >= l->sm_segment_activation_threshold && activation > highest_activation) {

				active_segment = current_segment;
				highest_activation = activation;

			}

		}

	}

	if (sequence_segment)
		active_segment = sequence_segment;

	return active_segment;
}

void get_segment_active_synapses(layer* l, segment* seg, moment_t when, bool new_synapses) {

	if (seg) {

		unsigned int active_count = 0;
		unsigned int initial_syn_no = rand_int() % SM_SEGMENT_POTENTIAL_POOL;

		for (unsigned s = 0; s < SM_SEGMENT_POTENTIAL_POOL; s++) {

			unsigned int syn = (s + initial_syn_no) % SM_SEGMENT_POTENTIAL_POOL;

			if (CHECK_CELL_STATE(l->columns[seg->synapses[syn].column_connection].cells[seg->synapses[syn].cell_connection], when, ACTIVE)) {

				seg->synapses[syn].update_queued = true;
				active_count++;
			}
			
			else {

				if (new_synapses && active_count < SM_SEGMENT_NEW_SYNAPSE_COUNT && l->learning_cache_size > 0) {

					active_count++;

					unsigned int selected_synapse_from_cache = rand_int() % l->learning_cache_size;

					CHECK(l->learning_cache != NULL);
					seg->synapses[syn] = l->learning_cache[selected_synapse_from_cache];
					seg->synapses[syn].permanence = SYNAPSE_INITIAL_PERMANENCE;
					seg->synapses[syn].fresh = true;

				} else
					seg->synapses[syn].update_queued = false;

			}

		}

	}
}

void adapt_segments(layer* l, unsigned long c, unsigned int i, bool positive_reinforcement) {

	for (unsigned int seg = 0; seg < l->columns[c].cells[i].number_of_segments; seg++) {

		segment* current_segment = &(l->SEGMENTS(c, i, seg));

		CHECK (current_segment != NULL);

		if (current_segment) {

			// if (current_segment->sequence_update_queued && positive_reinforcement) 
			// 	current_segment->sequence_segment = true;

			for (unsigned syn = 0; syn < SM_SEGMENT_POTENTIAL_POOL; syn++) {

				if (positive_reinforcement) {
					if (current_segment->synapses[syn].update_queued) {

						current_segment->synapses[syn].permanence += l->sm_synapse_permanence_increment;

					} else {

						current_segment->synapses[syn].permanence -= l->sm_synapse_permanence_decrement;
					}

				} else {

					if (current_segment->synapses[syn].update_queued) {

						current_segment->synapses[syn].permanence -= l->sm_synapse_permanence_decrement;

					}

				}

				if (current_segment->synapses[syn].fresh) current_segment->synapses[syn].fresh = false;

 				current_segment->synapses[syn].permanence = __max(current_segment->synapses[syn].permanence, SM_SYNAPSE_PERMANENCE_RANGE_MIN);
 				current_segment->synapses[syn].permanence = __min(current_segment->synapses[syn].permanence, SM_SYNAPSE_PERMANENCE_RANGE_MAX);

			}
			

		}

	}

}

} /* namespace htm */
} /* namespace mic */
