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
#ifndef __TOKENIZE_H__
#define __TOKENIZE_H__

#include <string>
#include <vector>

namespace mic {
  namespace data_utils {

    std::vector<size_t> tokenize_uint(std::string str, char c);
    std::vector<float> tokenize_float(std::string str, char c);

  }//: namespace data_utils
}//: namespace mic

#endif  /* __TOKENIZE_H__ */
