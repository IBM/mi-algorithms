/*!
 * \file Padding.cpp
 * \brief 
 * \author tkornut
 * \date Apr 12, 2016
 */

#include <mlnn/Padding.hpp>

namespace mic {
namespace mlnn {

Padding::Padding(size_t inputs_, size_t channels_, size_t batch_size_, size_t padding_) :
	Layer(inputs_, channels_ * (sqrt(inputs_ / channels_) + padding_ * 2) * (sqrt(inputs_ / channels_) + padding_ * 2), batch_size_, "padding"), channels(channels_), padding(padding_) {

};


void Padding::forward(bool test) {

	//pad((*s['x']), (*s['y']), channels, padding);

	std::cout<<"ERROR! pad not implemented\n";
	(*s['y']) =  (*s['x']);

}

/*void pad(mic::types::MatrixXf& x, mic::types::MatrixXf& x_padded, size_t kernel_size, size_t input_channels) {

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

}*/

void Padding::backward() {

	//unpad((*g['x']), (*g['y']), channels, padding);

	std::cout<<"ERROR! unpad not implemented\n";
	(*g['x']) =  (*g['y']);
}


} /* namespace mlnn */
} /* namespace mic */
