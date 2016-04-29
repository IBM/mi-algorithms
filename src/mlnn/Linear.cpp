/*!
 * \file Linear.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/Linear.hpp>

namespace mic {
namespace mlnn {

Linear::Linear(size_t inputs_, size_t outputs_, size_t batch_size_, std::string name_) :
	Layer(inputs_, outputs_, batch_size_, LayerTypes::Linear, name_) {

	W = mic::types::MatrixXf(outputs_, inputs_);
	b = (Eigen::VectorXf)Eigen::VectorXf::Zero(outputs_);
	double range = sqrt(6.0 / double(inputs_ + outputs_));

	W.rand(-range, range);

	mW = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(W.rows(), W.cols());
	mb = mic::types::VectorXf::Zero(b.rows());

};

void Linear::forward(bool test_) {
	// y = W * x + b.replicate(1, x.cols());
	(*s['y']) = W * (*s['x']) + b.replicate(1, (*s['x']).cols());

}

void Linear::backward() {

	/*dW = dy * x.transpose();
	db = dy.rowwise().sum();
	dx = W.transpose() * dy;*/

	dW = (*g['y']) * ((*s['x']).transpose());
	db = (*g['y']).rowwise().sum();
	(*g['x']) = W.transpose() * (*g['y']);

}

void Linear::resetGrads() {

	dW = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(W.rows(), W.cols());
	db = mic::types::VectorXf::Zero(b.rows());
}

void Linear::applyGrads(double alpha_, double decay_) {

	//adagrad
	mW += dW.cwiseProduct(dW);
	mb += db.cwiseProduct(db);

	W = (1 - decay_) * W + alpha_ * dW.cwiseQuotient(mW.unaryExpr(std::ptr_fun(sqrt_eps)));
	b += alpha_ * db.cwiseQuotient(mb.unaryExpr(std::ptr_fun(sqrt_eps)));

	// 'plain' fixed learning rate update
	// b += alpha * db;
	// W += alpha * dW;

}

} /* namespace mlnn */
} /* namespace mic */
