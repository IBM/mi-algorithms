/*!
 * @file: MNISTTypes.hpp
 * @Author: Tomasz Kornuta <tkornut@us.ibm.com>
 * @Date:   27-03-2017
 *
 * Copyright (c) 2016, IBM Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
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
