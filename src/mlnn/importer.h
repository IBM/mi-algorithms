/*
* @Author: kmrocki
* @Date:   2016-02-24 10:20:09
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-03-08 16:03:58
*/

/*TODO:
Integrate with external MI data importers*/

#ifndef __MNIST_IMPORTER__
#define __MNIST_IMPORTER__

#include <deque>
#include <fstream>

#include <types/MatrixTypes.hpp>

typedef struct {

	mic::types::VectorXf x; 	//inputs
	int y; 		//label

} datapoint;

//TODO: importers extend this class
class ImageImporter {


};

class CIFAR10Importer : public ImageImporter {

	public:

		static std::deque<datapoint> importFromFile(const char* filename, bool transpose_inputs = false) {

			const size_t offset_bytes = 0;
			const size_t w = 32;
			const size_t h = 32;
			const size_t c = 3;

			std::deque<datapoint> out;

			char buffer[w * h * c + 1];

			size_t allocs = 0;

			std::ifstream infile(filename, std::ios::in | std::ios::binary);

			if (infile.is_open()) {

				printf("Loading data from %s", filename);
				fflush(stdout);

				infile.seekg (offset_bytes, std::ios::beg);

				while (!infile.eof()) {

					infile.read(buffer, w * h * c + 1);

					if (!infile.eof()) {

						mic::types::VectorXf temp(w * h * c);

						allocs++;

						if (allocs % 1000 == 0) {
							putchar('.');
							fflush(stdout);
						}

						for (unsigned i = 0; i < w * h * c; i++) {

							temp(i) = (float)((uint8_t)buffer[i + 1]) / 255.0f;

						}

						datapoint dp;

						dp.x = temp;
						dp.y = (unsigned int)buffer[0];
						out.push_back(dp);

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

class MNISTImporter : public ImageImporter {

	public:

		static std::deque<datapoint> importFromFile(const char* filename,
				const char* labels_filename, bool transpose_inputs = true) {

			const size_t offset_bytes = 16;
			const size_t offset_bytes_lab = 8;
			const size_t w = 28;
			const size_t h = 28;

			std::deque<datapoint> out;

			char buffer[w * h];
			char buffer_lab;

			size_t allocs = 0;

			std::ifstream infile(filename, std::ios::in | std::ios::binary);
			std::ifstream labels_file(labels_filename, std::ios::in | std::ios::binary);

			if (infile.is_open() && labels_file.is_open()) {

				printf("Loading data from %s", filename);
				fflush(stdout);

				infile.seekg (offset_bytes, std::ios::beg);
				labels_file.seekg (offset_bytes_lab, std::ios::beg);

				while (!infile.eof() && !labels_file.eof()) {

					infile.read(buffer, w * h);
					labels_file.read(&buffer_lab, 1);

					if (!infile.eof() && !labels_file.eof()) {

						mic::types::VectorXf temp(w * h);


						allocs++;

						if (allocs % 1000 == 0) {
							putchar('.');
							fflush(stdout);
						}

						for (unsigned i = 0; i < w * h; i++) {

							temp(i) = (float)((uint8_t)buffer[i]) / 255.0f;

						}

						datapoint dp;
						//Eigen uses column-major layout as default, this is a quick fix to make visualizations easier
						if (transpose_inputs) {
							mic::types::MatrixXf transposed = temp;
							transposed.resize(w, h);
							transposed.transposeInPlace();
							transposed.resize(w * h, 1);
							temp = transposed;
						}
						dp.x = temp;
						dp.y = (unsigned int)buffer_lab;
						out.push_back(dp);

					}

				}

				printf("Finished.\n");
				infile.close();
				labels_file.close();

			} else {

				printf("Oops! Couldn't find file %s or %s\n", filename, labels_filename);
			}

			return out;

		}

};

#endif

