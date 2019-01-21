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
 * \file Encoder.hpp
 * \brief Contains definition of an abstract, template parent class for all auto-encoders.
 * \author tkornuta
 * \date Nov 16, 2015
 */

#ifndef __ENCODER_HPP__
#define __ENCODER_HPP__

#include <string>
#include <vector>
#include <iostream>

namespace mic {

  /*!
   * \brief Contains auto-encoder parent class and all derived classes. DEPRICATED.
   * \author tkornuta
   */
  namespace auto_encoders {

    /*!
     * \brief Parent class for all auto-encoders.  DEPRICATED.
     *
     * \author tkornuta
     * \tparam inputDataType Template parameter defining the input datatype.
     * \tparam SDRType Template parameter defining the SDR type.
     */
    template <typename inputDataType, typename SDRType>
    class Encoder {
    public:

      /*!
       * @brief Constructor - empty.
       */
      Encoder() {
      };

      /*!
       * Virtual destructor - empty.
       */
      virtual ~Encoder() {
      };

      /*!
       * @brief Method responsible for encoding input data into SDR (a table of floats).
       * @param[in] input_ Input data.
       * @param[out] sdr_ Output SDR.
       */
      virtual void encode(const inputDataType& input_, SDRType& sdr_) = 0;

      /*!
       * Method responsible for decoding of SDR into data.
       * @param[out] output_ Output data.
       * @param[in] sdr_ Input SDR.
       */
      virtual void decode(inputDataType& output_, const SDRType& sdr_) = 0;

    };



  } // namespace auto_encoders
} // namespace mic

#endif
