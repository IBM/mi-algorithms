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
	 * @param id_ Id (optional, as defalut set to "")
	 */
	MatrixArray(std::string name_, std::string id_="")  : name ( name_ + "_" + id_ ) {
	}


	/*!
	 * The main constructor. Adds parameters.
	 */
	MatrixArray ( std::string name_, std::initializer_list<std::tuple<std::string, size_t, size_t> > args_, std::string id_ ) : name ( name_ + "_" + id_ ) {

		add ( args_ );

	}

	MatrixArray ( const MatrixArray& other ) {

		namemap = other.namemap;
		name = other.name;
		matrices = other.matrices;

	}

	MatrixArray& operator= ( const MatrixArray& other ) {

		namemap = other.namemap;
		name = other.name;
		matrices = other.matrices;

		return *this;
	}

	/*!
	 * Adds several matrices at once.
	 * @param params_ Vector of tuples containing <id, width, height>.
	 */
	void add ( std::initializer_list<std::tuple<std::string, size_t, size_t> > params_ ) {
		for ( auto i : params_ ) {
			namemap[std::get<0> ( i )] = matrices.size();
			matrices.push_back ( std::make_shared<mic::types::Matrix<T> > ( mic::types::Matrix<T> ( std::get<1> ( i ), std::get<2> ( i ) ) ) );
		}//: for
	}

	/*!
	 * Adds a single matrix to array.
	 * @param param_ A tuple to be added.
	 */
	void add ( std::tuple<std::string, size_t, size_t> param_ ) {
		namemap[std::get<0> ( param_ )] = matrices.size();
		matrices.push_back ( std::make_shared<mic::types::Matrix<T> > ( mic::types::Matrix<T> ( std::get<1> ( param_ ), std::get<2> ( param_ ) ) ) );
	}

	/*!
	 * Returns the matrix with given number.
	 * @param number_ Number of the matrix.
	 * @return Pointer to a matrix.
	 */
	mic::types::MatrixPtr<T>& operator[] ( size_t number_ ) {

		return matrices[number_];

	}

	/*!
	 * Returns the matrix with given key (id).
	 * @param number_ Matrix key.
	 * @return Pointer to a matrix.
	 */
	mic::types::MatrixPtr<T>& operator[] ( char key_ ) {

		return ( *this ) [std::string ( 1, key_ )];

	}

	/*!
	 * Returns the matrix with given key (id).
	 * @param number_ Matrix key.
	 * @return Pointer to a matrix.
	 */
	mic::types::MatrixPtr<T>& operator[] ( std::string key ) {

		if ( namemap.find ( key ) == namemap.end() )
			std::cout << "Warning !!! " << name <<
					  "::[] - key not found:" << key << std::endl;

		return matrices[namemap[key]];

	}

	/*!
	 * Stream operator enabling to print the array.
	 * @param os_ Ostream object.
	 * @param obj_ Array object.
	 */
	friend std::ostream& operator<<(std::ostream& os_, const MatrixArray& obj_) {
		// Display name
		os_ << "[" << obj_.name << "]:\n";
		for (auto& i: obj_.namemap) {
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
	std::string getName() {
		return name;
	}

	/*!
	 * Returns the size of vector.
	 */
	std::string size() {
		return matrices.size();
	}

protected:
	/// Name of the given vector of matrices.
	std::string name;

	/// Vector of matrices.
	std::vector<mic::types::MatrixPtr<T> > matrices;


	/// Vector of names of consecutive matrices in the array.
	std::map<std::string, size_t> namemap;

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
		ar & name;
		size_t size = matrices.size();
		ar & size;
		// Serialize elements.
		for (auto& i: namemap) {
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
 		ar & name;
 		size_t size;
 		ar & size;
 		// Deserialize elements.
 		for (size_t i=0; i < size; i++) {
 			std::string tmp_name;
 			ar & tmp_name;
 			mic::types::Matrix<T> tmp_mat;
 			ar & tmp_mat;
 			// Add tuple to array.
 			namemap[ tmp_name ] = matrices.size();
 			matrices.push_back ( std::make_shared<mic::types::Matrix<T> > (mic::types::Matrix<T> (tmp_mat) ) );

 		}//: for

     }

     // The serialization must be splited as load requires to allocate the memory.
     BOOST_SERIALIZATION_SPLIT_MEMBER()

};

}//: namespace types
}//: namespace mic



#endif /*__MATRIXARRAY_H__*/
