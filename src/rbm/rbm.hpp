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
	RBM(const size_t n_visible, const size_t n_hidden, const size_t b_size);

	/*!
	 * Constructor.
	 * @param n_visible
	 * @param n_hidden
	 * @param v_type
	 * @param h_type
	 * @param l_type
	 * @param b_size
	 */
	RBM(const size_t n_visible, const size_t n_hidden, NTYPE v_type, NTYPE h_type, LTYPE l_type = LTYPE::CD,
			const size_t b_size = 1);

	/*!
	 * Destructor. Deallocates all matrices.
	 */
	virtual ~RBM();

	/*!
	 * ?
	 * @param in
	 */
	void up(mic::types::MatrixXfPtr in);

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


	mic::types::MatrixXf W;
	//mic::types::VectorXf sigma;
	mic::types::VectorXf b;
	mic::types::VectorXf c;

	mic::types::MatrixXf W_delta;
	mic::types::VectorXf b_delta;
	mic::types::VectorXf c_delta;

	mic::types::MatrixXf h;
	mic::types::VectorXf hidmeans;
	mic::types::VectorXf hidmeans_inc;
	mic::types::MatrixXf hidmeans_inc_rep;
	mic::types::MatrixXf sparsegrads;
	//mic::types::MatrixXf H2;
	mic::types::MatrixXf hn;
	mic::types::MatrixXf H;
	mic::types::MatrixXf rv;

	/// Matrix (of size BxI) - all elements contain random variables (uniform 0,1), used for thresholding h.
	mic::types::MatrixXf rh;

	mic::types::MatrixXf v;
	mic::types::MatrixXf vn;
	mic::types::MatrixXf ve;
	mic::types::MatrixXf pc;

	mic::types::MatrixXf posprods;
	mic::types::MatrixXf negprods;

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
