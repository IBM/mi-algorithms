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
 * \file FloatVectorImporter.cpp
 * \brief 
 * \author gwburr
 * \date May 27, 2016
 */

#include <data_io/FloatVectorImporter.hpp>
#include <logger/Log.hpp>
#include <data_utils/image_tools.hpp>

#include <fstream>

namespace mic {
  namespace data_io {

    FloatVectorImporter::FloatVectorImporter(std::string node_name_) : Importer(node_name_),
    data_filename("data_filename", "inputData.dat") {
      // Register properties - so their values can be overridden (read from the configuration file).
      registerProperty(data_filename);
    }

    bool FloatVectorImporter::importData(void) {
      return (true);
    }

    bool FloatVectorImporter::readData(float *destination) {
      // Read file containing float vector
      std::ifstream data_file(data_filename, std::ios::in | std::ios::binary);
      if (data_file.is_open()) {
        data_file.read((char *)(&vector_length), sizeof(unsigned int));
        unsigned int testValue;
        for (size_t i = 0; i < vector_length; i++) {
          if (!data_file.eof()) {
            data_file.read((char *)(destination+i), sizeof (float));
            //LOG(LSTATUS) << "reading element " << i << " and got " << destination[i];
          }
        }

        data_file.read((char*)(&testValue), sizeof(unsigned int));
        //LOG(LSTATUS) << "testValue is " << testValue;
        if (testValue == 67) {
          LOG(LINFO) << "Successfully imported float vector of size " << vector_length;
        }
        data_file.close();
      }
      else {
        LOG(LFATAL) << "Oops! Couldn't find file: " << data_filename;
        return false;
      }
      return true;
    }

    unsigned int FloatVectorImporter::queryVectorLength(void) {
      std::ifstream data_file(data_filename, std::ios::in | std::ios::binary);
      if (data_file.is_open()) {
        data_file.read((char *)(&vector_length), sizeof(unsigned int));
        data_file.close();
      }
      else {
        LOG(LFATAL) << "Oops! Couldn't find file: " << data_filename;
        return (0);
      }
      LOG(LINFO) << "Float vector in file " << data_filename << " is of length " << vector_length;
      return (vector_length);
    }
  } /* namespace data_io */
} /* namespace mic */
