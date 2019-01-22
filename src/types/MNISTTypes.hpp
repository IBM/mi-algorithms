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
