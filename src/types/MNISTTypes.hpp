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
typedef mic::types::Sample<mic::types::MatrixXf, unsigned int> MNISTSample;

/*!
 * \brief Pointer to the <MatrixXf-uint> sample type used by e.g. MNISTImporter.
 * \author tkornuta
 */
typedef std::shared_ptr<mic::types::MNISTSample> MNISTSamplePtr;


/*!
 * \brief The <MatrixXf-uint> batch type used by e.g. MNISTImporter.
 * \author tkornuta
 */
typedef mic::types::Batch<mic::types::MatrixXf, unsigned int> MNISTBatch;


/*!
 * \brief Pointer to the <MatrixXf-uint> batch type used by e.g. MNISTImporter.
 * \author tkornuta
 */
typedef std::shared_ptr<mic::types::MNISTBatch> MNISTBatchPtr;


}//: namespace types
}//: namespace mic



#endif /* MNISTTYPES_HPP_ */
