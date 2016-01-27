#include <htm/reconstruct.h>

#include <logger/Log.hpp>

namespace mic {
namespace htm {

/*
void allocate_reconstruction_space(reconstruction* r, size_t vector_length) {

	r->reconstruction_vector = (float*)ALLOC(vector_length * sizeof(float));
	r->reconstruction_vector_binary = (float*)ALLOC(vector_length * sizeof(float));

}

void deallocate_reconstruction_space(reconstruction* r) {

	FREE(r->reconstruction_vector);
	FREE(r->reconstruction_vector_binary);

}*/

void destroy_reconstruction_memory(layer *l) {

/*	for (unsigned i = 0; i < l->sp_params.columns; i++)
	 	deallocate_reconstruction_space(&l->static_reconstruction[i]);*/

	l->reconstruction_memory_usage -= l->sp_params.columns * l->sp_params.input_vector_length * sizeof(float) * 2;

	free(l->static_reconstruction);

	l->reconstruction_memory_usage -= l->sp_params.columns * sizeof(reconstruction);

/*	deallocate_reconstruction_space(&l->dynamic_reconstruction);
	deallocate_reconstruction_space(&l->prediction_reconstruction);*/

	l->reconstruction_memory_usage -= l->sp_params.input_vector_length * sizeof(float) * 4;


}

void initialize_reconstruction_memory(layer *l) {

	l->static_reconstruction = (reconstruction*) malloc(l->sp_params.columns * sizeof(reconstruction));

	l->reconstruction_memory_usage += l->sp_params.columns * sizeof(reconstruction);

/*	for (unsigned i = 0; i < l->sp_params.columns; i++)
	 	allocate_reconstruction_space(&l->static_reconstruction[i], l->sp_params.input_vector_length);*/ // TK size OK. NOT REQURED ANYMORE!!

	l->reconstruction_memory_usage += l->sp_params.columns * l->sp_params.input_vector_length * sizeof(float) * 2;

/*	allocate_reconstruction_space(&l->dynamic_reconstruction, l->sp_params.input_vector_length);
	allocate_reconstruction_space(&l->prediction_reconstruction, l->sp_params.input_vector_length);*/

	l->reconstruction_memory_usage += l->sp_params.input_vector_length * sizeof(float) * 4;

}

void reset_reconstruction_space(reconstruction* r, size_t vector_length) {

	for (unsigned int i = 0; i < vector_length; i++) {

		r->reconstruction_vector[i] = 0.0f;
		r->reconstruction_vector_binary[i] = 0.0f;

	}

}

void compute_static_reconstruction(layer* l) {

	if (l) {

		CHECK(l->columns != NULL);

		if (l->columns) {

			for (unsigned int c = 0; c < l->sp_params.columns; c++) {

				reset_reconstruction_space(&l->static_reconstruction[c], l->sp_params.input_vector_length);

				float reconstruction_max = 0.0f;
				float binary_reconstruction_max = 0.0f;

				for (unsigned int s = 0; s < l->sp_params.proximal_synapses_per_column; s++) {

					//l->static_reconstruction[c].reconstruction_vector[l->columns[c].synapses[s].column_connection] = get_permanence(l, c, s)/sum_of_squares;//l->columns[c].synapses[s].permanence;
					l->static_reconstruction[c].reconstruction_vector[l->columns[c].synapses[s].column_connection] = get_permanence(l, c, s);

					if (reconstruction_max < l->static_reconstruction[c].reconstruction_vector[l->columns[c].synapses[s].column_connection])
						reconstruction_max = l->static_reconstruction[c].reconstruction_vector[l->columns[c].synapses[s].column_connection];

					if (l->columns[c].synapses[s].permanence >= SYNAPSE_CONNECTED_PERMANENCE) {
						
						l->static_reconstruction[c].reconstruction_vector_binary[l->columns[c].synapses[s].column_connection] += l->columns[c].synapses[s].permanence * 2.0f - 0.5f;

						if (binary_reconstruction_max < l->static_reconstruction[c].reconstruction_vector_binary[l->columns[c].synapses[s].column_connection])
							binary_reconstruction_max = l->static_reconstruction[c].reconstruction_vector_binary[l->columns[c].synapses[s].column_connection];

					}

				}

				//printf("%d %f %f\n", c, reconstruction_max, binary_reconstruction_max);

				for (unsigned i = 0; i < l->sp_params.input_vector_length; i++) {

					// l->static_reconstruction[c].reconstruction_vector_binary[i] /= binary_reconstruction_max;
					//l->static_reconstruction[c].reconstruction_vector[i] /= (reconstruction_max);
				}

			}


		}

	}
}

void compute_dynamic_reconstruction(layer* l) {

	if (l) {

		reset_reconstruction_space(&l->dynamic_reconstruction, l->sp_params.input_vector_length);
		// reset_reconstruction_space(&l->prediction_reconstruction, l->sp_params.input_vector_length);

		// float binary_reconstruction_max = 0.0f;
		// float reconstruction_max = 0.0f;

		if (l->columns) {

			for (unsigned int c = 0; c < l->sp_params.columns; c++) {

				for (unsigned int s = 0; s < l->sp_params.proximal_synapses_per_column; s++) {

					// if (l->columns[c].synapses[s].permanence >= SYNAPSE_CONNECTED_PERMANENCE) {

					// 	if (l->columns[c].active[IS]) {

					// 		l->dynamic_reconstruction.reconstruction_vector_binary[l->columns[c].synapses[s].column_connection] += l->columns[c].overlap * l->columns[c].synapses[s].permanence;
					// 		if (binary_reconstruction_max < l->dynamic_reconstruction.reconstruction_vector_binary[l->columns[c].synapses[s].column_connection])
					// 			binary_reconstruction_max = l->dynamic_reconstruction.reconstruction_vector_binary[l->columns[c].synapses[s].column_connection];
					// 	}

					// 	if (l->columns[c].predicting[IS]) 
					// 		l->prediction_reconstruction.reconstruction_vector_binary[l->columns[c].synapses[s].column_connection] += 1.0f;
					// }

					if (l->columns[c].active[IS]) {


						l->dynamic_reconstruction.reconstruction_vector[l->columns[c].synapses[s].column_connection] += (float)(l->columns[c].synapses[s].permanence - 0.5f);// * sigmoid(l->columns[c].overlap);

						// if (reconstruction_max < l->dynamic_reconstruction.reconstruction_vector[l->columns[c].synapses[s].column_connection])
						// 	reconstruction_max += l->dynamic_reconstruction.reconstruction_vector[l->columns[c].synapses[s].column_connection];

					}

					// if (l->columns[c].predicting[IS])
					// 	l->prediction_reconstruction.reconstruction_vector[l->columns[c].synapses[s].column_connection] += l->columns[c].synapses[s].permanence;

				}
			}

			for (unsigned i = 0; i < l->sp_params.input_vector_length; i++) {

				// l->dynamic_reconstruction.reconstruction_vector_binary[i] /= binary_reconstruction_max;

				l->dynamic_reconstruction.reconstruction_vector[i] = l->dynamic_reconstruction.reconstruction_vector[i];

			}
		}
	}

}

float compute_dynamic_reconstruction_score(layer *l) {

	float error = 0.0f;
	float total = 0.0f;
	
	for (unsigned int s = 0; s < l->sp_params.input_vector_length; s++) {

			if (l->input_float_sdr[s]) {

				error += (float)fabs(l->dynamic_reconstruction.reconstruction_vector[s] - l->input_float_sdr[s]) * (float)fabs(l->dynamic_reconstruction.reconstruction_vector[s] - l->input_float_sdr[s]);
				total += 1.0f;
			}

	}

	return sqrtf((1.0f - error/total));

}

float get_permanence(layer* l, unsigned i, unsigned j) {

	return l->columns[i].synapses[j].permanence;
}

} /* namespace htm */
} /* namespace mic */
