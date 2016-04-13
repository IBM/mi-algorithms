/*!
 * \file Linear.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/Linear.hpp>

namespace mic {
namespace mlnn {

Linear::Linear(size_t inputs, size_t outputs, size_t batch_size) :
	Layer(inputs, outputs, batch_size, "fc") {

	W = mic::types::MatrixXf(outputs, inputs);
	b = (Eigen::VectorXf)Eigen::VectorXf::Zero(outputs);
	double range = sqrt(6.0 / double(inputs + outputs));

	rand(W, -range, range);

	mW = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(W.rows(), W.cols());
	mb = mic::types::VectorXf::Zero(b.rows());

};

void Linear::forward(bool test) {

	y = W * x + b.replicate(1, x.cols());

}

void Linear::backward() {

	dW = dy * x.transpose();
	db = dy.rowwise().sum();
	dx = W.transpose() * dy;

}

void Linear::resetGrads() {

	dW = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(W.rows(), W.cols());
	db = mic::types::VectorXf::Zero(b.rows());
}

void Linear::applyGrads(double alpha, double decay) {

	//adagrad
	mW += dW.cwiseProduct(dW);
	mb += db.cwiseProduct(db);

	W = (1 - decay) * W + alpha * dW.cwiseQuotient(mW.unaryExpr(std::ptr_fun(sqrt_eps)));
	b += alpha * db.cwiseQuotient(mb.unaryExpr(std::ptr_fun(sqrt_eps)));

	// 'plain' fixed learning rate update
	// b += alpha * db;
	// W += alpha * dW;

}

} /* namespace mlnn */
} /* namespace mic */
