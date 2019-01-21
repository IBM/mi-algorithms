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
#ifndef __TIME_SERIES_H__
#define __TIME_SERIES_H__

namespace mic {
namespace data_utils {

#ifdef __cplusplus 
extern "C" {
#endif

float population_equation(float t, float x);

#ifdef __cplusplus
}
#endif

}//: namespace data_utils
}//: namespace mic

#endif
