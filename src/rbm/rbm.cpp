/*!
 * \file rbm.cpp
 * \brief Contains RBM methods declaration.
 * \author tkornuta
 * \date Dec 22, 2015
 */

#include <rbm/rbm.hpp>

#include <cmath>
#include <data_utils/RandomGenerator.hpp>

#include <data_utils/functions.h>

#include <logger/Log.hpp>
using namespace mic::logger;

namespace mic {
namespace rbm {

RBM::RBM(size_t n_visible, size_t n_hidden, size_t b_size)
{

	num_input = n_visible;
	num_output = n_hidden;
	batch_size = b_size;

	// parameters
	// W - weights between input and output units.
	W.resize(num_output, num_input);
	W_delta.resize(num_output, num_input);
	//sigma.resize(num_input, 1); // => Eigen::VectorXd
	b.resize(num_input); // => Eigen::VectorXd
	b_delta.resize(num_input); // => Eigen::VectorXd
	c.resize(num_output); // => Eigen::VectorXd
	c_delta.resize(num_output); // => Eigen::VectorXd

	//temp storage
	h.resize(num_output, batch_size);
	//H2.resize(num_output, batch_size);
	hn.resize(num_output, batch_size);
	H.resize(num_output, batch_size);
	rv.resize(batch_size, num_input);
	rh.resize(num_output, batch_size);

	hidmeans.resize(num_output, 1); // => Eigen::VectorXd
	hidmeans_inc.resize(num_output, 1); // => Eigen::VectorXd
	hidmeans_inc_rep.resize(num_output, batch_size);
	sparsegrads.resize(num_output, num_input);

	v.resize(batch_size, num_input);
	vn.resize(batch_size, num_input);
	pc.resize(batch_size, num_input);

	ve.resize(batch_size, num_input);

	posprods.resize(num_output, num_input);
	negprods.resize(num_output, num_input);

	// Reset matrices/vectors.
	W.setZero();
	W_delta.setZero();
	//sigma.setZero();
	b.setZero();
	b_delta.setZero();
	c.setZero();
	c_delta.setZero();
	h.setZero();
	hn.setZero();
	H.setZero();
	rv.setZero();
	rh.setZero();
	hidmeans.setZero();
	hidmeans_inc.setZero();
	hidmeans_inc_rep.setZero();
	sparsegrads.setZero();
	v.setZero();
	vn.setZero();
	pc.setZero();
	ve.setZero();
	posprods.setZero();
	negprods.setZero();

	//params init
	//W->elementwise_function(&_randn);
	//W->elementwise_function_scalar(&_mult, 0.001f);
	// Initialize W with random variables from 0.0f to 0.001f (normal distribution mi=0, variance=1).
	/*for (size_t x=0; x < W.cols(); x++)
		for (size_t y=0; y < W.rows(); y++)
			W(y,x) = 0.001*RAN_GEN->normRandReal();*/
	W.normRandReal(0.0, 0.001);

/*	W->display_mode = DISPLAY_MODE::ROWS_ARE_WEIGHTS;
	H->display_mode = DISPLAY_MODE::ROWS_ARE_IMAGES;
	v->display_mode = dm;
	vn->display_mode = dm;
	pc->display_mode = dm;
	h->display_mode = DISPLAY_MODE::ROWS_ARE_IMAGES;
	hn->display_mode = DISPLAY_MODE::ROWS_ARE_IMAGES;*/

/*	mflops = (num_output * num_input * batch_size * 2 * 4 +  //SGEMMs
	          num_output * batch_size * 8	//OPs on hidden
	         ) / (1024 * 1024);*/

	if (learning_type == LTYPE::PCD) {
		//hn->elementwise_function(&_rand);
		// Initialize hn with random variables from 0.0f to 1.0f (uniform distribution).
		/*for (size_t x=0; x < hn.cols(); x++)
			for (size_t y=0; y < hn.rows(); y++)
				hn(y,x) = RAN_GEN->uniRandReal();*/
		hn.uniRandReal(0.0, 1.0);


	}

}

RBM::RBM(size_t n_visible, size_t n_hidden, NTYPE v_type, NTYPE h_type, LTYPE l_type, size_t b_size) :
		RBM(n_visible, n_hidden, b_size)
{
	visible_type = v_type;
	hidden_type = h_type;
	learning_type = l_type;
}

RBM::~RBM() {

/*	if (W != nullptr) delete(W);
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
	if (negprods != nullptr) delete(negprods);*/

}

void RBM::up(mic::types::MatrixXfPtr in) { // h given v
	LOG(LTRACE)<<"RBM::up";

	//MATRIX_MEMCPY(v, in);
	// Copy input matrix to "RBM visible units".
//	LOG(LINFO)<<" -- v = (*in.get())";
	v = (*in.get());
//	LOG(LINFO)<<" input v:";
//	LOG(LINFO)<<"\n" << v;

	// Flatten!
	v.resize(batch_size, num_input);
//	LOG(LINFO)<<" resized v:";
//	LOG(LINFO)<<"\n" << v;

	// That was commented earlier.
	//v->flatten();
	// rv->elementwise_function(&_rand);
	// v->elementwise_function_matrix(&_compare, *rv);

	//v->transpose();
	//Matrix<float>::sgemm(*h, *W, *v);
	//v->transpose();
	// Compute hidden units h by multiplication of visible units v and weight matrix W.
//	LOG(LINFO)<<" -- h = W * (v.transpose())";
//	LOG(LINFO)<<" h.rows" << h.rows() << " h.cols" << h.cols() ;
//	LOG(LINFO)<<" W.rows" << W.rows() << " W.cols" << W.cols() ;
//	LOG(LINFO)<<" v.rows" << v.rows() << " v.cols" << v.cols() ;
//	LOG(LINFO)<<" v'.rows" << v.transpose().rows() << " v'.cols" << v.transpose().cols() ;
//	mic::types::matrixd_t aa = W * (v.transpose());
//	LOG(LINFO)<<" aa.rows" << aa.rows() << " aa.cols" << aa.cols() ;
	h = W * (v.transpose());


	//h->matrix_row_vector_function(&_add, *c);
	// Add c to h?
//	LOG(LINFO)<<" -- h += c";
	for (size_t x=0; x < h.cols(); x++)
		for (size_t y=0; y < h.rows(); y++){
			// Add c??
			h(y,x) += c(x);
		}//: for

	// h->elementwise_function(&_sigmoid);
	// Sigmoid
//	LOG(LINFO)<<" -- Sigmoid";
	/*for (size_t x=0; x < h.cols(); x++)
		for (size_t y=0; y < h.rows(); y++)
			h(y,x) = (double)1.0 / ((double)1.0 + expf(-h(y,x)));*/
	h.elementwiseFunction(&_sigmoid);

	// Matrix<float>::sgemm(*posprods, *h, *v);
	// Multiply hidden units by visible - why?
//	LOG(LINFO)<<" -- posprods = h * v";
	posprods = h * v;

}

void RBM::down() { // v given h
	LOG(LTRACE)<<"RBM::down";

	//memcpy(H->data, h->data, sizeof(float) * h->elements);
	// rh->elementwise_function(&_rand);
	//H->elementwise_function_matrix(&_compare, *rh);
	// T _compare(T x, T y) { return (T)(x > y); }
	/*for (size_t x=0; x < H.cols(); x++)
		for (size_t y=0; y < H.rows(); y++)
			H(y,x) = h(y,x) > RAN_GEN->uniRandReal();*/
	H = h;
	rh.uniRandReal(0.0, 1.0);
	H.elementwiseFunctionMatrix(&_compare, rh);



	/*H->transpose();
	Matrix<float>::sgemm(*vn, *H, *W);
	H->transpose();*/
	vn = (H.transpose()) * W;

	/*vn->matrix_column_vector_function(&_add, *b);
	vn->elementwise_function(&_sigmoid);*/
	for (size_t x=0; x < vn.cols(); x++)
		for (size_t y=0; y < vn.rows(); y++){
			// Add bias?
			vn(y,x) += b(x);
			// Sigmoid
			//vn(y,x) = (double)1.0 / ((double)1.0 + expf(-vn(y,x)));
		}
	//vn.matrixColumnVectorFunction(&_add, b);
	vn.elementwiseFunction(&_sigmoid);


	if (learning_type == LTYPE::CD) {

		//vn->transpose();
		//Matrix<float>::sgemm(*hn, *W, *vn);
		//vn->transpose();
		hn = W * (vn.transpose());


		//hn->matrix_row_vector_function(&_add, *c);
		//hn->elementwise_function(&_sigmoid);
		for (size_t x=0; x < hn.cols(); x++)
			for (size_t y=0; y < hn.rows(); y++){
				// Add c??
				hn(y,x) += c(x);
				// Sigmoid
				//hn(y,x) = (double)1.0 / ((double)1.0 + expf(-hn(y,x)));
			}//: for
		//hn.matrixRowVectorFunction(&_add, c);
		hn.elementwiseFunction(&_sigmoid);

		//Matrix<float>::sgemm(*negprods, *hn, *vn);
		negprods = hn * vn;

		// MATRIX_MEMCPY(pc, vn);
		pc = vn;

	} else {
		// TO JUZ BYLO przed if!!
		//memcpy(H->data, hn->data, sizeof(float) * h->elements);
		//rh->elementwise_function(&_rand);
		//H->elementwise_function_matrix(&_compare, *rh);


		//H->transpose();
		//Matrix<float>::sgemm(*pc, *H, *W);
		//H->transpose();
		pc = (H.transpose()) * W;


		//pc->matrix_column_vector_function(&_add, *b);
		//pc->elementwise_function(&_sigmoid);
		for (size_t x=0; x < pc.cols(); x++)
			for (size_t y=0; y < pc.rows(); y++){
				// Add bias?
				pc(y,x) += b(x);
				// Sigmoid
				//pc(y,x) = (double)1.0 / ((double)1.0 + expf(-vn(y,x)));
			}
		//pc.matrixColumnVectorFunction(&_add, b);
		pc.elementwiseFunction(&_sigmoid);


		//pc->transpose();
		//Matrix<float>::sgemm(*hn, *W, *pc);
		//pc->transpose();
		hn = W * (pc.transpose());


		//hn->matrix_row_vector_function(&_add, *c);
		//hn->elementwise_function(&_sigmoid);
		for (size_t x=0; x < hn.cols(); x++)
			for (size_t y=0; y < hn.rows(); y++){
				// Add c??
				hn(y,x) += c(x);
				// Sigmoid
				//hn(y,x) = (double)1.0 / ((double)1.0 + expf(-hn(y,x)));
			}//: for
		//hn.matrixRowVectorFunction(&_add, c);
		hn.elementwiseFunction(&_sigmoid);

		//Matrix<float>::sgemm(*negprods, *hn, *pc);
		negprods = hn * pc;
	}

}

void RBM::compute_statistics() {
	LOG(LTRACE)<<"RBM::compute_statistics";

	//MATRIX_MEMCPY(ve, vn);
	//ve = vn;
	//ve->elementwise_function_matrix(&_sub, *v);
	// Bylo zakomentowane
	// ve->elementwise_function(&_square);
	// err = ve->sum();

	// Compute error as difference between vn and v.
	/*for (size_t x=0; x < ve.cols(); x++)
		for (size_t y=0; y < ve.rows(); y++)
			ve(y,x) = vn(y,x) - v(y,x);*/
	ve = vn - v;
	err = ve.norm();
	err *= err / batch_size;


	// Compute sum and normalize it by batch size and number of outputs.
	hsum = h.sum() / (batch_size * num_output);

// Commented as not used later!
/*	MATRIX_MEMCPY(H2, h);
	H2->elementwise_function(&_H2);
	I = H2->sum() / (batch_size * num_output);*/

}

void RBM::adapt(float alpha, float decay, float sparsecost, float sparsetarget, float sparsedamping) {
	LOG(LTRACE)<<"RBM::adapt";

	// reset
	/*W_delta->elementwise_function(&_zero);
	b_delta->elementwise_function(&_zero);
	c_delta->elementwise_function(&_zero);*/
	// Reset deltas.
	W_delta.setZero();
	b_delta.setZero();
	c_delta.setZero();

	// *************
	// hidmeans = sparsedamping*hidmeans + (1-sparsedamping)*poshidact/numcases;
	//hidmeans->elementwise_function_scalar(&_mult, sparsedamping);
	hidmeans *= (double)sparsedamping;

	//hidmeans_inc->sum_rows(*h);
	hidmeans_inc = h.rowwise().sum();

	//hidmeans_inc->elementwise_function_scalar(&_mult, (1.0f - sparsedamping) / batch_size);
	hidmeans_inc *= (double)((1.0f - sparsedamping) / batch_size);

	//hidmeans->elementwise_function_matrix(&_add, *hidmeans_inc);
	hidmeans += hidmeans_inc;

	// *************
	// sparsegrads = sparsecost*(repmat(hidmeans,numcases,1)-sparsetarget);
	//MATRIX_MEMCPY(hidmeans_inc, hidmeans);
	hidmeans_inc = hidmeans;


	// hidmeans_inc->elementwise_function_scalar(&_sub, sparsetarget);
	// hidmeans_inc -= (double)sparsetarget; !!!!! EIGEN PROBLEM!
	for (size_t x=0; x < hidmeans_inc.cols(); x++)
		for (size_t y=0; y < hidmeans_inc.rows(); y++)
			hidmeans_inc(y,x) -= (double)sparsetarget;

	//hidmeans_inc->elementwise_function_scalar(&_mult, sparsecost);
	hidmeans_inc *= (double)sparsecost;

	//Matrix<float>::repmat(*hidmeans_inc_rep, *hidmeans_inc, batch_size);
	for (size_t y = 0; y < hidmeans_inc_rep.cols(); y++) {  // cols = batchsize
		hidmeans_inc_rep.col(y) = hidmeans_inc.col(0);
	}

	// Matrix<float>::sgemm(*sparsegrads, *hidmeans_inc_rep, *v);
	sparsegrads = hidmeans_inc_rep * v;

	//hidmeans_inc->sum_rows(*sparsegrads);
	hidmeans_inc = sparsegrads.rowwise().sum();

	// *************
	// compute
	//MATRIX_MEMCPY(W_delta, posprods);
	//W_delta->elementwise_function_matrix(&_sub, *negprods);
	//W_delta->elementwise_function_matrix(&_sub, *sparsegrads);
	W_delta = posprods - negprods - sparsegrads;

	//W_delta->elementwise_function_scalar(&_mult, alpha / batch_size);
	W_delta *= (alpha / batch_size);

	//b_delta->diff_cols(*v, *pc);
	b_delta = (v-pc).colwise().sum();

	// b_delta->elementwise_function_scalar(&_mult, alpha / batch_size);
	b_delta *= (alpha / batch_size);

	//c_delta->diff_rows(*h, *hn);
	c_delta = (h-hn).rowwise().sum();

	//c_delta->elementwise_function_matrix(&_sub, *hidmeans_inc);
	c_delta -= hidmeans_inc;

	//c_delta->elementwise_function_scalar(&_mult, alpha / batch_size);
	c_delta *= (alpha / batch_size);

	// *************
	// apply
	//W->elementwise_function_matrix(&_add, *W_delta);
	W += W_delta;
	//W->elementwise_function_scalar(&_mult, 1.0f - decay);
	W *= (1.0f - decay);

	//b->elementwise_function_matrix(&_add, *b_delta);
	//c->elementwise_function_matrix(&_add, *c_delta);
	b += b_delta;
	c += c_delta;
}


void RBM::serialize(std::ostream& os) const {

	os << "[" << num_input << " x " << num_output << " Encoder]" << std::endl;
	os << W << std::endl;
}



} /* namespace rbm */
} /* namespace mic */


