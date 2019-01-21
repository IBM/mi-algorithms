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
/*
 * \file get_average.hpp
 * \brief 
 * \author: tkornut
 * \date Nov 20, 2015
 */

#ifndef SRC_DATA_UTILS_GET_AVERAGE_HPP_
#define SRC_DATA_UTILS_GET_AVERAGE_HPP_

#include <vector>
#include <numeric>

namespace mic {
namespace data_utils {

/*!
 * @brief Computes average value of a given data taking into account the last number of elements.
 * @author krocki/tkornuta
 * @param data Data vector.
 * @param period Number of previous data used.
 * @tparam Type Template parameter denoting datatype.
 * @return Average.
 */
template<typename Type> Type get_average(std::vector<Type> data, unsigned period) {
	// Truncate size.
    if (period > data.size())
        period = (unsigned)data.size();

    // Return 0 if there are no data.
    if (period < 1)
        return (Type)0.0f;

    // Return average otherwise.
    return (Type)(std::accumulate(data.end() - period, data.end(), (Type)0.0f) / (Type)period);
}



}//: namespace data_utils
}//: namespace mic

#endif /* SRC_DATA_UTILS_GET_AVERAGE_HPP_ */
