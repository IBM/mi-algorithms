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
