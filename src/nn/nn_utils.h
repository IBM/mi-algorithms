/*
* @Author: kmrocki
* @Date:   2016-02-24 10:47:03
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-03-10 12:43:26
*/

#ifndef __NN_UTILS_H__
#define __NN_UTILS_H__

#include <iostream>
#include <iomanip>
#include <random>

//TODO: integrate with MI Matrixf/Vector
//setMatrix  implementation
/*#include <Eigen/Dense>
typedef Eigen::VectorXf Vector;
typedef Eigen::MatrixXf Matrix;*/
#include <types/MatrixTypes.hpp>

inline float sqrt_eps(const float x) {
	return sqrtf(x + 1e-6);
}

//f(x) = sigm(x)
inline float __logistic(const float x) {
	return 1.0f / (1.0f +::expf(-x));
}

inline float __exponential(const float x) {
	return expf(x);
}

mic::types::MatrixXf rectify(mic::types::MatrixXf& x) {

	mic::types::MatrixXf y(x.rows(), x.cols());

	for (int i = 0; i < x.rows(); i++) {
		for (int j = 0; j < x.cols(); j++) {

			y(i, j) = fmaxf(x(i, j), 0.0f);
		}
	}

	return y;

}

// Exponential Linear Unit
// http://arxiv.org/pdf/1511.07289v5.pdf
mic::types::MatrixXf activation_ELU(mic::types::MatrixXf& x) {

	mic::types::MatrixXf y(x.rows(), x.cols());

	for (int i = 0; i < x.rows(); i++) {
		for (int j = 0; j < x.cols(); j++) {

			y(i, j) = x(i, j) > 0.0f ? x(i, j) : (expf(x(i, j)) - 1.0f);

		}
	}

	return y;

}

mic::types::MatrixXf derivative_ELU(mic::types::MatrixXf& x) {

	mic::types::MatrixXf y(x.rows(), x.cols());

	for (int i = 0; i < x.rows(); i++) {
		for (int j = 0; j < x.cols(); j++) {

			y(i, j) = x(i, j) > 0.0f ? 1.0f : expf(x(i, j));
		}
	}

	return y;

}

mic::types::MatrixXf derivative_ReLU(mic::types::MatrixXf& x) {

	mic::types::MatrixXf y(x.rows(), x.cols());

	for (int i = 0; i < x.rows(); i++) {
		for (int j = 0; j < x.cols(); j++) {

			y(i, j) = (float)(x(i, j) > 0);
		}
	}

	return y;

}

mic::types::MatrixXf logistic(mic::types::MatrixXf& x) {

	mic::types::MatrixXf y(x.rows(), x.cols());

	for (int i = 0; i < x.rows(); i++) {
		for (int j = 0; j < x.cols(); j++) {

			y(i, j) = __logistic(x(i, j));
		}
	}

	return y;
}

mic::types::MatrixXf softmax(mic::types::MatrixXf& x) {

	mic::types::MatrixXf y(x.rows(), x.cols());

	//probs(class) = exp(x, class)/sum(exp(x, class))

	mic::types::MatrixXf e = (Eigen::MatrixXf)(x.unaryExpr(std::ptr_fun(::expf)));

	mic::types::VectorXf sum = e.colwise().sum();

	for (int i = 0; i < e.rows(); i++) {
		for (int j = 0; j < e.cols(); j++) {

			y(i, j) = e(i, j) / sum(j);
		}
	}

	return y;
}

float cross_entropy(mic::types::MatrixXf& predictions, mic::types::MatrixXf& targets) {

	float ce = 0.0;
	mic::types::MatrixXf error(predictions.rows(), predictions.cols());

	//check what has happened and get information content for that event
	error.array() = -predictions.unaryExpr(std::ptr_fun(::logf)).array() * targets.array();
	ce = error.sum();

	return ce;
}

//generate an array of random numbers in range
void randi(mic::types::VectorXi& m, int range_min, int range_max) {

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> dis(range_min, range_max);

	for (int i = 0; i < m.rows(); i++) {
		m(i) = (float)dis(mt);
	}

}

void __unpool_disjoint_2D(mic::types::MatrixXf& dx, mic::types::MatrixXf& cache, mic::types::MatrixXf& dy, size_t kernel_size) {

	for (size_t xi = 0; xi < dx.rows(); xi++) {

		for (size_t xj = 0; xj < dx.cols(); xj++) {

			dx(xj, xi) = cache(xj, xi) * dy(xj / kernel_size, xi / kernel_size);

		}
	}

}

void __pool_disjoint_2D(mic::types::MatrixXf& out, mic::types::MatrixXf& cache, mic::types::MatrixXf& image, size_t kernel_size) {

	for (size_t yi = 0; yi < out.rows(); yi++) {

		for (size_t yj = 0; yj < out.cols(); yj++) {

			float value = -INFINITY;
			size_t idx_j;
			size_t idx_i;

			for (size_t ki = 0; ki < kernel_size; ki++) {

				for (size_t kj = 0; kj < kernel_size; kj++) {

					float pix = image(yj * kernel_size + kj, yi * kernel_size + ki);

					if (value < pix) {

						value = pix;
						idx_j = kj;
						idx_i = ki;

					}

				}
			}

			out(yj, yi) = (float)value;
			cache(yj * kernel_size + idx_j, yi * kernel_size + idx_i) = 1;
		}
	}
}

#define ADDRESS_2D_TO_1D(i, j, cols) ((j) + (i) * (cols))
#define ADDRESS_3D_TO_1D(i, j, k, cols, channel_size) ((i) + (j) * (cols) + (k) * (channel_size))

void pad(mic::types::MatrixXf& x, mic::types::MatrixXf& x_padded, size_t kernel_size, size_t input_channels) {

	size_t padding = kernel_size / 2;
	size_t batch_size = x.cols();
	size_t image_size = sqrt(x.rows() / input_channels);
	size_t padded_size = sqrt(x_padded.rows() / input_channels);

	#pragma omp parallel for
	for (size_t b = 0; b < batch_size; b++) {

		mic::types::MatrixXf im_channel = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(image_size, image_size);
		mic::types::MatrixXf im_padded = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(padded_size, padded_size * input_channels);
		mic::types::MatrixXf image = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(x.rows(), 1);

		image = x.col(b);
		image.resize(image_size, image_size * input_channels);

		for (size_t f = 0; f < input_channels; f++) {

			im_channel = image.block(0, f * image_size, image_size, image_size);
			mic::types::MatrixXf padded_channel = (Eigen::MatrixXf)(Eigen::MatrixXf::Ones(padded_size, padded_size) * 0.5);
			padded_channel.block(padding, padding, image_size, image_size) = im_channel;
			im_padded.block(0, f * padded_size, padded_size, padded_size) = padded_channel;

		}

		im_padded.resize(padded_size * padded_size * input_channels, 1);
		x_padded.col(b) = im_padded;

	}

}

//outer loop over image locations, all images processed in parallel
void convolution_forward_gemm(size_t input_channels, mic::types::MatrixXf& out, mic::types::MatrixXf& W, mic::types::MatrixXf& in, mic::types::VectorXf& b) {

	//W is size [kernel_length x filters]
	//I is size [batch_size x kernel_length]
	//O is [batch_size x filters] = [batch_size x kernel_length] * [kernel_length x filters]

	//total number of operations proportional to
	//out_image_size * out_image_size * batch_size * kernel_length * filters

	size_t kernel_size = sqrt(W.cols() / input_channels);
	size_t channel_length = in.rows() / input_channels;
	size_t kernel_length = kernel_size * kernel_size * input_channels;
	size_t kernel_length_channel = kernel_size * kernel_size;
	size_t image_size = sqrt(in.rows() / input_channels);
	size_t batch_size = in.cols();
	size_t out_image_size = image_size - kernel_size + 1;
	size_t out_image_channel_length = out_image_size * out_image_size;
	size_t filters = W.rows();

	#pragma omp parallel for collapse(2)
	for (size_t x = 0; x < out_image_size; x++) {
		for (size_t y = 0; y < out_image_size; y++) {

			mic::types::MatrixXf O = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(batch_size, filters);
			mic::types::MatrixXf I = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(batch_size, kernel_length);

			//inputs(:, :) = images(:, x, y, :);
			for (size_t k0 = 0; k0 < kernel_size; k0++) {
				for (size_t k1 = 0; k1 < kernel_size; k1++) {

					for (size_t channel = 0; channel < input_channels; channel++) {

						size_t i = x + k0;
						size_t j = y + k1;
						size_t k = channel * kernel_length_channel + k0 * kernel_size + k1;
						I.col(k) = in.row(ADDRESS_3D_TO_1D(i, j, channel, image_size, channel_length));

					}

				}
			}


			O = I * W.transpose();
			O = O + b.transpose().replicate(batch_size, 1);

			for (size_t k = 0; k < filters; k++) {

				out.row(ADDRESS_3D_TO_1D(x, y, k, out_image_size, out_image_channel_length)) = O.col(k);

			}

		} 	// y loop
	}	// x loop

}

void convolution_backward_full_gemm(size_t input_channels, mic::types::MatrixXf& out, mic::types::MatrixXf& W, mic::types::MatrixXf& in) {

	size_t channel_length = in.rows() / input_channels;
	size_t kernel_size = sqrt(W.cols() / input_channels);
	size_t kernel_length_channel = kernel_size * kernel_size;
	size_t image_size = sqrt(in.rows() / input_channels);
	size_t batch_size = in.cols();
	size_t out_image_size = image_size - kernel_size + 1;
	size_t filters = W.rows();

	//pad matrices
	size_t padded_size = image_size + kernel_size - 1;
	mic::types::MatrixXf out_padded = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(padded_size * padded_size * filters, batch_size);

	#pragma omp parallel for shared(out_padded)
	for (size_t b = 0; b < batch_size; b++) {

		mic::types::MatrixXf out_resized = (Eigen::MatrixXf)out.col(b);
		mic::types::MatrixXf padded_temp = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(padded_size, padded_size * filters);
		out_resized.resize(out_image_size, out_image_size * filters);

		for (size_t f = 0; f < filters; f++) {

			mic::types::MatrixXf padded_temp2 = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(padded_size, padded_size);
			mic::types::MatrixXf out_temp2 = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(out_image_size, out_image_size);
			out_temp2 = out_resized.block(0, f * out_image_size, out_image_size, out_image_size);
			padded_temp2.block(kernel_size - 1, kernel_size - 1, out_image_size, out_image_size) = out_temp2;
			padded_temp.block(0, f * padded_size, padded_size, padded_size) = padded_temp2;

		}

		padded_temp.resize(padded_size * padded_size * filters, 1);
		out_padded.col(b) = padded_temp;

	}

	mic::types::MatrixXf W_permuted = mic::types::MatrixXf(kernel_size * kernel_size * filters, input_channels);
	mic::types::MatrixXf temp_W2 = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(1, kernel_size * kernel_size);

	for (size_t c = 0; c < input_channels; c++) {

		for (size_t f = 0; f < filters; f++) {

			mic::types::MatrixXf temp_W2 = (Eigen::MatrixXf)W.block(f, c * kernel_size * kernel_size, 1, kernel_size * kernel_size);
			temp_W2.reverseInPlace();
			W_permuted.block(f * kernel_size * kernel_size, c, kernel_size * kernel_size, 1) = temp_W2.transpose().eval();
		}

	}

	#pragma omp parallel for collapse(2)

	for (size_t x = 0; x < image_size; x++) {
		for (size_t y = 0; y < image_size; y++) {

			mic::types::MatrixXf O = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(batch_size, kernel_size * kernel_size * filters);
			mic::types::MatrixXf I = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(batch_size, input_channels);

			//inputs(:, :) = images(:, x, y, :);
			for (size_t k0 = 0; k0 < kernel_size; k0++) {

				for (size_t k1 = 0; k1 < kernel_size; k1++) {

					for (size_t channel = 0; channel < filters; channel++) {

						size_t i = x + k0;
						size_t j = y + k1;
						size_t k = channel * kernel_length_channel + k0 * kernel_size + k1;
						O.col(k) = out_padded.row(ADDRESS_3D_TO_1D(i, j, channel, padded_size, padded_size * padded_size));

					}

				}
			}

			I = O * W_permuted;

			for (size_t k = 0; k < input_channels; k++) {

				in.row(ADDRESS_3D_TO_1D(x, y, k, image_size, channel_length)) = I.col(k);

			}

		}
	}
}

void convolution_backward_gemm(size_t input_channels, mic::types::MatrixXf& out, mic::types::MatrixXf& W, mic::types::MatrixXf& in, mic::types::MatrixXf& b) {

	//W is size [filters x kernel_length]
	//I is size [batch_size x kernel_length]
	//O is [batch_size x filters] = [batch_size x kernel_length] * [kernel_length x filters]

	//total number of operations proportional to
	//out_image_size * out_image_size * batch_size * kernel_length * filters
	size_t channel_length = in.rows() / input_channels;
	size_t kernel_size = sqrt(W.cols() / input_channels);
	size_t kernel_length = kernel_size * kernel_size * input_channels;
	size_t kernel_length_channel = kernel_size * kernel_size;
	size_t image_size = sqrt(in.rows() / input_channels);
	size_t batch_size = in.cols();
	size_t out_image_size = image_size - kernel_size + 1;
	size_t out_image_channel_length = out_image_size * out_image_size;
	size_t filters = W.rows();

	W.setZero();
	b.setZero();
	#pragma omp parallel for collapse(2)
	for (size_t x = 0; x < out_image_size; x++) {

		for (size_t y = 0; y < out_image_size; y++) {

			mic::types::MatrixXf dW = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(W.rows(), W.cols());
			mic::types::MatrixXf db = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(b.rows(), b.cols());
			mic::types::MatrixXf O = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(batch_size, filters);
			mic::types::MatrixXf I = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(batch_size, kernel_length);

			//inputs(:, : ) = images(:, x, y, : );
			for (size_t k0 = 0; k0 < kernel_size; k0++) {

				for (size_t k1 = 0; k1 < kernel_size; k1++) {

					for (size_t channel = 0; channel < input_channels; channel++) {

						size_t i = x + k0;
						size_t j = y + k1;
						size_t k = channel * kernel_length_channel + k0 * kernel_size + k1;

						I.col(k) = in.row(ADDRESS_3D_TO_1D(i, j, channel, image_size, channel_length));

					}

				}
			}

			for (size_t k = 0; k < filters; k++) {

				O.col(k) = out.row(ADDRESS_3D_TO_1D(x, y, k, out_image_size, out_image_channel_length));
				db(k) = O.col(k).sum() / batch_size;
			}

			dW = (O.transpose() * I);

			//reduction
			#pragma omp critical
			{
				W = W + dW / batch_size;
				b = b + db;
			}

		} 	// y loop
	}	// x loop

}

mic::types::MatrixXf pooling_forward_channel(mic::types::MatrixXf& x, mic::types::MatrixXf& cache, size_t window_size) {

	size_t image_size = sqrt(x.rows());
	// size_t y_width = image_size - window_size + 1;
	//disjoint
	size_t y_width = image_size / window_size;
	mic::types::MatrixXf y = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(y_width * y_width, x.cols());

	for (size_t i = 0; i < y.cols(); i++) { //images in a batch

		mic::types::MatrixXf image = (Eigen::MatrixXf)x.col(i);
		mic::types::MatrixXf local_cache = (Eigen::MatrixXf)cache.col(i);

		image.resize(image_size, image_size);
		local_cache.resize(image_size, image_size);

		mic::types::MatrixXf out = (Eigen::MatrixXf)y.col(i);
		out.resize(sqrt(out.size()), sqrt(out.size()));

		out.setZero();
		local_cache.setZero();

		// __pooling_2D(out, image, window_size);
		__pool_disjoint_2D(out, local_cache, image, window_size);

		out.resize(out.size(), 1);
		local_cache.resize(local_cache.size(), 1);

		y.col(i) = out;
		cache.col(i) = local_cache;
	}

	return y;

}

mic::types::MatrixXf pooling_backward_channel(mic::types::MatrixXf& dy, mic::types::MatrixXf& cache, size_t window_size) {

	mic::types::MatrixXf dx = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(cache.rows(), cache.cols());

	for (size_t i = 0; i < dy.cols(); i++) {

		mic::types::MatrixXf dy_local = (Eigen::MatrixXf)dy.col(i);
		mic::types::MatrixXf cache_local = (Eigen::MatrixXf)cache.col(i);
		mic::types::MatrixXf dx_local = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(cache_local.rows(), cache_local.cols());
		dy_local.resize(sqrt(dy_local.size()), sqrt(dy_local.size()));
		cache_local.resize(sqrt(cache_local.size()), sqrt(cache_local.size()));
		dx_local.resize(sqrt(cache_local.size()), sqrt(cache_local.size()));
		dx_local.setZero();

		__unpool_disjoint_2D(dx_local, cache_local, dy_local, window_size);

		dx_local.resize(dx_local.size(), 1);
		dx.col(i) = dx_local;
	}

	return dx;
}

void pooling_forward(size_t channels, mic::types::MatrixXf& x, mic::types::MatrixXf& y, mic::types::MatrixXf& cache, size_t window_size) {

	#pragma omp parallel for
	for (size_t k = 0; k < channels; k++) {
		mic::types::MatrixXf y_map = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(y.rows() / channels, y.cols());
		mic::types::MatrixXf inputs = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(x.rows() / channels, x.cols());
		mic::types::MatrixXf cache_map = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(x.rows() / channels, x.cols());
		inputs = x.block(inputs.rows() * k, 0, inputs.rows(), inputs.cols());
		y_map.array() = pooling_forward_channel(inputs, cache_map, window_size).array();
		y.block(y_map.rows() * k, 0, y_map.rows(), y_map.cols()) = y_map;
		cache.block(cache_map.rows() * k, 0, cache_map.rows(), cache_map.cols()) = cache_map;

	}
}

void pooling_backward(size_t channels, mic::types::MatrixXf& dx, mic::types::MatrixXf& dy, mic::types::MatrixXf& cache, size_t window_size) {

	#pragma omp parallel for
	for (size_t k = 0; k < channels; k++) {
		mic::types::MatrixXf dy_map = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(dy.rows() / channels, dy.cols());
		mic::types::MatrixXf dx_map = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(dx.rows() / channels, dx.cols());
		mic::types::MatrixXf cache_map = (Eigen::MatrixXf)Eigen::MatrixXf::Zero(dx.rows() / channels, dx.cols());
		dy_map = dy.block(dy_map.rows() * k, 0, dy_map.rows(), dy_map.cols());
		cache_map = cache.block(cache_map.rows() * k, 0, cache_map.rows(), cache_map.cols());
		dx_map = pooling_backward_channel(dy_map, cache_map, window_size).array();
		dx.block(dx_map.rows() * k, 0, dx_map.rows(), dx_map.cols()) = dx_map;
	}
}

void randn(mic::types::MatrixXf& m, float mean, float stddev) {

	std::random_device rd;
	std::mt19937 mt(rd());
	std::normal_distribution<> dis(mean, stddev);

	for (int i = 0; i < m.rows(); i++) {
		for (int j = 0; j < m.cols(); j++) {
			m(i, j) = dis(mt);
		}
	}

}

void rand(mic::types::MatrixXf& m, float range_min, float range_max) {

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> dis(range_min, range_max);

	for (int i = 0; i < m.rows(); i++) {
		for (int j = 0; j < m.cols(); j++) {
			m(i, j) = dis(mt);
		}
	}

}

void linspace(mic::types::VectorXi& m, int range_min, int range_max) {

	for (int i = 0; i < m.rows(); i++) {
		m(i) = (float)(range_min + i);
	}

}

mic::types::MatrixXf make_batch(std::deque<datapoint>& data, mic::types::VectorXi& random_numbers) {

	size_t batch_size = random_numbers.rows();
	mic::types::MatrixXf batch(data[0].x.rows(), batch_size);

	for (size_t i = 0; i < batch_size; i++) {

		batch.col(i) = data[random_numbers(i)].x;

	}

	return batch;
}

mic::types::MatrixXf make_targets(std::deque<datapoint>& data, mic::types::VectorXi& random_numbers, size_t classes) {

	size_t batch_size = random_numbers.rows();
	mic::types::MatrixXf encoding = (Eigen::MatrixXf)Eigen::MatrixXf::Identity(classes, classes);
	mic::types::MatrixXf batch(classes, batch_size);

	for (size_t i = 0; i < batch_size; i++) {

		batch.col(i) = encoding.col(data[random_numbers(i)].y);

	}

	return batch;
}

mic::types::VectorXi colwise_max_index(mic::types::MatrixXf& m) {

	mic::types::VectorXi indices(m.cols());

	for (size_t i = 0; i < m.cols(); i++) {

		float current_max_val;
		int index;

		for (size_t j = 0; j < m.rows(); j++) {

			if (j == 0 || m(j, i) > current_max_val) {

				index = j;
				current_max_val = m(j, i);
			}

			indices(i) = index;

		}
	}

	return indices;
}

size_t count_zeros(mic::types::VectorXi& m) {

	size_t zeros = 0;

	for (int i = 0; i < m.rows(); i++) {

		bool isZero = m(i) == 0;

		zeros += isZero;
	}

	return zeros;

}

size_t count_correct_predictions(mic::types::MatrixXf& p, mic::types::MatrixXf& t) {

	mic::types::VectorXi predicted_classes = colwise_max_index(p);
	mic::types::VectorXi target_classes = colwise_max_index(t);
	mic::types::VectorXi correct = (target_classes - predicted_classes);

	return count_zeros(correct);
}

#endif
