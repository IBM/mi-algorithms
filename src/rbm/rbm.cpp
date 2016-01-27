/*!
 * \file rbm.cpp
 * \brief Contains RBM methods declaration.
 * \author tkornuta
 * \date Dec 22, 2015
 */

#include <rbm/rbm.hpp>

namespace mic {
namespace rbm {

RBM::RBM(size_t n_visible, size_t n_hidden, size_t b_size, DISPLAY_MODE dm) {

	num_input = n_visible;
	num_output = n_hidden;
	batch_size = b_size;

	// parameters
	W = new Matrix<float>(num_output, num_input);
	W_delta = new Matrix<float>(num_output, num_input);
	sigma = new Matrix<float>(num_input, 1);
	b = new Matrix<float>(1, num_input);
	b_delta = new Matrix<float>(1, num_input);
	c = new Matrix<float>(num_output, 1);
	c_delta = new Matrix<float>(num_output, 1);

	//temp storage
	h = new Matrix<float>(num_output, batch_size);
	H2 = new Matrix<float>(num_output, batch_size);
	hn = new Matrix<float>(num_output, batch_size);
	H = new Matrix<float>(num_output, batch_size);
	rv = new Matrix<float>(batch_size, num_input);
	rh = new Matrix<float>(num_output, batch_size);

	hidmeans = new Matrix<float>(num_output, 1);
	hidmeans_inc = new Matrix<float>(num_output, 1);
	hidmeans_inc_rep = new Matrix<float>(num_output, batch_size);
	sparsegrads = new Matrix<float>(num_output, num_input);

	v = new Matrix<float>(batch_size, num_input);
	vn = new Matrix<float>(batch_size, num_input);
	pc = new Matrix<float>(batch_size, num_input);

	ve = new Matrix<float>(batch_size, num_input);

	posprods = new Matrix<float>(num_output, num_input);
	negprods = new Matrix<float>(num_output, num_input);

	//params init
	W->elementwise_function(&_randn);
	W->elementwise_function_scalar(&_mult, 0.001f);

	W->display_mode = DISPLAY_MODE::ROWS_ARE_WEIGHTS;
	H->display_mode = DISPLAY_MODE::ROWS_ARE_IMAGES;
	v->display_mode = dm;
	vn->display_mode = dm;
	pc->display_mode = dm;
	h->display_mode = DISPLAY_MODE::ROWS_ARE_IMAGES;
	hn->display_mode = DISPLAY_MODE::ROWS_ARE_IMAGES;

	mflops = (num_output * num_input * batch_size * 2 * 4 +  //SGEMMs
	          num_output * batch_size * 8	//OPs on hidden
	         ) / (1024 * 1024);

	if (learning_type == LTYPE::PCD)
		hn->elementwise_function(&_rand);

}

RBM::RBM(size_t n_visible, size_t n_hidden, NTYPE v_type, NTYPE h_type, LTYPE l_type, DISPLAY_MODE dm, size_t b_size) :
		RBM(n_visible, n_hidden, b_size, dm)
{
	visible_type = v_type;
	hidden_type = h_type;
	learning_type = l_type;
}

RBM::~RBM() {

	if (W != nullptr) delete(W);
	if (W_delta != nullptr) delete(W_delta);
	if (sigma != nullptr) delete(sigma);
	if (b != nullptr) delete(b);
	if (c != nullptr) delete(c);
	if (b_delta != nullptr) delete(b_delta);
	if (c_delta != nullptr) delete(c_delta);
	if (v != nullptr) delete(v);
	if (vn != nullptr) delete(vn);
	if (pc != nullptr) delete(pc);
	if (ve != nullptr) delete(ve);
	if (h != nullptr) delete(h);
	if (hidmeans != nullptr) delete(hidmeans);
	if (hidmeans_inc != nullptr) delete(hidmeans_inc);
	if (hidmeans_inc_rep != nullptr) delete(hidmeans_inc_rep);
	if (sparsegrads != nullptr) delete(sparsegrads);
	if (hn != nullptr) delete(hn);
	if (H != nullptr) delete(H);
	if (H2 != nullptr) delete(H2);
	if (rv != nullptr) delete(rv);
	if (rh != nullptr) delete(rh);
	if (posprods != nullptr) delete(posprods);
	if (negprods != nullptr) delete(negprods);

}

void RBM::up(Matrix<float>* in) { // h given v

	MATRIX_MEMCPY(v, in);

	//v->flatten();
	// rv->elementwise_function(&_rand);
	// v->elementwise_function_matrix(&_compare, *rv);

	v->transpose();
	Matrix<float>::sgemm(*h, *W, *v);
	v->transpose();

	h->matrix_row_vector_function(&_add, *c);
	h->elementwise_function(&_sigmoid);

	Matrix<float>::sgemm(*posprods, *h, *v);

}

void RBM::down(Matrix<float>* in) { // v given h

	memcpy(H->data, h->data, sizeof(float) * h->elements);
	rh->elementwise_function(&_rand);
	H->elementwise_function_matrix(&_compare, *rh);
	H->transpose();
	Matrix<float>::sgemm(*vn, *H, *W);
	H->transpose();
	vn->matrix_column_vector_function(&_add, *b);
	vn->elementwise_function(&_sigmoid);

	if (learning_type == LTYPE::CD) {

		vn->transpose();
		Matrix<float>::sgemm(*hn, *W, *vn);
		vn->transpose();
		hn->matrix_row_vector_function(&_add, *c);
		hn->elementwise_function(&_sigmoid);

		Matrix<float>::sgemm(*negprods, *hn, *vn);
		MATRIX_MEMCPY(pc, vn);

	} else {

		memcpy(H->data, hn->data, sizeof(float) * h->elements);
		rh->elementwise_function(&_rand);
		H->elementwise_function_matrix(&_compare, *rh);
		H->transpose();
		Matrix<float>::sgemm(*pc, *H, *W);
		H->transpose();
		pc->matrix_column_vector_function(&_add, *b);
		pc->elementwise_function(&_sigmoid);
		pc->transpose();
		Matrix<float>::sgemm(*hn, *W, *pc);
		pc->transpose();
		hn->matrix_row_vector_function(&_add, *c);
		hn->elementwise_function(&_sigmoid);

		Matrix<float>::sgemm(*negprods, *hn, *pc);

	}

}

void RBM::compute_statistics() {

	MATRIX_MEMCPY(ve, vn);
	ve->elementwise_function_matrix(&_sub, *v);
	// ve->elementwise_function(&_square);
	// err = ve->sum();
	hsum = h->sum() / (batch_size * num_output);
	err = ve->norm();
	err *= err / batch_size;

	MATRIX_MEMCPY(H2, h);
	H2->elementwise_function(&_H2);
	I = H2->sum() / (batch_size * num_output);

}

void RBM::adapt(float alpha, float decay, float sparsecost, float sparsetarget, float sparsedamping) {

	// reset
	W_delta->elementwise_function(&_zero);
	b_delta->elementwise_function(&_zero);
	c_delta->elementwise_function(&_zero);

	// hidmeans = sparsedamping*hidmeans + (1-sparsedamping)*poshidact/numcases;
	hidmeans->elementwise_function_scalar(&_mult, sparsedamping);
	hidmeans_inc->sum_rows(*h);
	hidmeans_inc->elementwise_function_scalar(&_mult, (1.0f - sparsedamping) / batch_size);
	hidmeans->elementwise_function_matrix(&_add, *hidmeans_inc);

	// sparsegrads = sparsecost*(repmat(hidmeans,numcases,1)-sparsetarget);
	MATRIX_MEMCPY(hidmeans_inc, hidmeans);
	hidmeans_inc->elementwise_function_scalar(&_sub, sparsetarget);
	hidmeans_inc->elementwise_function_scalar(&_mult, sparsecost);
	Matrix<float>::repmat(*hidmeans_inc_rep, *hidmeans_inc, batch_size);
	Matrix<float>::sgemm(*sparsegrads, *hidmeans_inc_rep, *v);
	hidmeans_inc->sum_rows(*sparsegrads);
	// compute
	MATRIX_MEMCPY(W_delta, posprods);
	W_delta->elementwise_function_matrix(&_sub, *negprods);
	W_delta->elementwise_function_matrix(&_sub, *sparsegrads);
	W_delta->elementwise_function_scalar(&_mult, alpha / batch_size);

	b_delta->diff_cols(*v, *pc);
	b_delta->elementwise_function_scalar(&_mult, alpha / batch_size);

	c_delta->diff_rows(*h, *hn);
	c_delta->elementwise_function_matrix(&_sub, *hidmeans_inc);
	c_delta->elementwise_function_scalar(&_mult, alpha / batch_size);

	// apply
	W->elementwise_function_matrix(&_add, *W_delta);
	W->elementwise_function_scalar(&_mult, 1.0f - decay);
	b->elementwise_function_matrix(&_add, *b_delta);
	c->elementwise_function_matrix(&_add, *c_delta);
}


void RBM::serialize(std::ostream& os) const {

	os << "[" << num_input << " x " << num_output << " Encoder]" << std::endl;
	os << *W << std::endl;
}



} /* namespace rbm */
} /* namespace mic */


