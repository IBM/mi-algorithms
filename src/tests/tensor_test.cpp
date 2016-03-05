/*!
 * \file tensor_test.cpp
 * \brief Contains program for testing of tensors/Eigen map.
 * \author tkornut
 * \date Feb 17, 2016
 */


#include <iostream>
#include <iomanip>

#include <sys/time.h>

#include <types/MatrixTypes.hpp>

#include <types/tensor.h>

#include <data_utils/functions.h>
#include <data_utils/Timer.hpp>

using namespace Eigen;
using namespace std;


template <class T = float>
class matrix3d {
public:
	matrix3d() = default;

		matrix3d(const matrix3d& t) {

			// Copy dimensions.
			static_assert(t.dim.size() != 3, "Matrix 3D must have exactly three dimensions");
			memcpy(dim, t.dim, sizeof(size_t) * t.dim.size());
			elements = t.elements;

			// Copy data.
			if (data != nullptr) delete(data);
			data = new T[t.elements];
			memcpy(data, t.data, sizeof(T) * elements);

			// ????
			//this = t;
		}

		matrix3d<T>& operator=(matrix3d<T>& t) {
			// Copy dimensions.
			static_assert(t.dim.size() != 3, "Matrix 3D must have exactly three dimensions");
			dim = t.dim;
			elements = t.elements;

			// Copy data.
			memcpy(data, t.data, sizeof(T) * elements);
		}

		matrix3d(std::initializer_list<size_t> args) {

			// Set dimensions.
			//static_assert(args.size() != 3, "Matrix 3D must have exactly three dimensions");
			elements = 1;
			for (auto i: args) {

				dim.push_back(i);
				elements *= i;
			}

			// Allocate memory.
			data = new T[elements];

			if (!data) {

				//couldn't allocate
				std::cout << "could not allocate mem: " << __PRETTY_FUNCTION__ << std::endl;
			}

			else reset();
			// TMP!!!
			for (size_t i = 0; i < elements; i++) {
				data[i] = i;
			}

		}

		~matrix3d() {

			if (data) delete(data);

		}

        virtual void serialize(std::ostream& os) const {

            os << "[";

			for (size_t i = 0; i < dim.size() - 1; i++)
				os << dim[i] << " x ";

			os << dim.back() << "] ";
			os << elements << " elements:\n";

			for (size_t i = 0; i < elements; i++) {
				os << data[i] << ", ";
			}
			os << endl;

			for (size_t m = 0; m < dim[2]; m++) {
				for (size_t r = 0; r < dim[0]; r++) {
					for (size_t c = 0; c < dim[1]; c++) {
						os << data[(c+r*dim[1]) + m*(dim[0]*dim[1])] << ", ";
					}
					os << endl;
				}
				os << endl;
			}
        }

		friend std::ostream& operator<<(std::ostream& os, const matrix3d& t) {

			t.serialize(os);

			return os;

		}

		void reset() {

			memset(data, 0, elements * sizeof(T));

		}


	    /*!
	     * Set values of all matrix elements to random with a normal distribution.
	     * @param mean Mean
	     * @param stddev Variance
	     */
	    void normRandReal(float mean = 0, float stddev = 1){

			// Initialize random number generator with normal distribution.
			std::random_device rd;
			std::mt19937 mt(rd());
			std::normal_distribution<> dist(mean, stddev);

			#pragma omp parallel for
			for (size_t i = 0; i < elements; i++) {
				data[i] = dist(mt);
			}
		}


		Eigen::MatrixXf operator () (size_t n) {
			float *addr = data+ n*(dim[0]*dim[1]);
			// Map
			return Map<Eigen::MatrixXf>(addr,dim[1],dim[0]).transpose();
		}

private:
	/*!
	 * Dimensions.
	 */
	std::vector<size_t> dim;

	/*!
	 * Table of elements of data types.
	 */
	T* data;

	/*!
	 * Number of elements.
	 */
	size_t elements;

};



/*!
 * \brief Program for testing tensors/Eigen map
 * \author tkornuta
 * @param[in] argc Number of parameters (not used).
 * @param[in] argv List of parameters (not used).
 * @return (not used).
 */
int main(int argc, char* argv[]) {

	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 3;
	const size_t K = 4;
	const size_t number_of_multiplications = 100;

	// Matrices.
	mic::types::MatrixXf nm(N, M);
	mic::types::MatrixXf mk(M, K);
	mic::types::MatrixXf nk(N, K);

	// Initialize matrices with random numbers.
	nm.normRandReal(1, 0.001);
	mk.normRandReal(1, 0.001);
	std::cout <<"nm=\n" << nm <<  std::endl;

	matrix3d<float> m3d {N, M, K};

	//m3d.normRandReal();
	std::cout <<"m3d=\n" << m3d <<  std::endl;

	for (size_t i=0; i< K; i++)
		std::cout <<"m3d("<< i << ")=\n" << m3d(i) <<  std::endl;


/*	int data[] = {1,2,3,4,5,6,7,8,9};
	Map<RowVectorXi> v(data,4);
	cout << "The mapped vector v is: " << v << "\n";
	v(2) = 100;
	cout << "The mapped vector v is: " << v << "\n";

	new (&v) Map<RowVectorXi>(data+4,5);
	cout << "Now v is: " << v << "\n";*/

	// Adding test.
/*	Eigen::VectorXf vect(N);
	vect.setRandom();
	std::cout <<"vect=\n" << vect <<  std::endl;
	nm.colwise() += vect;
	std::cout <<"nm+=vect\n" << nm <<  std::endl;

	// Elementwise function test.
	mic::types::MatrixXf nm2(N, M);
	nm2.normRandReal(1, 0.00001);
	std::cout <<"nm2=\n" << nm2 <<  std::endl;
	nm.elementwiseFunctionMatrix(&_add, nm2);
	std::cout <<"nm+=nm2\n" << nm <<  std::endl;


	std::cout <<"mk=\n" << mk <<  std::endl;

	mic::data_utils::Timer timer;
	timer.start();

	for (size_t i = 0; i < number_of_multiplications; i++) {

		// Multiplication.
		nk = nm * mk;

	}//: for
	double time = timer.end();
	std::cout <<"nk=\n" << nk <<  std::endl;

	std::cout  <<  "Multiplication time = " << time <<  std::endl;
*/
}//: main


