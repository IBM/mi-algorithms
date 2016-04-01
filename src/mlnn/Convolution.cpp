/*!
 * \file Convolution.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/Convolution.hpp>

namespace mic {
namespace mlnn {

void Convolution::forward(bool apply_dropout) {

	//pad(x, x_padded, kernel_size, input_channels);
	convolution_forward_gemm(input_channels, y, W, x, b);

}

void Convolution::backward() {

	dx.setZero();
	//dW
	convolution_backward_gemm(input_channels, dy, dW, x, db);
	//dx
	convolution_backward_full_gemm(input_channels, dy, W, dx);
}

void Convolution::save_to_files(std::string prefix) {

	save_matrix_to_file(W, prefix + "_W.txt");
	save_matrix_to_file(x, prefix + "_x.txt");
	save_matrix_to_file(y, prefix + "_y.txt");
	save_matrix_to_file(dx, prefix + "_dx.txt");
	save_matrix_to_file(dy, prefix + "_dy.txt");

}

Convolution::Convolution(size_t inputs, size_t channels, size_t filter_size, size_t filters, size_t batch_size) :
	Layer(inputs * channels, filters * (sqrt(inputs) - filter_size + 1) * (sqrt(inputs) - filter_size + 1), batch_size, "conv"),
	input_channels(channels), output_channels(filters), kernel_size(filter_size),
	output_map_size((sqrt(inputs) - filter_size + 1) * (sqrt(inputs) - filter_size + 1)) {


	W = mic::types::MatrixXf(filters, filter_size * filter_size * input_channels);
	b = (Eigen::VectorXf)Eigen::VectorXf::Zero(filters);

	//W << 0.1, 0, 0, 0, 0, 0, 0, 0, 0;
	size_t fan_in = channels * filter_size * filter_size;
	size_t fan_out = filters * filter_size * filter_size;
	double range = sqrt(6.0 / double(fan_in + fan_out));

	rand(W, -range, range);

	mW = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(W.rows(), W.cols());
	mb = mic::types::VectorXf::Zero(b.rows());

};

void Convolution::resetGrads() {

	dW = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(W.rows(), W.cols());
	db = mic::types::VectorXf::Zero(b.rows());;

}

void Convolution::applyGrads(double alpha, double decay) {

	//adagrad
	mW += dW.cwiseProduct(dW);
	mb += db.cwiseProduct(db);

	W = (1 - decay) * W + alpha * dW.cwiseQuotient(mW.unaryExpr(std::ptr_fun(sqrt_eps)));
	b += alpha * db.cwiseQuotient(mb.unaryExpr(std::ptr_fun(sqrt_eps)));

}

} /* namespace mlnn */
} /* namespace mic */
