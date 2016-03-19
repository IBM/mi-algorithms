#include <htm/spatial.h>
#include <htm/reconstruct.h>

#include <logger/Log.hpp>

#include <data_utils/rand.hpp>
#include <data_utils/input.h>
#include <data_utils/inline.h>

#ifdef _WIN32
#include <float.h>
#include <math.h>
#endif

using namespace mic::data_utils;

namespace mic {
namespace htm {

int initialize_spatial_pooler(layer* l) {

	// check values of the parameters

	l->columns = (column*)malloc(l->sp_params.columns * sizeof(column));

	CHECK(l->columns != NULL);

	if (!l->columns) {

		fprintf(stderr, "SP: out of memory! (Requested %lu B)\n", l->sp_params.columns * sizeof(column));
		return 0;
	}

	else {
		
		l->spatial_memory_usage += l->sp_params.columns * sizeof(column);

		initialize_columns(l);
		initialize_potential_pool(l);

		return 1;

	}

}

void destroy_spatial_pooler(layer* l) {

	for (unsigned int c = 0; c < l->sp_params.columns; c++) {

		free(l->columns[c].synapses);
		l->spatial_memory_usage -= l->sp_params.proximal_synapses_per_column * sizeof(synapse);

		free(l->columns[c].full_input_vector);
		l->spatial_memory_usage -= l->sp_params.input_vector_length* sizeof(float);

	}

	free(l->columns);
	l->spatial_memory_usage -= l->sp_params.columns * sizeof(column);

}

void initialize_columns(layer* l) {

	// iterate over columns
	for (unsigned int c = 0; c < l->sp_params.columns; c++) {

		l->columns[c].synapses = (synapse*)malloc(l->sp_params.proximal_synapses_per_column * sizeof(synapse));

		CHECK(l->columns[c].synapses != NULL);

		if (!(l->columns[c].synapses)) {

			fprintf(stderr, "l->columns[c].synapses == NULL, SP: out of memory! (Requested %lu B)\n", l->sp_params.proximal_synapses_per_column * sizeof(synapse));

		}

		l->spatial_memory_usage += l->sp_params.proximal_synapses_per_column * sizeof(synapse);

		l->columns[c].full_input_vector = (float*)malloc(l->sp_params.input_vector_length* sizeof(float));

		CHECK(l->columns[c].full_input_vector != NULL);

		if (!(l->columns[c].full_input_vector)) {

			fprintf(stderr, "l->columns[c].full_input_vector == NULL, SP: out of memory! (Requested %lu B)\n", l->sp_params.input_vector_length * sizeof(float));

		}

		l->spatial_memory_usage += l->sp_params.input_vector_length * sizeof(float);

		// initial values
		l->columns[c].boost = l->sp_params.initial_column_boost;
		l->columns[c].overlap = 0.0f;
		l->columns[c].active_duty_cycle = l->sp_params.initial_active_duty_cycle + (float)get_rand_range(0.00f, 0.05f);
		l->columns[c].min_duty_cycle = l->sp_params.initial_min_duty_cycle;
		l->columns[c].overlap_duty_cycle = l->sp_params.initial_overlap_duty_cycle;
		l->columns[c].active[IS] = false;
		l->columns[c].active[WAS] = false;
		l->columns[c].unpredicted = false;
		l->columns[c].predicted = false;
		l->columns[c].mispredicted = false;

	}

}

void initialize_potential_pool(layer* l) {

	// iterate over columns
	for (unsigned int c = 0; c < l->sp_params.columns; c++) {

		// unsigned image_w = 28;
		// unsigned side = (unsigned)(int)(float)sqrtf(l->sp_params.columns);
		// float pos_x = (float)(c % side)/(float)side;
		// float pos_y = (float)(c / side)/(float)side;
		// unsigned long size = 7;//(unsigned)(int)(float)round(5.0f + 5.0f * sqrtf((0.5f - pos_x) * (0.5f - pos_x) + (0.5f - pos_y) * (0.5f - pos_y)));
		// unsigned long center = (unsigned)(round(pos_x * (float)(image_w - size))) + size + image_w * ((unsigned)(round(pos_y * (float)(image_w - size))) + size);//////((l->sp_params.input_vector_length);

		for (unsigned int s = 0; s < l->sp_params.proximal_synapses_per_column; s++) {

			// permanence is set to a random [0,1] real number
			l->columns[c].synapses[s].permanence = SYNAPSE_CONNECTED_PERMANENCE - 0.05f + (float)get_rand_range(0.0f, 0.1f);
			// random column connection
			//l->columns[c].synapses[s].column_connection = s;//(unsigned int) (rand_int() % (l->sp_params.input_vector_length));
			l->columns[c].synapses[s].column_connection = s;//(unsigned int) (rand_int() % (l->sp_params.input_vector_length));
			//l->columns[c].synapses[s].column_connection = (unsigned int) rand_int_radius_2d (center, size, image_w, l->sp_params.input_vector_length);
			// l->columns[c].synapses[s].column_connection = (unsigned int) rand_int_radius_2d_gaussian (center, size, image_w, l->sp_params.input_vector_length);
			l->columns[c].synapses[s].cell_connection = 0;
			l->columns[c].length = 1.0f;

			
		}
	}		
}


float get_max_sum_of_permanences(layer* l) {

	float max = 0.0f;

	for (unsigned int c = 0; c < l->sp_params.columns; c++) {

		if (max < l->columns[c].sum_of_permanences)
			max = l->columns[c].sum_of_permanences;

	}

	return max;
}

float calculate_column_overlap(layer* l, unsigned int c) {

	float overlap = 0.0f;
	
	// WHITEPAPER overlap
	for (unsigned int s = 0; s < l->sp_params.proximal_synapses_per_column; s++) {
			// is input active?
			overlap += (l->columns[c].synapses[s].permanence - 0.5f) * (IS_BIT_SET_01(l->input_float_sdr, l->columns[c].synapses[s].column_connection) - 0.5f);
	}//: for

	return overlap;

}

void calculate_overlap(layer* l, bool boost) {

	CHECK(l->input_overlap != NULL);

	l->number_of_columns_active = 0;

	// WP: Line 1. for c in columns

	l->worst_column_overlap = INFINITY;
	l->best_column_overlap= -INFINITY;

	for (unsigned int c = 0; c < l->sp_params.columns; c++) {

		// WP: Line 4. for s in connectedSynapses(c)


		l->columns[c].overlap = calculate_column_overlap(l, c);

		if (l->columns[c].overlap > l->best_column_overlap)
			l->best_column_overlap = l->columns[c].overlap;

		if (l->columns[c].overlap < l->worst_column_overlap)
			l->worst_column_overlap = l->columns[c].overlap;

		if (boost) {
			l->columns[c].overlap_duty_cycle = 
			l->sp_params.duty_cycle_history_weight * l->columns[c].overlap_duty_cycle + 
			(1.0f - l->sp_params.duty_cycle_history_weight) * 
			(l->columns[c].overlap >= l->sp_params.min_overlap);
		}

	}

}

void activate_column(layer* l, unsigned int c) {
	if (l) {
		l->columns[c].active[IS] = true;

	}
}

void inhibit_neighboring_columns(layer* l) {

 	// WP: Line 11. for c in columns

  	unsigned int radius = l->sp_params.local_inhibition_radius;
	// for (unsigned int c = 0; c < l->sp_params.columns; c++) {

	// 	// nupic code seemed to be overcomplicated here for a parallel impl.
	// 	// 1. in parallel, this is going to be executed in a random order (+)
	// 	// 2. another possibility is to run local_inhibition(rand(), rand()) 
	// 	// until active columns are less than 2%
	// 	// 3. one more possibility is to calculate mean overlap and base the
	// 	// minimum activation level based on percentile value
	// 	// i.e. P = mean +/- Z * SD, 
	// 	// 97,5 percentile = mean + 1,96 * Standard deviation
	// 	// currently, method 1. is implemented
	// 	if (l->columns[c].active[IS])
	// 		local_inhibition(l, c, radius);
	// 	// WP: Line 13. minLocalActivity = kthScore(neighbors(c), desiredLocalActivity)
	// 	// WP: Line 14. if overlap(c) > 0 and then overlap(c) >= minLocalActivity then
	// 		// WP: Line 16. activeColumns(t).append(c)
	// }

  	//unsigned max_active_cols = l->sp_params.max_active_columns;//l->sp_params.columns/500 + 1;

  	// float old_reconstruction_score = 0.0f;
  	// float reconstruction_score = 0.0f;

	  	l->number_of_columns_active = 0;
	  	for (unsigned active_cols = 0; active_cols < l->sp_params.max_active_columns; active_cols++) {

	  		// unsigned last_added = 
	  		find_best_active_column(l, radius);
	  		l->number_of_columns_active++;
	   	}


}


unsigned find_best_active_column(layer* l, unsigned int radius) {

	unsigned col = 0;
	float highest_activation = -INFINITY;

	unsigned starting_column = rand_int() % l->sp_params.columns;

	for (unsigned int current_column = 0; current_column < l->sp_params.columns; current_column++) {

		unsigned c = (starting_column + current_column) % l->sp_params.columns;

		if ((l->columns[c].overlap > highest_activation && !l->columns[c].active[IS])) {

			col = c;
			highest_activation = l->columns[c].overlap;
		
		}

	}

	//just to get rid of warnings
	radius = radius + 1;

	l->columns[col].active[IS] = true;
	l->number_of_columns_active++;

	return col;
}


void update_columns(layer* l, float multiplier) {

	// Not converting the whole array into a list of active columns
	// Might not give any speedup in parallel version, plus
	// it needs more memory
	// Instead just checking if a column is active

	// if learning is enabled
	if (l->spatial_learning_enabled) {

		// WP: Line 18. for c in activeColumns(t)
		CHECK(l->columns != NULL);

		if (l->columns) {
			unsigned starting_column = rand_int() % l->sp_params.columns;

			for (unsigned int current_column = 0; current_column < l->sp_params.columns; current_column++) {

				unsigned c = (starting_column + current_column) % l->sp_params.columns;

				if (l->columns[c].active[IS]) {

					// WP: Line 20. for s in potentialSynapses(c) 
					for (unsigned int s = 0; s < l->sp_params.proximal_synapses_per_column; s++) {

						float update = powf( ( (float)fabs(IS_BIT_SET_01(l->input_float_sdr, l->columns[c].synapses[s].column_connection) - 0.5f) * ((float)fabs(IS_BIT_SET_01(l->input_float_sdr, l->columns[c].synapses[s].column_connection) - l->dynamic_reconstruction.reconstruction_vector[l->columns[c].synapses[s].column_connection]))), 1.0f) * sgn(IS_BIT_SET_01(l->input_float_sdr, l->columns[c].synapses[s].column_connection) - l->dynamic_reconstruction.reconstruction_vector[l->columns[c].synapses[s].column_connection]);
						permanence_update(&(l->columns[c].synapses[s].permanence), l->sp_params.permanence_increment * multiplier * update * ((float)l->sp_params.columns - (float)l->sp_params.max_active_columns)/(float)l->sp_params.columns);// * 1.0f/(l->input_overlap[l->columns[c].synapses[s].column_connection])) ;// * expf(l->columns[c].synapses[s].permanence) * multiplier * 1.0f/((l->input_overlap[s])));
					}//: for

				} else {
				 	for (unsigned int s = 0; s < l->sp_params.proximal_synapses_per_column; s++) {
						float update = powf( ((float)fabs(IS_BIT_SET_01(l->input_float_sdr, l->columns[c].synapses[s].column_connection) - 0.5f) * ((float)fabs(IS_BIT_SET_01(l->input_float_sdr, l->columns[c].synapses[s].column_connection) - l->dynamic_reconstruction.reconstruction_vector[l->columns[c].synapses[s].column_connection]))), 1.0f) * sgn(IS_BIT_SET_01(l->input_float_sdr, l->columns[c].synapses[s].column_connection) - l->dynamic_reconstruction.reconstruction_vector[l->columns[c].synapses[s].column_connection]);
				 		permanence_update(&(l->columns[c].synapses[s].permanence), -l->sp_params.permanence_increment * multiplier * update * (float)l->sp_params.max_active_columns/(float)l->sp_params.columns);// * 1.0f/(l->input_overlap[l->columns[c].synapses[s].column_connection])) ;// * expf(l->columns[c].synapses[s].permanence) * multiplier * 1.0f/((l->input_overlap[s])));
				 	}//: for
				}//: else
			}//: if columns :]
		} // if (l->columns)
	} // if (l->spatial_learning_enabled)
	l->max_sum_of_permanences = get_max_sum_of_permanences(l);
}

void permanence_update(float* p, const float val) {

#ifdef _WIN32
	if (!(std::isnan(val)))
#else    
	if (!isnan(val))
#endif    
		*p = *p + val;
}


float max_duty_cycle(layer* l) {

	float max_duty_cycle = -INFINITY;
	for (unsigned i = 0; i <= l->sp_params.columns; i++) {
		if (l->columns[i].active_duty_cycle > max_duty_cycle)
			max_duty_cycle = l->columns[i].active_duty_cycle;
	}//: for

	return max_duty_cycle;
}

void active_columns_to_bit_vector(layer* l, bool* vector) {

	for (unsigned int c = 0; c < l->sp_params.columns; c++) {
		vector[c] = l->columns[c].active[IS];
	}//: for

}

void active_columns_to_float_vector(layer* l, float* vector) {

	for (unsigned int c = 0; c < l->sp_params.columns; c++) {
		vector[c] = l->columns[c].overlap;
	}//: for

}

void update_full_connectivity_vectors(layer *l) {

	for (unsigned int c = 0; c < l->sp_params.columns; c++) {

		for (unsigned i = 0; i < l->sp_params.input_vector_length; i++) {
			l->columns[c].full_input_vector[i] = 0.0f;
		}


		for (unsigned s = 0; s < l->sp_params.proximal_synapses_per_column; s++) {

			l->columns[c].full_input_vector[l->columns[c].synapses[s].column_connection] += l->columns[c].synapses[s].permanence;

		}
	}
}

float compute_orthogonality(layer* l, unsigned c0, unsigned c1) {

	float dot_product = 0.0f;
	float cosine_similarity = 0.0f;
	float length_a = 0.0f;
	float length_b = 0.0f;

	// proximal connections can be sparse, so first we need to map them to input vectors: 
	// run update_full_connectivity_vectors(layer *l) first -> use full_input_vector
	
	//now we can compute the dot product of a and b (perm = 0.5 is 'zero state') and lengths of the vectors
	for (unsigned i = 0; i < l->sp_params.input_vector_length; i++) {

		dot_product += (l->columns[c0].full_input_vector[i] - 0.5f) * (l->columns[c1].full_input_vector[i] - 0.5f);
		length_a += (l->columns[c0].full_input_vector[i] - 0.5f) * (l->columns[c0].full_input_vector[i] - 0.5f);
		length_b += (l->columns[c1].full_input_vector[i] - 0.5f) * (l->columns[c1].full_input_vector[i] - 0.5f);

	}

	length_a = sqrtf(length_a);
	length_b = sqrtf(length_b);

	cosine_similarity = dot_product/(length_a * length_b);

	return cosine_similarity;
}

float compute_column_permanence(layer* l, unsigned c0) {
	// Compute column permanence factor for a given column.
	float permanence_norm = 0;
	for (unsigned s = 0; s < l->sp_params.proximal_synapses_per_column; s++)
		permanence_norm += l->columns[c0].synapses[s].permanence * l->columns[c0].synapses[s].permanence;
	return sqrt(permanence_norm);
}//:

} /* namespace htm */
} /* namespace mic */
