/*!
 * \file PerformanceStatistics.hpp
 * \brief 
 * \author tkornut
 * \date Dec 8, 2015
 */

#ifndef SRC_HTM_PERFORMANCESTATISTICS_HPP_
#define SRC_HTM_PERFORMANCESTATISTICS_HPP_

#include <sys/time.h>

namespace mic {
namespace htm {

/*!
 * \brief Class for storing the performance statistics for HTM/Spatial Pooler.
 * \author tkornuta/krocki
 */
class PerformanceStatistics {
public:
	PerformanceStatistics();
	virtual ~PerformanceStatistics();

	    struct timeval 		collection_time;

	    long double			time_since_program_start;

		// columns
		unsigned long long 	active_columns;
		unsigned long long 	shared_sdr_columns;
		unsigned long long 	predicted_columns;
		unsigned long long 	unpredicted_columns;
		unsigned long long 	predicting_columns;
		unsigned long long 	mispredicted_columns;

		// proximal synapses
		unsigned long long 	total_connected_proximal_synapses;
		unsigned long long 	active_connected_proximal_synapses;
		unsigned long long 	active_disconnected_proximal_synapses;

		long double			sum_of_permanences_proximal_synapses;

		//cells
		unsigned long long 	active_cells;
		unsigned long long 	learning_cells;
		unsigned long long 	shared_context_cells;
		unsigned long long 	predicted_cells;
		unsigned long long 	predicting_cells;
		unsigned long long 	unpredicted_cells;
		unsigned long long 	mispredicted_cells;
		unsigned long long	segment_change_cells;
		unsigned int 		fewest_segments;
		unsigned int 		median_segments;
		unsigned int 		mean_segments;
		unsigned int 		most_segments;

		//segments
		unsigned long long 	total_segments;
		unsigned long long	active_segments;
		unsigned long long 	connected_active_segments;
		unsigned long long 	disconnected_active_segments;
		unsigned long long 	sequence_segments;
		unsigned long long 	new_segments;

		unsigned long long	spatial_memory_preallocated;
		unsigned long long	segment_memory_preallocated;
		unsigned long long	segment_memory_usage;
		unsigned long long	reconstruction_memory_preallocated;

		long double 		min_segment_age;
		long double 		avg_segment_age;
		long double 		max_segment_age;
		long double			avg_active_duty_cycle;

		//distal synapses
		unsigned long long 	total_connected_distal_synapses;
		unsigned long long	active_distal_synapses;
		unsigned long long 	active_connected_distal_synapses;
		unsigned long long 	active_disconnected_distal_synapses;
		unsigned long long 	invalid_distal_synapses;
		unsigned long long 	valid_distal_synapses;
		unsigned long long 	mispredicting_distal_synapses;
		unsigned long long 	cached_distal_synapses;
		unsigned long long 	altered_distal_synapses;

		void logPerformanceStatistics();

};

} /* namespace htm */
} /* namespace mic */

#endif /* SRC_HTM_PERFORMANCESTATISTICS_HPP_ */