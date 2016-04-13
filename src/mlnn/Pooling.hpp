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

	Pooling(size_t inputs_, size_t window_size_, size_t channels_, size_t batch_size_);

	virtual ~Pooling() {};

	void forward(bool test_ = false);

	void backward();

	//this is mainly for debugging - TODO: proper serialization of layers
	void save_to_files(std::string prefix);

protected:

	mic::types::MatrixXf cache;

	const size_t channels;

	const size_t window_size;
};

} /* namespace mlnn */
} /* namespace mic */

#endif /* SRC_MLNN_POOLING_HPP_ */
