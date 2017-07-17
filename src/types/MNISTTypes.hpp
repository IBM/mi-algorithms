/*!
 * @file: MNISTTypes.hpp
 * @Author: Tomasz Kornuta <tkornut@us.ibm.com>
 * @Date:   27-03-2017
 *
 * Copyright (c) 2017, Tomasz Kornuta, IBM Corporation. All rights reserved.
 *
 */

#ifndef MNISTTYPES_HPP_
#define MNISTTYPES_HPP_

#include <types/MatrixTypes.hpp>
#include <types/Sample.hpp>
#include <types/Batch.hpp>


namespace mic {
namespace types {

/*!
 * \brief The <MatrixXf-uint> sample type used by e.g. MNISTImporter.
 * \author tkornuta
 */
template <typename inputDataType = float>
using MNISTSample = mic::types::Sample<mic::types::Matrix<inputDataType>, unsigned int>;

/*!
 * \brief Pointer to the <MatrixXf-uint> sample type used by e.g. MNISTImporter.
 * \author tkornuta
 */
template <typename inputDataType = float>
using MNISTSamplePtr = std::shared_ptr<mic::types::MNISTSample<inputDataType> >;

/*!
 * \brief The <MatrixXf-uint> batch type used by e.g. MNISTImporter.
 * \author tkornuta
 */
template <typename inputDataType = float>
using MNISTBatch = mic::types::Batch<mic::types::Matrix<inputDataType>, unsigned int>;

/*!
 * \brief Pointer to the <MatrixXf-uint> batch type used by e.g. MNISTImporter.
 * \author tkornuta
 */
template <typename inputDataType = float>
using MNISTBatchPtr = std::shared_ptr<mic::types::MNISTBatch<inputDataType> >;


}//: namespace types
}//: namespace mic



#endif /* MNISTTYPES_HPP_ */
