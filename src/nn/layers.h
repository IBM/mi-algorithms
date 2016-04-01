/*
* @Author: kmrocki
* @Date:   2016-02-24 15:26:54
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-03-11 10:17:28
*/

#ifndef __NN_LAYERS_H__
#define __NN_LAYERS_H__

#include <nn/nn_utils.h>
#include <data_io/matrix_io.h>
#include <string>

#include<types/MatrixTypes.hpp>

//abstract
class Layer {

	public:

		const std::string name;

		//used in forward pass
		mic::types::MatrixXf x; //inputs
		mic::types::MatrixXf y; //outputs

		//grads, used in backward pass
		mic::types::MatrixXf dx;
		mic::types::MatrixXf dy;

		const float dropout;
		mic::types::MatrixXf dropout_mask;

		Layer(size_t inputs, size_t outputs, size_t batch_size, std::string _label = "layer", float _dropout = 1.0f) : name(_label), dropout(_dropout) {

			x = mic::types::MatrixXf(inputs, batch_size);
			y = mic::types::MatrixXf(outputs, batch_size);
			dx = mic::types::MatrixXf(inputs, batch_size);
			dy = mic::types::MatrixXf(outputs, batch_size);

			//for dropout implementation
			dropout_mask = mic::types::MatrixXf::Ones(y.rows(), y.cols());

		};

		// Dropout paper - http://arxiv.org/pdf/1207.0580.pdf
		void applyDropout() {

			mic::types::MatrixXf rands = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(y.rows(), y.cols());
			rand(rands, 0.0f, 1.0f);
			//dropout mask - 1s - preserved elements
			dropout_mask = (rands.array() < dropout).cast <float> ();
			// y = y .* dropout_mask, discard elements where mask is 0
			y.array() = y.array() * dropout_mask.array();
			// normalize, so that we don't have to do anything at test time
			y /= dropout;

		}

		//need to override these
		virtual void forward(bool apply_dropout) = 0;
		virtual void backward() = 0;

		//this is mainly for debugging - TODO: proper serialization of layers and object NN
		virtual void save_to_files(std::string prefix) {};
		virtual void resetGrads() {};
		virtual void applyGrads(double alpha, double decay) {};

		virtual ~Layer() {};

};

class Pooling : public Layer {

	public:

		void forward(bool apply_dropout = false) {


			pooling_forward(channels, x, y, cache, window_size);

		}

		void backward() {

			pooling_backward(channels, dx, dy, cache, window_size);

		}

		//this is mainly for debugging - TODO: proper serialization of layers
		void save_to_files(std::string prefix) {

			save_matrix_to_file(x, prefix + "_x.txt");
			save_matrix_to_file(y, prefix + "_y.txt");
			save_matrix_to_file(cache, prefix + "_cache.txt");
			save_matrix_to_file(dx, prefix + "_dx.txt");
			save_matrix_to_file(dy, prefix + "_dy.txt");

		}

		Pooling(size_t inputs, size_t _window_size, size_t _channels, size_t batch_size) :
			Layer(inputs,
				  _channels * (sqrt(inputs / _channels) / _window_size) * (sqrt(inputs / _channels) / _window_size),
				  batch_size, "pool"),
			channels(_channels), window_size(_window_size) {

			cache = mic::types::MatrixXf::Zero(x.rows(), x.cols());

		};

		~Pooling() {};

		mic::types::MatrixXf cache;

		const size_t channels;
		const size_t window_size;
};

class Convolution : public Layer {

	public:

		mic::types::MatrixXf W;
		mic::types::VectorXf b;

		mic::types::MatrixXf dW;
		mic::types::MatrixXf db;

		mic::types::MatrixXf mW;
		mic::types::MatrixXf mb;

		const size_t input_channels;
		const size_t output_channels;
		const size_t kernel_size;
		const size_t output_map_size;

		void forward(bool apply_dropout = false) {

			//pad(x, x_padded, kernel_size, input_channels);
			convolution_forward_gemm(input_channels, y, W, x, b);

		}

		void backward() {

			dx.setZero();
			//dW
			convolution_backward_gemm(input_channels, dy, dW, x, db);
			//dx
			convolution_backward_full_gemm(input_channels, dy, W, dx);
		}

		//this is mainly for debugging - TODO: proper serialization of layers
		void save_to_files(std::string prefix) {

			save_matrix_to_file(W, prefix + "_W.txt");
			save_matrix_to_file(x, prefix + "_x.txt");
			save_matrix_to_file(y, prefix + "_y.txt");
			save_matrix_to_file(dx, prefix + "_dx.txt");
			save_matrix_to_file(dy, prefix + "_dy.txt");

		}

		Convolution(size_t inputs, size_t channels, size_t filter_size, size_t filters, size_t batch_size) :
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

		void resetGrads() {

			dW = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(W.rows(), W.cols());
			db = mic::types::VectorXf::Zero(b.rows());;

		}

		void applyGrads(double alpha, double decay = 0) {

			//adagrad
			mW += dW.cwiseProduct(dW);
			mb += db.cwiseProduct(db);

			W = (1 - decay) * W + alpha * dW.cwiseQuotient(mW.unaryExpr(std::ptr_fun(sqrt_eps)));
			b += alpha * db.cwiseQuotient(mb.unaryExpr(std::ptr_fun(sqrt_eps)));

		}

		~Convolution() {};

};

class Linear : public Layer {

	public:

		mic::types::MatrixXf W;
		mic::types::VectorXf b;

		mic::types::MatrixXf dW;
		mic::types::MatrixXf db;

		mic::types::MatrixXf mW;
		mic::types::MatrixXf mb;

		void forward(bool apply_dropout = false) {

			y = W * x + b.replicate(1, x.cols());

			if (apply_dropout)
				applyDropout();
		}

		void backward() {

			dW = dy * x.transpose();
			db = dy.rowwise().sum();
			dx = W.transpose() * dy;

		}

		Linear(size_t inputs, size_t outputs, size_t batch_size, float _dropout = 1.0f) :
			Layer(inputs, outputs, batch_size, "fc", _dropout) {

			W = mic::types::MatrixXf(outputs, inputs);
			b = (Eigen::VectorXf)Eigen::VectorXf::Zero(outputs);
			double range = sqrt(6.0 / double(inputs + outputs));

			rand(W, -range, range);

			mW = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(W.rows(), W.cols());
			mb = mic::types::VectorXf::Zero(b.rows());

		};

		void resetGrads() {

			dW = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(W.rows(), W.cols());
			db = mic::types::VectorXf::Zero(b.rows());
		}

		void applyGrads(double alpha, double decay = 0) {

			//adagrad
			mW += dW.cwiseProduct(dW);
			mb += db.cwiseProduct(db);

			W = (1 - decay) * W + alpha * dW.cwiseQuotient(mW.unaryExpr(std::ptr_fun(sqrt_eps)));
			b += alpha * db.cwiseQuotient(mb.unaryExpr(std::ptr_fun(sqrt_eps)));

			// 'plain' fixed learning rate update
			// b += alpha * db;
			// W += alpha * dW;

		}

		~Linear() {};

};

class Sigmoid : public Layer {

	public:

		void forward(bool apply_dropout = false) {

			y = logistic(x);

		}

		void backward() {

			dx.array() = dy.array() * y.array() * (1.0 - y.array()).array();

		}

		Sigmoid(size_t inputs, size_t outputs, size_t batch_size) :
			Layer(inputs, outputs, batch_size, "sigmoid") {};
		~Sigmoid() {};

};

class Identity : public Layer {

	public:

		void forward(bool apply_dropout = false) {

			y = x;

		}

		void backward() {

			dx = dy;

		}

		Identity(size_t inputs, size_t outputs, size_t batch_size) :
			Layer(inputs, outputs, batch_size, "noop") {};
		~Identity() {};

};

class ReLU : public Layer {

	public:

		void forward(bool apply_dropout = false) {

			y = rectify(x);

		}

		void save_to_files(std::string prefix) {

			save_matrix_to_file(y, prefix + "_y.txt");

		}

		void backward() {

			dx.array() = derivative_ReLU(y).array() * dy.array();

		}

		ReLU(size_t inputs, size_t outputs, size_t batch_size) :
			Layer(inputs, outputs, batch_size, "relu") {};
		~ReLU() {};

};

// Exponential Linear Unit
// http://arxiv.org/pdf/1511.07289v5.pdf

class ELU : public Layer {

	public:

		void forward(bool apply_dropout = false) {

			y = activation_ELU(x);

		}

		void backward() {

			dx.array() = derivative_ELU(y).array() * dy.array();

		}

		ELU(size_t inputs, size_t outputs, size_t batch_size) :
			Layer(inputs, outputs, batch_size, "elu") {};
		~ELU() {};

};

class Softmax : public Layer {

	public:

		void forward(bool apply_dropout = false) {

			y = softmax(x);

		}

		void backward() {

			dx = dy - y;
		}


		Softmax(size_t inputs, size_t outputs, size_t batch_size) :
			Layer(inputs, outputs, batch_size, "softmax") {};
		~Softmax() {};

};

#endif
