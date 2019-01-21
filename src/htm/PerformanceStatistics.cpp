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
/*!
 * \file PerformanceStatistics.cpp
 * \brief 
 * \author tkornut
 * \date Dec 8, 2015
 */

#include <htm/PerformanceStatistics.hpp>
#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#endif

// TODO Refactor - configuration!
#include <htm/shared/defs.h>

// getTimeInfo
//#include <system_utils/system_info.hpp>
//using namespace mic::system_utils;

namespace mic {
namespace htm {

PerformanceStatistics::PerformanceStatistics() {
	// Set all data to zero. :]
	//std::memset( this, '\0', sizeof(PerformanceStatistics));
}

PerformanceStatistics::~PerformanceStatistics() {
	// TODO Auto-generated destructor stub
}


void PerformanceStatistics::logPerformanceStatistics() {

	printf("\t-----------------\n");
	std::cout << "\tcollection_time: " << collection_time << std::endl;
	std::cout << "\ttime_since_program_start: " << time_since_program_start << std::endl;

	/* TODO Change SP params to configuration!
	printf("\tactive_columns: %llu (%.3Lf%%)\n", active_columns, (long double)(active_columns * 100.0)/(long double)(l->sp_params.columns));
	*/
	printf("\tshared_sdr_columns: %llu (%.3Lf%%)\n", shared_sdr_columns, (long double)(shared_sdr_columns * 100.0)/(long double)(active_columns));
	printf("\tpredicted_columns: %llu (%.3Lf%%)\n", predicted_columns, (long double)(predicted_columns * 100.0)/(long double)(active_columns));
	printf("\tunpredicted_columns: %llu (%.3Lf%%)\n", unpredicted_columns, (long double)(unpredicted_columns * 100.0)/(long double)(active_columns));

	/* TODO Change SP params to configuration!
	printf("\tpredicting_columns: %llu (%.3Lf%%)\n", predicting_columns, (long double)(predicting_columns * 100.0)/(long double)(l->sp_params.columns));
	*/
	printf("\tmispredicted_columns: %llu (%.3Lf%%)\n", mispredicted_columns, (long double)(mispredicted_columns * 100.0)/(long double)(predicted_columns + mispredicted_columns));
	/* TODO Change SP params to configuration!
	printf("\ttotal_connected_proximal_synapses: %llu (%.3Lf%%)\n", total_connected_proximal_synapses, (long double)(total_connected_proximal_synapses * 100.0)/(long double)(l->sp_params.columns * l->sp_params.proximal_synapses_per_column));
	printf("\tactive_connected_proximal_synapses: %llu (%.3Lf%%)\n", active_connected_proximal_synapses, (long double)(active_connected_proximal_synapses * 100.0)/(long double)(l->sp_params.columns * l->sp_params.proximal_synapses_per_column));
	printf("\tactive_disconnected_proximal_synapses: %llu (%.3Lf%%)\n", active_connected_proximal_synapses, (long double)(active_connected_proximal_synapses * 100.0)/(long double)(l->sp_params.columns * l->sp_params.proximal_synapses_per_column));
	*/
	printf("\tsum_of_permanences_proximal_synapses: %Lf\n", sum_of_permanences_proximal_synapses);
	printf("\tactive_cells: %llu (%.3Lf%%)\n", active_cells, (long double)(active_cells * 100.0)/(long double)(active_columns * SM_CELLS_PER_COLUMN));
	/* TODO Change SP params to configuration!
	printf("\tlearning_cells: %llu (%.3Lf%%)\n", learning_cells, (long double)(learning_cells * 100.0)/(long double)(l->sp_params.columns * SM_CELLS_PER_COLUMN));
	*/
	printf("\tshared_context_cells: %llu (%.3Lf%%)\n", shared_context_cells, (long double)(shared_context_cells * 100.0)/(long double)(active_cells));
	printf("\tpredicted_cells: %llu (%.3Lf%%)\n", predicted_cells, (long double)(predicted_cells * 100.0)/(long double)(active_cells));
	/* TODO Change SP params to configuration!
	printf("\tpredicting_cells: %llu (%.3Lf%%)\n", predicting_cells, (long double)(predicting_cells * 100.0)/(long double)(l->sp_params.columns * SM_CELLS_PER_COLUMN));
	*/
	printf("\tunpredicted_cells: %llu (%.3Lf%%)\n", unpredicted_cells, (long double)(unpredicted_cells * 100.0)/(long double)(active_cells));
	printf("\tmispredicted_cells: %llu (%.3Lf%%)\n", mispredicted_cells, (long double)(mispredicted_cells * 100.0)/(long double)(predicted_cells + mispredicted_cells));
	printf("\tsegment_change_cells: %llu (%.3Lf%%)\n", segment_change_cells, (long double)(segment_change_cells * 100.0)/(long double)(active_cells));
	/* TODO Change SP params to configuration!
	printf("\ttotal_segments: %llu (%.3Lf%%)\n", total_segments, (long double)(total_segments * 100.0)/(long double)(l->sp_params.columns * SM_CELLS_PER_COLUMN * SM_SEGMENTS_PER_CELL));
	*/
	printf("\ttotal_active_segments: %llu (%.3Lf%%)\n", active_segments, (long double)(active_segments * 100.0)/(long double)(total_segments));
	printf("\tconnected_active_segments: %llu (%.3Lf%%)\n", connected_active_segments, (long double)(connected_active_segments * 100.0)/(long double)(active_segments));
	printf("\tdisconnected_active_segments: %llu (%.3Lf%%)\n", disconnected_active_segments, (long double)(disconnected_active_segments * 100.0)/(long double)(active_segments));
	printf("\tsequence_segments: %llu (%.3Lf%%)\n", sequence_segments, (long double)(sequence_segments * 100.0)/(long double)(total_segments));
	printf("\tnew_segments: %llu (%.3Lf%%)\n", new_segments, (long double)(new_segments * 100.0)/(long double)(total_segments));
	printf("\tspatial_memory_preallocated: %.2Lf MB\n", (long double)(spatial_memory_preallocated) / (1024.0 * 1024.0));
	printf("\tsegment_memory_preallocated: %.2Lf MB\n", (long double)(segment_memory_preallocated) / (1024.0 * 1024.0));
	printf("\treconstruction_memory_preallocated: %.2Lf MB\n", (long double)(reconstruction_memory_preallocated) / (1024.0 * 1024.0));
	printf("\tsegment_memory_usage: %.4Lf MB (%.3Lf%%)\n", (long double)(segment_memory_usage)/ (1024.0 * 1024.0), (long double)(segment_memory_usage * 100.0)/(long double)(segment_memory_preallocated) );
	printf("\tmin_segment_age: %.3Lf s\n", min_segment_age);
	printf("\tavg_segment_age: %.3Lf s\n", avg_segment_age);
	printf("\tmax_segment_age: %.3Lf s\n", max_segment_age);
	printf("\tavg_active_duty_cycle: %.3Lf\n", avg_active_duty_cycle);
	printf("\ttotal_connected_distal_synapses: %llu/%llu (%.3Lf%%)\n", total_connected_distal_synapses, (total_segments * SM_SEGMENT_POTENTIAL_POOL), (long double)(total_connected_distal_synapses * 100.0)/(long double)(total_segments * SM_SEGMENT_POTENTIAL_POOL));
	printf("\ttotal_active_distal_synapses: %llu/%llu (%.3Lf%%)\n", active_distal_synapses, (total_segments * SM_SEGMENT_POTENTIAL_POOL), (long double)((active_distal_synapses) * 100.0)/(long double)(total_segments * SM_SEGMENT_POTENTIAL_POOL));
	printf("\tactive_connected_distal_synapses: %llu/%llu (%.3Lf%%)\n", active_connected_distal_synapses, active_distal_synapses, (long double)(active_connected_distal_synapses * 100.0)/(long double)(active_distal_synapses));
	printf("\tactive_disconnected_distal_synapses: %llu/%llu (%.3Lf%%)\n", active_disconnected_distal_synapses, active_distal_synapses, (long double)(active_disconnected_distal_synapses * 100.0)/(long double)(active_distal_synapses));
	printf("\tinvalid_distal_synapses: %llu (%.3Lf%%)\n", invalid_distal_synapses, (long double)(invalid_distal_synapses * 100.0)/(long double)(total_segments * SM_SEGMENTS_PER_CELL));
	printf("\tvalid_distal_synapses: %llu (%.3Lf%%)\n", valid_distal_synapses, (long double)(valid_distal_synapses * 100.0)/(long double)(total_segments * SM_SEGMENTS_PER_CELL));
	printf("\tmispredicting_distal_synapses: %llu (%.3Lf%%)\n", mispredicting_distal_synapses, (long double)(mispredicting_distal_synapses * 100.0)/(long double)(total_segments * SM_SEGMENTS_PER_CELL));
	printf("\tcached_distal_synapses: %llu (%.3Lf%%)\n", cached_distal_synapses, (long double)(cached_distal_synapses * 100.0)/(long double)(total_segments * SM_SEGMENTS_PER_CELL));
	printf("\taltered_distal_synapses: %llu (%.3Lf%%)\n", altered_distal_synapses, (long double)(altered_distal_synapses * 100.0)/(long double)(total_segments * SM_SEGMENTS_PER_CELL));

}
} /* namespace htm */
} /* namespace mic */
