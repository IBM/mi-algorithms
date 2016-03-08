/*!
 * \file MatrixXf.hpp
 * \brief 
 * \author tkornut
 * \date Feb 17, 2016
 */

#if 0
//#ifndef SRC_TYPES_MATRIXXF_HPP_
#define SRC_TYPES_MATRIXXF_HPP_

//#ifdef FALSE
#include <Eigen/Dense>

namespace mic {
namespace types {

class MatrixXf;

/*!
 * \brief Vector of floats with single precision (of dynamic size).
 * \author tkornuta
 */
typedef Eigen::VectorXf VectorXf;


/*!
 * \brief Shared pointer to vector of floats with single precision (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< Eigen::VectorXf >VectorXfPtr;



/*!
 * \brief Matrix with single precision floats (of dynamic size).
 * Uses OpenBLAS if found by CMAKE.
 *
 * \date Feb 17, 2016
 * \author tkornuta
 */
class MatrixXf : public Eigen::MatrixXf {
public:

	/*!
	 * Constructor. Calls default Eigen::MatrixXf constructor.
	 */
	MatrixXf () : Eigen::MatrixXf() { };


	/*!
	 * Constructor. Calls default Eigen::MatrixXf constructor.
	 * @param Rows_ Number of rows.
	 * @param Cols_ Number of columns.
	 */
	MatrixXf (int Rows_, int Cols_) : Eigen::MatrixXf(Rows_, Cols_) { };

	/*!
	 * Overloaded operator - calls base operator.
	 */
    EIGEN_STRONG_INLINE Eigen::MatrixXf& operator =(const Eigen::MatrixXf& mat_)
    {
    	// Using base EIGEN operator =
    	return Base::operator=(mat_);
	}

    /*!
     * Overloaded matrix multiplication operator. Uses OpenBLAS if found by CMAKE.
     */
    Eigen::MatrixXf operator *(const Eigen::MatrixXf& mat_);

    /*!
     * Set values of all matrix elements to random with a normal distribution.
     * @param mean Mean
     * @param stddev Variance
     */
    void normRandReal(float mean = 0, float stddev = 1);

	/*!
	 * Set values of all matrix elements to random real numbers from range <min, max> - uniform distribution.
	 * @param min Min value.
	 * @param max Max value.
	 * @return Random real value.
	 */
	void uniRandReal(float min = 0, float max = 1);

    /*!
     * Applies elementwise function to all matrix elements.
     * @param func Function to be applied.
     */
	void elementwiseFunction(float (*func)(float));

	/*!
     * Applies elementwise function to all matrix elements passing scalar as function argument.
     * @param func Function to be applied.
	 * @param scalar_ Scalar passed to function as argument.
	 */
	void elementwiseFunctionScalar(float (*func)(float, float), float scalar_);

    /*!
     * Applies elementwise function to all matrix elements and uses additional Matrix mat_ data as function parameter.
     * @param func Function to be applied.
     * @param mat_ Matrix passed to function as argument.
     */
	void elementwiseFunctionMatrix(float (*func)(float, float), Eigen::MatrixXf & mat_);

	/*!
     * Applies function to all matrix elements and uses additional vector data as function parameter - columnwise.
	 * @param func Used function
	 * @param v_ Vector passed to function
	 */
	void matrixColumnVectorFunction( float(*func)(float, float), mic::types::VectorXf& v_);

	/*!
     * Applies function to all matrix elements and uses additional vector data as function parameter - rowwise.
	 * @param func Used function
	 * @param v_ Vector passed to function
	 */
	void matrixRowVectorFunction( float(*func)(float, float), mic::types::VectorXf& v_);

    /*!
     * Sets the consecutive columns to be equal to given vector.
     * @param in Input vector, that will be "cloned".
     */
	void repeatVector(VectorXf &in);

};


/*!
 * \brief Shared pointer to matrix with single precision floats (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< mic::types::MatrixXf > MatrixXfPtr;


/*!
 * \brief The <single precision float matrix-char> pair type.
 * \author tkornuta
 */
typedef std::pair< MatrixXfPtr, std::shared_ptr<char> > MatrixXfCharPair;

/*!
 * \brief The <single precision float matrix-unsigned int> pair.
 * \author tkornuta
 */
typedef std::pair< MatrixXfPtr, std::shared_ptr<unsigned int> > MatrixXfUintPair;


} /* namespace types */
} /* namespace mic */

//#endif

#endif /* SRC_TYPES_MATRIXXF_HPP_ */
