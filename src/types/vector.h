/*
* @File: vector.h
* @Author: kamilrocki
* @Email: kmrocki@us.ibm.com
* @Created:	2015-04-26 13:05:16
* @Last Modified by:   kamilrocki
* @Last Modified time: 2015-05-02 15:09:40
*/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <types/tensor.h>

namespace mic {
namespace types {


/**
 * \brief Vector is a 1D Matrix.
 * \author krocki
 * \tparam T template parameter denoting data type stored in matrix.
 */
template <typename T>
class Vector : public mic::types::Tensor<T> {

	public:

		Vector() = default;

		Vector(const Vector& v) : Tensor<T>(v) { } //copy everything from Matrix

		Vector(size_t N) : Tensor<T>({N}) {

		}

		~Vector() = default;

		/*!
		 * Serializes vector into ostream.
		 * \author tkornuta
		 * @param os
		 */
        virtual void serialize(std::ostream& os) const {

            Tensor<T>::serialize(os);

            os << std::endl;

            for (size_t i = 0; i < Tensor<T>::dim[0]; i++) {

		            if (typeid(T) == typeid(bool)) {
		            	os << Tensor<T>::data[i ];
		            } else {
	            		os << "[" << std::setw( 3 ) << std::setfill( '-' ) << i << "] " <<
	            		std::fixed << std::setw( 6 ) << std::setprecision( 3 ) <<
	            		std::setfill( ' ' ) << Tensor<T>::data[i] << " ";
            		}
            	if (i < Tensor<T>::dim[0] - 1) os << std::endl;
            }
            
        }

        /*!
         * Returns reference to value stored at i-th position.
         * \author tkornuta
         */
		T& operator()(const size_t i) {

		    return Tensor<T>::data[i];
		}

};

}//: namespace types
}//: namespace mic

#endif /*__VECTOR_H__*/
