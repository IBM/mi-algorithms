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
#include <htm/cla.h>
#include <htm/spatial.h>
#include <htm/sequence.h>

#include <logger/Log.hpp>

#include <data_utils/rand.hpp>
using namespace mic::data_utils;

namespace mic {
namespace htm {


//SM

// Based on the 2011 Whitepaper by Numenta (notes)
// Phase 1: Compute the active state, activeState(t), for each column
// ----------------
// 
// The first phase calculates the active state for each column. 
// For each winning c we determine which cells should become active. 
// If the bottom-up input was predicted by any column 
// (i.e. its predictiveState was 1 due to a sequence segment in the previous time step), 
// then those cells become active (lines 4-9). If the bottom-up input was unexpected 
// (i.e. no cells had predictiveState output on), 
// then each i in the c becomes active (lines 11-13).

void compute_active_state(layer* l) {

	if (l->columns) {

		//tracelog("compute_active_state(layer* l)\n");
		for (unsigned int c = 0; c < l->sp_params.columns; c++) { 

			if (l->columns[c].active[IS]) { // column is NOW ACTIVE

				bool predicted = false;
				bool chosen = false;

				for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {

					if (CHECK_CELL_STATE(l->columns[c].cells[i], WAS, PREDICTING)) {

						segment* s = get_active_segment(l, c, i, WAS, ACTIVE);

						if (s) {
							//if (s->sequence_segment) {

								predicted = true;
								SET_CELL_STATE(l->columns[c].cells[i], IS, ACTIVE);

								if (l->temporal_learning_enabled) {

									if (check_segment_state(l, s, WAS, LEARNING, true)) {

										chosen = true;
										SET_CELL_STATE(l->columns[c].cells[i], IS, LEARNING);

									}

								}

							//}
						}

					}

				}

				if (!predicted) {

					for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {

						SET_CELL_STATE(l->columns[c].cells[i], IS, ACTIVE);

					}
				}

				if (l->temporal_learning_enabled) {

					if (!chosen) {

						segment* bms = get_best_matching_segment(l, c, WAS);

						 if (!bms) {

						 	bms = add_new_segment(l, c);
						 	
						 }

						if (bms) {

							unsigned int bmc = bms->parent_cell_index;
							SET_CELL_STATE(l->columns[c].cells[bmc], IS, LEARNING);
							get_segment_active_synapses(l, bms, WAS, true);
							bms->sequence_update_queued = true;

						} else {

							// couldn't add a new segment
						}

					}

				}

			}
		}
	}
}

void compute_predictive_state(layer* l) {

	if (l) {

		for (unsigned int c = 0; c < l->sp_params.columns; c++) {

			for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {

				for (unsigned int seg = 0; seg < l->columns[c].cells[i].number_of_segments; seg++) {

					segment* current_segment = &(l->SEGMENTS(c, i, seg));

					CHECK (current_segment != NULL);

					if (check_segment_state(l, current_segment, IS, ACTIVE, true)) {

						SET_CELL_STATE(l->columns[c].cells[i], IS, PREDICTING);
						l->columns[c].predicting[IS] = true;

						if (l->temporal_learning_enabled) {

							get_segment_active_synapses(l, current_segment, IS, false);

							current_segment->active = true;
							current_segment->active_duty_cycle -= 0.01f;
							
						}

					}
					
				}

			}

		}
	}

}

void update_synapses(layer* l) {

	if (l) {

		if (l->temporal_learning_enabled) {
			for (unsigned int c = 0; c < l->sp_params.columns; c++) {

				for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {

					if (CHECK_CELL_STATE(l->columns[c].cells[i], IS, LEARNING)) {

						adapt_segments(l, c, i, true);

					} else if (!(CHECK_CELL_STATE(l->columns[c].cells[i], IS, ACTIVE)) &&
								CHECK_CELL_STATE(l->columns[c].cells[i], WAS, PREDICTING)) {

						adapt_segments(l, c, i, false);

					}
				}

			}
		}
	}

}

// move forward in time, update states t-1
void time_step(layer* l) {

	// segment* current_segment = NULL;

	if (l->columns) { // Was l initialized in SP ?

		reset_learning_cache(l);
 		fill_learning_cache(l);
		
		for (unsigned int c = 0; c < l->sp_params.columns; c++) {

			//update columns' state
			l->columns[c].active[WAS] = l->columns[c].active[IS];
			l->columns[c].predicting[WAS] = l->columns[c].predicting[IS];
			l->columns[c].active[IS] = false;
			l->columns[c].predicting[IS] = false;
			l->columns[c].mispredicted = false;
			l->columns[c].unpredicted = false;
			l->columns[c].predicted = false;

			for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {

				// update cells' state
				SHIFT_CELL_STATE(l->columns[c].cells[i]);

				for (unsigned int seg = 0; seg < l->columns[c].cells[i].number_of_segments; seg++) {

					segment* sp = &(l->SEGMENTS(c, i, seg));

					if (sp) {

						sp->predicting = sp->active;
						sp->mispredicted = false;
						sp->active = false;

					}

					
				}

				l->columns[c].cells[i].segment_change = false;

			} // for (unsigned int n = 0; n < SM_CELLS_PER_COLUMN; n++)

		} // for (unsigned int c = 0; c < l->sp_params.columns; c++)

	} else {

		fprintf(stderr, "SM, time_step(): l->columns == NULL\n");
		return;

	}	

}

// void compute_active_state(layer* l) {

// 	if (l->columns) {

// 		//tracelog("compute_active_state(layer* l)\n");
// 		for (unsigned int c = 0; c < l->sp_params.columns; c++) { 

// 			if (l->columns[c].active[IS]) { // column is NOW ACTIVE

// 				if (!l->columns[c].predicting[WAS]) { // Case I: Unpredicted column, LEARN

// 					l->columns[c].unpredicted = true;

// 				} else {	// Case II: Predicted, OK

// 					l->columns[c].predicted = true;
// 				}

// 			} else { // column is NOW INACTIVE

// 				if (l->columns[c].predicting[WAS]) { // Case III: Mispredicted column, CORRECT

// 					l->columns[c].mispredicted = true;

// 				} else {
// 					// Case IV: Nothing
// 					continue;
// 				}

// 			}

// 			// Case I
// 			if (l->columns[c].unpredicted) {

// 				// light up all the cells

// 				for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {

// 					SET_CELL_STATE(l->columns[c].cells[i], IS, ACTIVE);

// 				}

// 				//learn new transition
// 				unsigned int learning_cell = learn_prediction(l, c);

// 				SET_CELL_STATE(l->columns[c].cells[learning_cell], IS, LEARNING);

// 			}

// 			// Case II
// 			if (l->columns[c].predicted) {

// 				// Rewrite predicting cells' states
// 				for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {

// 					if (CHECK_CELL_STATE(l->columns[c].cells[i], WAS, PREDICTING)) {
// 						SET_CELL_STATE(l->columns[c].cells[i], IS, ACTIVE);

// 						reinforce_prediction(l, c, i);

// 						SET_CELL_STATE(l->columns[c].cells[i], IS, LEARNING);
					
// 					}
// 					// else
// 					//  	UNSET_CELL_STATE(l->columns[c].cells[i], IS, ACTIVE); // just to be sure


// 				}

				

// 				//reinforce prediction

// 			}

// 			// Case III
// 			if (l->columns[c].mispredicted) {

// 				// Find mispredicting cells
// 				for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {
// 					if (CHECK_CELL_STATE(l->columns[c].cells[i], WAS, PREDICTING)) {

// 						//do something
// 						forget_prediction(l, c, i);
// 					}

// 				}

// 			}
// 		}
// 	}

// 	reset_learning_cache(l);
// 	fill_learning_cache(l);
// 	commit_segment_updates(l);

// }

// Phase 2: Compute the predicted state, predictiveState(t), for each cell
// ----------------
// 
// void compute_predictive_state(layer* l) {

// 	if (l) {

// 		for (unsigned int c = 0; c < l->sp_params.columns; c++) {

// 			for (unsigned int i = 0; i < SM_CELLS_PER_COLUMN; i++) {

// 				bool predicting = false;

// 				for (unsigned int seg = 0; seg < l->columns[c].cells[i].number_of_segments; seg++) {

// 					segment* current_segment = &(l->SEGMENTS(c, i, seg));

// 					CHECK (current_segment != NULL);

// 					if (segment_activation_level(l, current_segment, IS, ACTIVE, true) >= SM_SEGMENT_ACTIVATION_THRESHOLD) {

// 						current_segment->active = true;
// 						current_segment->active_duty_cycle -= 0.01f;
// 						current_segment->mispredicted = false;

// 						predicting = true;

// 						queue_synapse_updates(l, current_segment);

// 					}
					
// 				}

// 				if (predicting) {
// 					SET_CELL_STATE(l->columns[c].cells[i], IS, PREDICTING);
// 					l->columns[c].predicting[IS] = true;
// 				}

// 			}

// 		}
// 	}

// }

// Phase 3: Update synapses
// ----------------
// 
// void update_synapses(layer* l) {

// 	if (l) {



// 	}
// 	// for (unsigned int c = 0; c < l->sp_params.columns; c++) {

// 	// 	if (l->columns[c].predicted) {
// 	// 		commit_synapse_updates(l, c, true);
// 	// 	}

// 	// 	if (l->columns[c].mispredicted) {
// 	// 		commit_synapse_updates(l, c, false);
// 	// 	}

// 	// }

// }

} /* namespace htm */
} /* namespace mic */
