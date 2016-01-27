#ifndef __SEQUENCE_MEMORY_H__
#define __SEQUENCE_MEMORY_H__

#include <htm/layer.h>

#include <data_utils/rand.hpp>
using namespace mic::data_utils;


namespace mic {
namespace htm {

int initialize_sequence_memory(layer* l);
int initialize_learning_cache(layer* l);
void initialize_cells(layer* l);
void initialize_segments(layer* l);
void reset_learning_cache(layer* l);
void cache_learning_synapse(layer* l, synapse s);
synapse retrieve_learning_synapse(layer* l, bool remove_from_cache);
void fill_learning_cache(layer* l);
void reset_segment(segment* seg, unsigned int i, layer* l);
void reset_synapse(synapse* s, layer* l);
void destroy_sequence_memory(layer* l);

bool check_segment_state(layer* l, segment* seg, moment_t when, BYTE state, bool only_connected_synapses);
unsigned int learn_prediction(layer* l, unsigned long c);
void forget_prediction(layer* l, unsigned long c, unsigned int cell);
void adjust_segment(layer* l, segment* seg);
void reinforce_prediction(layer* l, unsigned long c, unsigned int cell);
unsigned int segment_activation_level(layer* l, segment* seg, moment_t when, BYTE state, bool only_connected_synapses);
segment* add_new_segment_to_cell(layer* l, unsigned long column, unsigned int cell);
segment* add_new_segment(layer* l, unsigned long column);
void queue_synapse_updates(layer* l, segment* seg);
void commit_segment_updates(layer* l);
void commit_synapse_updates(layer* l, unsigned int column, bool negative_reinforcement);
segment* get_active_segment(layer* l, unsigned long c, unsigned int i, moment_t when, BYTE state);
segment* get_best_matching_segment(layer* l, unsigned long c, moment_t when);
void get_segment_active_synapses(layer* l, segment* s, moment_t when, bool new_synapses);
void adapt_segments(layer* l, unsigned long c, unsigned int i, bool positive_reinforcement);

} /* namespace htm */
} /* namespace mic */

#endif
