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
 * \file FloatVectorImporter.hpp
 * \brief 
 * \author gwburr
 * \date May 27, 2016
 */

#ifndef SRC_DATA_IO_FLOATVECTORIMPORTER_HPP_
#define SRC_DATA_IO_FLOATVECTORIMPORTER_HPP_

#include <data_io/Importer.hpp>
#include <types/image_types.hpp>

//#include <configuration/PropertyTree.hpp>

namespace mic {
  namespace data_io {

    /*!
     * \brief Importer responsible for importing/loading float vectors.
     * \author gwburr
     */
    //class FloatVectorImporter : public mic::configuration::PropertyTree {
    class FloatVectorImporter : public mic::data_io::Importer<mic::types::image, unsigned int> {
    public:
      /*!
       * Constructor. Registers properties.
       * @param node_name_ Name of the node in configuration file.
       */
      FloatVectorImporter(std::string node_name_ = "float_vector_importer");

      /*!
       * Virtual destructor. Empty.
       */
      virtual ~FloatVectorImporter() {
      };

      /*!
       * Method responsible for importing/loading the float vectors.
       * @return TRUE if data loaded successfully, FALSE otherwise.
       */
      bool importData(void);
      bool readData(float *);
      unsigned int queryVectorLength(void);


      /*!
       * Sets data filename (with path). DEPRICATED (should be used only for testing purposes).
       * @param data_filename_ Path and filename
       */
      void setDataFilename(std::string data_filename_) {
        data_filename = data_filename_;
      }

      /*!
       * Method responsible for initialization of all variables that are property-dependent - here not required, yet empty.
       */
      virtual void initializePropertyDependentVariables() {
      };

    private:
      /*!
       * Length of vector.
       */
      unsigned int vector_length;

      /*!
       * Property: directory/Name of file containing images (binary datafile).
       */
      mic::configuration::Property<std::string> data_filename;

    };

  } /* namespace data_io */
} /* namespace mic */

#endif /* SRC_DATA_IO_FLOATVECTORIMPORTER_HPP_ */
