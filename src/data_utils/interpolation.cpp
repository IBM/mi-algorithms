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
#include <data_utils/interpolation.hpp>

#include <algorithm>    // std::min, std::max

namespace mic {
namespace data_utils {


double linear_interpolation(double value, double x0, double x1) {

	//CHECK(value >= 0.0 && value <= 1.0);

	//if (value < 0.0 || value > 1.0) printf("%f\n", value);

	value = std::min(1.0, value);
	value = std::max(0.0, value);

    return x0 * (1.0 - value) + x1 * value;

}

double bilinear_interpolation(double value_x, double value_y, double ul, double ur, double ll, double lr) {

	// UL -a---- UR
	// ------------
	// ----x-------
	// LL -b---- LR
	//
	// a - upper_interpolation
	// b - lower_interpolation
	// x - final_interpolation
	
	double upper_interpolation = linear_interpolation(value_x, ul, ur);
	double lower_interpolation = linear_interpolation(value_x, ll, lr);
	double final_interpolation = linear_interpolation(value_y, upper_interpolation, lower_interpolation);

    return final_interpolation;

}



}//: namespace data_utils
}//: namespace mic
