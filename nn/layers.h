/*
* @Author: kmrocki
* @Date:   2016-02-24 15:26:54
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-03-11 10:17:28
*/

#ifndef __LAYERS_H__
#define __LAYERS_H__

#include <utils.h>
#include <io.h>
#include <string>

//abstract
class Layer {

	public:

		const std::string name;

		//used in forward pass
		Matrix x; //inputs
		Matrix y; //outputs

		//grads, used in backward pass
		Matrix dx;
		Matrix dy;

		const float dropout;
		Matrix dropout_mask;

		Layer(size_t inputs, size_t outputs, size_t batch_size, std::string _label = "layer", float _dropout = 1.0f) : name(_label), dropout(_dropout) {

			x = Matrix(inputs, batch_size);
			y = Matrix(outputs, batch_size);
			dx = Matrix(inputs, batch_size);
			dy = Matrix(outputs, batch_size);

			//for dropout implementation
			dropout_mask = Matrix::Ones(y.rows(), y.cols());

		};

		// Dropout paper - http://arxiv.org/pdf/1207.0580.pdf
		void applyDropout() {

			Matrix rands = Matrix::Zero(y.rows(), y.cols());
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

			cache = Matrix::Zero(x.rows(), x.cols());

		};

		~Pooling() {};

		Matrix cache;

		const size_t channels;
		const size_t window_size;
};

class Convolution : public Layer {

	public:

		Matrix W;
		Vector b;

		Matrix dW;
		Matrix db;

		Matrix mW;
		Matrix mb;

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
			input_channels(channels), kernel_size(filter_size), output_channels(filters),
			output_map_size((sqrt(inputs) - filter_size + 1) * (sqrt(inputs) - filter_size + 1)) {


			W = Matrix(filters, filter_size * filter_size * input_channels);
			b = Vector::Zero(filters);

			//W << 0.1, 0, 0, 0, 0, 0, 0, 0, 0;
			size_t fan_in = channels * filter_size * filter_size;
			size_t fan_out = filters * filter_size * filter_size;
			double range = sqrt(6.0 / double(fan_in + fan_out));

			rand(W, -range, range);

			mW = Matrix::Zero(W.rows(), W.cols());
			mb = Vector::Zero(b.rows());

		};

		void resetGrads() {

			dW = Matrix::Zero(W.rows(), W.cols());
			db = Vector::Zero(b.rows());;

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

		Matrix W;
		Vector b;

		Matrix dW;
		Matrix db;

		Matrix mW;
		Matrix mb;

		void forward(bool apply_dropout = false) {

			y = b.replicate(1, x.cols());
			BLAS_mmul(y, W, x);

			if (apply_dropout)
				applyDropout();
		}

		void backward() {

			dW.setZero();
			BLAS_mmul(dW, dy, x, false, true);
			//dW = dy * x.transpose();
			db = dy.rowwise().sum();
			dx.setZero();
			BLAS_mmul(dx, W, dy, true, false);
			//dx = W.transpose() * dy;

		}

		Linear(size_t inputs, size_t outputs, size_t batch_size, float _dropout = 1.0f) :
			Layer(inputs, outputs, batch_size, "fc", _dropout) {

			W = Matrix(outputs, inputs);
			b = Vector::Zero(outputs);
			double range = sqrt(6.0 / double(inputs + outputs));

			rand(W, -range, range);

			mW = Matrix::Zero(W.rows(), W.cols());
			mb = Vector::Zero(b.rows());

		};

		void resetGrads() {

			dW = Matrix::Zero(W.rows(), W.cols());
			db = Vector::Zero(b.rows());
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
