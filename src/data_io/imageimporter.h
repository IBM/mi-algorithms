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
* @File: imageimporter.h
* @Author: kamilrocki
* @Email: kmrocki@us.ibm.com
* @Created:	2015-05-11 19:01:12
* @Last Modified by:   kamilrocki
* @Last Modified time: 2015-05-14 17:53:55
*/

#include <fstream>
#include <vector>
#include <types/matrix.h>

#ifndef __IMAGE_IMPORTER_H__
#define __IMAGE_IMPORTER_H__

class ImageImporter {

	public:

		ImageImporter() = default;
		ImageImporter(const ImageImporter&) = default;

		virtual ~ImageImporter() = default;

		static std::vector<Matrix<float>*> importFromFile(const char* filename, size_t w, size_t h, size_t offset_bytes = 0) {

			std::vector<Matrix<float>*> out;
			char buffer[w * h];
			size_t allocs = 0;

			std::ifstream infile(filename, std::ios::in | std::ios::binary);

			if (infile.is_open()) {

				printf("Loading images from %s", filename);
				fflush(stdout);

				infile.seekg (offset_bytes, std::ios::beg);

				while(!infile.eof()) {

					infile.read(buffer, w * h);

					Matrix<float>* temp;

					if (!infile.eof()) {

						temp = new Matrix<float>(w, h);
						allocs++;

						if (allocs % 1000 == 0) {
							putchar('.');
							fflush(stdout);
						}

						for (unsigned i = 0; i < w * h; i++) {

							unsigned row = i / (unsigned)w;
							unsigned col = i % (unsigned)h;

							(*temp)(row, col) = (float)((uint8_t)buffer[i])/255.0f;

						}

						out.push_back(temp);

					}

				}

				printf("Finished.\n");
				infile.close();

			} else {

				printf("Oops! Couldn't find file %s\n", filename);
			}
	
			return out;

		}

};

#endif
