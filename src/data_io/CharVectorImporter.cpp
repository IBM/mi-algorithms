/*!
 * \file CharVectorImporter.cpp
 * \brief 
 * \author gwburr
 * \date May 27, 2016
 */

#include <data_io/CharVectorImporter.hpp>
#include <logger/Log.hpp>
#include <data_utils/image_tools.hpp>

#include <fstream>

namespace mic {
  namespace data_io {

    CharVectorImporter::CharVectorImporter(std::string node_name_) : Importer(node_name_),
    data_filename("data_filename", "inputData.dat") {
      // Register properties - so their values can be overridden (read from the configuration file).
      registerProperty(data_filename);
    }

    bool CharVectorImporter::importData(void) {
      return (true);
    }

    bool CharVectorImporter::readData(char *destination) {
      // Read file containing char vector
      std::ifstream data_file(data_filename, std::ios::in | std::ios::binary);
      if (data_file.is_open()) {
        data_file.read((char *)(&vector_length), sizeof(unsigned int));
        unsigned int testValue;
        for (size_t i = 0; i < vector_length; i++) {
          if (!data_file.eof()) {
            data_file.read((char *)(destination+i), sizeof (char));
            //LOG(LSTATUS) << "reading element " << i << " and got " << destination[i];
          }
        }
        destination[vector_length] = '\0'; // properly terminate the string
        data_file.read((char*)(&testValue), sizeof(unsigned int));
        //LOG(LSTATUS) << "testValue is " << testValue;
        if (testValue == 67) {
          LOG(LINFO) << "Successfully imported char vector of size " << vector_length;
        }
        data_file.close();
      }
      else {
        LOG(LFATAL) << "Oops! Couldn't find file: " << data_filename;
        return false;
      }
      return true;
    }

    unsigned int CharVectorImporter::queryVectorLength(void) {
      std::ifstream data_file(data_filename, std::ios::in | std::ios::binary);
      if (data_file.is_open()) {
        data_file.read((char *)(&vector_length), sizeof(unsigned int));
        data_file.close();
      }
      else {
        LOG(LFATAL) << "Oops! Couldn't find file: " << data_filename;
        return (0);
      }
      LOG(LINFO) << "Char vector in file " << data_filename << " is of length " << vector_length;
      return (vector_length);
    }
  } /* namespace data_io */
} /* namespace mic */
