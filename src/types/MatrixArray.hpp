/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*!
 * 	@Author: kmrocki
 * 	@Date:   2016-04-05
 */

#ifndef __MATRIXARRAY_H__
#define __MATRIXARRAY_H__

#include <types/MatrixTypes.hpp>
#include <string>
#include <map>
#include <stdio.h>
#include <exception>


// Forward declaration of class boost::serialization::access
namespace boost {
namespace serialization {
class access;
}//: serialization
}//: access

namespace mic {
namespace types {


/*!
 * 	\brief A dynamic array of matrices.
 *	It's just what it looks like - std::vector<Matrix>;
 *	elements are are accessible by name with operator []
 *	name needs to be given together with name in the
 *	main constructor.
 *
 *	An example:
 *
 *	m = MatrixArray(
 *
 *		{	"something"	}, {
 *
 *			std::make_tuple("W", M, N),
 *			std::make_tuple("U", X, Y),
 *
 *		}
 *	)
 *
 *	will make a 2 sub-matrices "W" and "U" of dimensions M x N and X x Y
 *	respectively;
 *
 *	later, m['W'] will return the first matrix and m['U'] the second one.
 *
 *	Other things are just implementations of operators and IO.
 *
 * \tparam T Template parameter denoting elementary type of data used (int, float, double etc.)
 * \date Apr 11, 2016
 *	\author krocki/tkornuta
 */
template<typename T>
class MatrixArray {
public:

	/*!
	 * Default empty constructor.
	 */
	MatrixArray() = default;

	/*!
	 * Simple constructor. Stores the name.
	 * @param name_ Name of the array.
	 */
	MatrixArray(std::string name_)  : array_name ( name_ ) {
	}


	/*!
	 * The main constructor. Adds parameters.
	 * @param name_ Name of the array.
	 * @param args_ Vector of tuples containing <id, width, height>.
	 */
	MatrixArray ( std::string name_, std::initializer_list<std::tuple<std::string, size_t, size_t> > args_) : array_name ( name_ ) {

		add ( args_ );

	}

	MatrixArray ( const MatrixArray& other ) {
		// Copy name.
		array_name = other.array_name;

		keys_map.clear();
		matrices.clear();
		// Copy data.
		for (auto& i: other.keys_map) {
			std::string tmp_name = i.first;
			mic::types::MatrixPtr<T> tmp_mat_ptr =  other.matrices[i.second];

			// Add tuple to array.
 			keys_map[ tmp_name ] = matrices.size();
 			matrices.push_back ( std::make_shared<mic::types::Matrix<T> > (mic::types::Matrix<T> (*tmp_mat_ptr) ) );
		}//: for

	}

	MatrixArray& operator= ( const MatrixArray& other ) {
		// Copy name.
		array_name = other.array_name;

		keys_map.clear();
		matrices.clear();
		// Copy data.
		for (auto& i: other.keys_map) {
			std::string tmp_name = i.first;
			mic::types::MatrixPtr<T> tmp_mat_ptr =  other.matrices[i.second];

			// Add tuple to array.
 			keys_map[ tmp_name ] = matrices.size();
 			matrices.push_back ( std::make_shared<mic::types::Matrix<T> > (mic::types::Matrix<T> (*tmp_mat_ptr) ) );
		}//: for

		return *this;
	}

	/*!
	 * Adds several matrices at once.
	 * @param params_ Vector of tuples containing <id, width, height>.
	 */
	void add ( std::initializer_list<std::tuple<std::string, size_t, size_t> > params_ ) {
		for ( auto i : params_ ) {
			keys_map[std::get<0> ( i )] = matrices.size();
			matrices.push_back ( std::make_shared<mic::types::Matrix<T> > ( mic::types::Matrix<T> ( std::get<1> ( i ), std::get<2> ( i ) ) ) );
		}//: for
	}

	/*!
	 * Adds a single matrix to array.
	 * @param param_ A tuple to be added.
	 */
	void add ( std::tuple<std::string, size_t, size_t> param_ ) {
		keys_map[std::get<0> ( param_ )] = matrices.size();
		matrices.push_back ( std::make_shared<mic::types::Matrix<T> > ( mic::types::Matrix<T> ( std::get<1> ( param_ ), std::get<2> ( param_ ) ) ) );
	}

	/*!
	 * Adds a single matrix to array.
	 * @param name_ Name of the matrix.
	 * @param input_ Input length.
	 * @param output_ Output length.
	 */
	void add ( std::string name_, size_t input_, size_t output_) {
		keys_map[name_] = matrices.size();
		matrices.push_back ( std::make_shared<mic::types::Matrix<T> > ( mic::types::Matrix<T> ( input_, output_ ) ) );
	}

	/*!
	 * Adds a pointer to an existing matrix to array.
	 * @param name_ Name of the matrix.
	 * @param matrix_ptr_ Pointer to the existing array.
	 */
	void add ( std::string name_, std::shared_ptr<mic::types::Matrix<T> > matrix_ptr_) {
		keys_map[name_] = matrices.size();
		matrices.push_back ( matrix_ptr_);
	}

	/*!
	 * Returns the matrix with given number.
	 * @param number_ Number of the matrix.
	 * @return Pointer to a matrix.
	 */
	mic::types::MatrixPtr<T>& operator[] ( size_t number_ ) {
		if (number_ >= matrices.size())
			throw std::range_error("MatrixArray " + array_name + " size is smaller than: " + std::to_string(number_));

		return matrices[number_];
	}

	/*!
	 * Checks whether matrix indexed by a given key exists.
	 * @param key_ Key as a single character.
	 */
	inline bool keyExists(char key_) {
		return (keys_map.find ( std::to_string(key_) ) == keys_map.end());
	}

	/*!
	 * Checks whether matrix indexed by a given key exists.
	 * @param key_ Key as string.
	 */
	inline bool keyExists(std::string key_) {
		return keys_map.find ( key_ ) != keys_map.end();
	}


	/*!
	 * Returns the matrix with given key (id).
	 * @param number_ Matrix key.
	 * @return Pointer to a matrix.
	 */
	mic::types::MatrixPtr<T>& operator[] ( char key_ ) {
		if ( !keyExists(key_) )
			throw std::range_error("MatrixArray " + array_name + " does not have a key " + key_);

		return ( *this ) [std::string ( 1, key_ )];
	}

	/*!
	 * Returns the matrix with given key (id).
	 * @param number_ Matrix key.
	 * @return Pointer to a matrix.
	 */
	mic::types::MatrixPtr<T>& operator[] ( std::string key_ ) {
		if ( !keyExists(key_) )
			throw std::range_error("MatrixArray " + array_name + " does not have a key " + key_);

		return matrices[keys_map[key_]];
	}

	/*!
	 * Stream operator enabling to print the array.
	 * @param os_ Ostream object.
	 * @param obj_ Array object.
	 */
	friend std::ostream& operator<<(std::ostream& os_, const MatrixArray& obj_) {
		// Display name
		os_ << "[" << obj_.array_name << "]:\n";
		for (auto& i: obj_.keys_map) {
			// Display elements.
			os_ << "(" << i.second << ") [" << i.first << "]:\n";
			os_ << (*obj_.matrices[i.second]) << std::endl;
		}
		return os_;
	}

	/*!
	 * Zeroes all matrices.
	 */
	void setZero() {

		for ( size_t i = 0; i < matrices.size(); i++ )
			matrices[i]->setZero();
	}

	/*!
	 * Returns the name of the vector of matrices.
	 */
	std::string name() {
		return array_name;
	}

	/*!
	 * Returns the name of the vector of matrices.
	 */
	std::map<std::string, size_t> keys() {
		return keys_map;
	}

	/*!
	 * Returns the size of vector.
	 */
	size_t size() {
		return matrices.size();
	}

protected:
	/// Name of the given vector of matrices.
	std::string array_name;

	/// Vector of matrices.
	std::vector<mic::types::MatrixPtr<T> > matrices;


	/// Vector of names of consecutive matrices in the array.
	std::map<std::string, size_t> keys_map;

private:
	// Friend class - required for using boost serialization.
    friend class boost::serialization::access;

    /*!
     * Serialization save - saves the matrix object to archive.
     * @param ar Used archive.
     * @param version Version of the matrix class (not used currently).
     */
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const {
		// Serialize name and size.
		ar & array_name;
		size_t size = matrices.size();
		ar & size;
		// Serialize elements.
		for (auto& i: keys_map) {
			ar & i.first;
			ar & (*matrices[i.second]);
		}//: for
     }

    /*!
     * Serialization load - loads the matrix object to archive.
     * @param ar Used archive.
     * @param version Version of the matrix class (not used currently).
     */
     template<class Archive>
     void load(Archive & ar, const unsigned int version) {
 		// Deserialize name and size.
 		ar & array_name;
 		size_t size;
 		ar & size;
 		// Deserialize elements.
 		for (size_t i=0; i < size; i++) {
 			std::string tmp_name;
 			ar & tmp_name;
 			mic::types::Matrix<T> tmp_mat;
 			ar & tmp_mat;
 			// Add tuple to array.
 			keys_map[ tmp_name ] = matrices.size();
 			matrices.push_back ( std::make_shared<mic::types::Matrix<T> > (mic::types::Matrix<T> (tmp_mat) ) );

 		}//: for

     }

     // The serialization must be splited as load requires to allocate the memory.
     BOOST_SERIALIZATION_SPLIT_MEMBER()

};

}//: namespace types
}//: namespace mic

// Just in the case that something important will change in the MatrixArray class - set version.
BOOST_CLASS_VERSION(mic::types::MatrixArray<bool>, 1)
BOOST_CLASS_VERSION(mic::types::MatrixArray<short>, 1)
BOOST_CLASS_VERSION(mic::types::MatrixArray<int>, 1)
BOOST_CLASS_VERSION(mic::types::MatrixArray<long>, 1)
BOOST_CLASS_VERSION(mic::types::MatrixArray<float>, 1)
BOOST_CLASS_VERSION(mic::types::MatrixArray<double>, 1)



#endif /*__MATRIXARRAY_H__*/
