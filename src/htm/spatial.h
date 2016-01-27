#ifndef __SPATIAL_POOLER_H__
#define __SPATIAL_POOLER_H__

//#include <stdlib.h>
#include <htm/layer.h>

namespace mic {
namespace htm {

int initialize_spatial_pooler(layer* l);
void initialize_columns(layer* l);
void initialize_potential_pool(layer* l);
void destroy_spatial_pooler(layer* l);

void permanence_update(float* p, const float val);

// bool column_state(layer* l, unsigned int c, moment when, BYTE what);


/*!
 * Calculates the overlap for a given column.
 * @author krocki
 * @param c Column number.
 * @return Overlap factor.
 */
float calculate_column_overlap(layer* l, unsigned int c);

/*!
 * Realizes the spatial pooling phase 1: Overlap
 * -------------------
 * Based on the 2011 Whitepaper by Numenta (notes)
 *
 * Given an input vector, the first phase calculates the overlap
 * of each column with that vector. The overlap for each column
 * is simply the number of connected synapses with active inputs,
 * multiplied by its boost. If this value is below minOverlap,
 * we set the overlap score to zero.
 *
 * @author krocki
 */
void calculate_overlap(layer* l, bool boost);


/*!
 * Realizes the spatial pooling phase 2: Inhibition
 * -------------------
 *
 * The second phase calculates which columns remain as winners
 * after the inhibition step. desiredLocalActivity is a parameter
 * that controls the number of columns that end up winning.
 * For example, if desiredLocalActivity is 10, a column will
 * be a winner if its overlap score is greater than the score
 * of the 10'th highest column within its inhibition radius.

 * KR:
 * Method 1.Since k is small, you can use the tournament method
 * to find the kth largest. This method is described
 * in Knuth's Art of Programming, Volume 3, Page 212.
 * Method 2.You can do this in O(n) by using a selection algorithm.
 * Find the kth largest element with the partition algorithm,
 * then all the elements after it will be larger than it,
 * and those are your top k.

 * If you need those top k in sorted order, you can then sort them in O(k log k).
 *
 * @author krocki
 */
void inhibit_neighboring_columns(layer* l);

/*!
 * Finds the best active column within a given "radius"
 * @author krocki
 * @param radius
 */
unsigned find_best_active_column(layer* l, unsigned int radius);

/*!
 * Performs local inhibition of columns "around" the given column within a given "radius".
 * @author krocki
 * @param c
 * @param radius
 */
void local_inhibition(layer* l, unsigned int c, unsigned int radius);

/*!
 * Phase 3: Learning
 * -----------------
 *
 * The third phase performs learning; it updates the permanence values
 * of all synapses as necessary, as well as the boost and inhibition radius.
 *
 * The main learning rule is implemented in lines 20-26.
 * For winning columns, if a synapse is active, its permanence value
 * is incremented, otherwise it is decremented. Permanence values are
 * constrained to be between 0 and 1.
 *
 * Lines 28-36 implement boosting. There are two separate boosting
 * mechanisms in place to help a column learn connections.
 * If a column does not win often enough (as measured by activeDutyCycle),
 * its overall boost value is increased (line 30-32).
 * Alternatively, if a column's connected synapses do not overlap
 * well with any inputs often enough (as measured by overlapDutyCycle),
 * its permanence values are boosted (line 34-36). Note: once learning
 * is turned off, boost(c) is frozen.
 * Finally, at the end of Phase 3 the inhibition radius is recomputed (l. 38).
 *
 * @param multiplier Learning factor.
 *
 * @author krocki
 */
void update_columns(layer* l, float multiplier);

/*!
 * Activates given column.
 * @param c Column number.
 *
 * @author krocki
 */
void activate_column(layer* l, unsigned int c);

/*!
 * Computes max duty cycle.
 *
 * @author krocki
 */
float max_duty_cycle(layer* l);

/*!
 * Calculates the max sum of permanences.
 * @return Sum of permanences.
 *
 * @author krocki
 */
float get_max_sum_of_permanences(layer* l);

/*!
 * Updates the connectivity vectors.
 *
 * @author krocki
 */
void update_full_connectivity_vectors(layer *l);

/*!
 * \brief Computes orthogonality between two columns.
 * Orthogonality denotes how similar the activation of those columns will be for the same input vector.
 *
 * Computes the 'angle' between two features (1 = same, 0 = dissimilar)
// http://pyevolve.sourceforge.net/wordpress/?p=2497
 *
 * @param c0 First column number.
 * @param c1 Second column number
 * @return Computed orthogonality.
 *
 * @author krocki
 */
float compute_orthogonality(layer* l, unsigned c0, unsigned c1);

/*!
 * \brief Computes connection factor for a given column.
 * Connection factor denotes the general strength of a given column connections.
 *
 * @param c0 The number of column.
 * @return Computed factor.
 *
 * @author krocki
 */
float compute_column_permanence(layer* l, unsigned c0);

/*!
 * Returns active columns as a bit vector.
 * @param vector
 *
 * @author krocki
 */
void active_columns_to_bit_vector(layer* l, bool* vector);

/*!
 * Returns active columns (column overlap factor) as a bit vector.
 * @param vector
 *
 * @author krocki
 */
void active_columns_to_float_vector(layer* l, float* vector);

} /* namespace htm */
} /* namespace mic */

#endif
