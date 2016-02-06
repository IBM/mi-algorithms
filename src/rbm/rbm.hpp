/*!
* @File: rbm.hpp
* @Author: kamilrocki
* @Email: kmrocki@us.ibm.com
* @Created:	2015-04-26 10:11:19
* @Last Modified by:   kamilrocki
* @Last Modified time: 2015-10-16 10:10:50
* \brief Contains RBM definition.
*/

#ifndef __CXXRBM_H__
#define __CXXRBM_H__

#include <types/MatrixTypes.hpp>

using namespace mic::types;

namespace mic {

/**
 * \namespace mic::rbm
 * \brief Namespace containing RBM (Restricted Boltzmann Machine) related structures, classes and functions.
 * \author tkornuta
 */
namespace rbm {

enum class NTYPE {BINARY, GAUSSIAN, RELU};
enum class LTYPE {CD, PCD};


/**
 * \brief Class representing Restricted Boltzmann Machine (RBM).
 * \author krocki
 */
class RBM {

  public:

	RBM() = default;

	/*!
	 * Constructor with reduced number of parameters. Sets parameters and allocates matrices.
	 * @param n_visible Number of units in the visible (input) layer.
	 * @param n_hidden Number of units in the hidden layer.
	 * @param b_size Batch size.
	 */
	RBM(size_t n_visible, size_t n_hidden, size_t b_size);

	/*!
	 * Constructor.
	 * @param n_visible
	 * @param n_hidden
	 * @param v_type
	 * @param h_type
	 * @param l_type
	 * @param b_size
	 */
	RBM(size_t n_visible, size_t n_hidden, NTYPE v_type, NTYPE h_type, LTYPE l_type = LTYPE::PCD,
	    size_t b_size = 1);

	/*!
	 * Destructor. Deallocates all matrices.
	 */
	virtual ~RBM();

	/*!
	 * ?
	 * @param in
	 */
	void up(mic::types::matrixd_ptr_t in);

	/*!
	 * ?
	 */
	void down();

	/*!
	 * Computes the statistics. TODO: remove.
	 */
	void compute_statistics();

	/*!
	 * Adaptation.
	 * @param alpha
	 * @param decay
	 * @param sparsecost
	 * @param sparsetarget
	 * @param sparsedamping
	 */
	void adapt(float alpha = 0.001f, float decay = 0.0000f, float sparsecost = 0.0f,
	           float sparsetarget = 0.1f, float sparsedamping = 0.0f);

	/*!
	 * Serializes the RBM settings into ostream.
	 * @param os
	 */
	virtual void serialize(std::ostream& os) const;

	/*!
	 * Number of units in input (visible) layer.
	 */
	size_t num_input;

	/*!
	 * Number of units in hidden (output?) layer.
	 */
	size_t num_output;

	/*!
	 * Batch size - size of data samples (images, letters etc.) trained at once.
	 */
	size_t batch_size;


	mic::types::matrixd_t W;
	mic::types::matrixd_t sigma;
	mic::types::matrixd_t b;
	mic::types::matrixd_t c;

	mic::types::matrixd_t W_delta;
	mic::types::matrixd_t b_delta;
	mic::types::matrixd_t c_delta;

	mic::types::matrixd_t h;
	mic::types::matrixd_t hidmeans;
	mic::types::matrixd_t hidmeans_inc;
	mic::types::matrixd_t hidmeans_inc_rep;
	mic::types::matrixd_t sparsegrads;
	//mic::types::matrixd_t H2;
	mic::types::matrixd_t hn;
	mic::types::matrixd_t H;
	mic::types::matrixd_t rv;
	mic::types::matrixd_t rh;

	mic::types::matrixd_t v;
	mic::types::matrixd_t vn;
	mic::types::matrixd_t ve;
	mic::types::matrixd_t pc;

	mic::types::matrixd_t posprods;
	mic::types::matrixd_t negprods;

	float err;
	float hsum;
	// float I; -- commented as not used!
	//double mflops;

	NTYPE visible_type;
	NTYPE hidden_type;
	LTYPE learning_type;

};

} /* namespace rbm */
} /* namespace mic */

#endif /*__RBM_H__*/
