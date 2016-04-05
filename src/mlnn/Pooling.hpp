/*!
 * \file Pooling.hpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#ifndef SRC_MLNN_POOLING_HPP_
#define SRC_MLNN_POOLING_HPP_

#include <mlnn/Layer.hpp>

namespace mic {
namespace mlnn {

/*!
 * \author krocki
 */
class Pooling : public mic::mlnn::Layer {

	public:

		void forward(bool apply_dropout = false);

		void backward();

		//this is mainly for debugging - TODO: proper serialization of layers
		void save_to_files(std::string prefix);

		Pooling(size_t inputs, size_t _window_size, size_t _channels, size_t batch_size);

		virtual ~Pooling() {};

		mic::types::MatrixXf cache;

		const size_t channels;
		const size_t window_size;
};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_POOLING_HPP_ */
