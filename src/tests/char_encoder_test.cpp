/*!
 * \file image_encoder_test.cpp
 * \brief Program for testing ImageEncoder and Visualization.
 * \author tkornuta
 * \date Nov 20, 2015
 */

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include <data_utils/image_tools.hpp>
#include <data_utils/rand.hpp>
using namespace mic::data_utils;

#include <data_io/RawTextImporter.hpp>

#include <auto_encoders/DummyCharEncoder.hpp>

#include <logger/Log.hpp>
#include <logger/ConsoleOutput.hpp>
using namespace mic::logger;

#include <application/ApplicationState.hpp>




/*!
 * \brief Main program function - tests character importer and encoder.
 * \author tkornuta
 * @param[in] argc Number of parameters - not used.
 * @param[in] argv List of parameters - not used.
 * @return (not used)
 */
int main(int argc, char* argv[]) {
	// Set console output to logger.
	LOGGER->addOutput(new ConsoleOutput());
	LOG(LINFO) << "Logger initialized. Starting application";

	// Manualy set sdr and batch size.
	size_t sdr_size = 128;
	size_t batch_size = 5;


	mic::auto_encoders::DummyCharEncoder encoder;
	mic::types::MatrixXf sdr(sdr_size,1);
	LOG(LWARNING) << "sdr.cols()=" << sdr.cols() << " sdr.rows()=" << sdr.rows();

	// Load dataset.
	mic::data_io::RawTextImporter importer;
	// Manually set paths. DEPRICATED! Used here only for simplification of the test.
	importer.setDataFilename("/Users/tkornut/Documents/workspace/machine-intelligence-core/data/txt/pl/ep-06-01-16-003.txt");
	importer.setBatchSize(batch_size);


	if (!importer.importData())
		return -1;

 	// Main application loop.
	while (!APP_STATE->Quit()) {

		// If not paused.
		if (!APP_STATE->isPaused()) {

			// If single step mode - pause after the step.
			if (APP_STATE->isSingleStepModeOn())
				APP_STATE->pressPause();

			// Random select image.
			char_char_pair_t sample = importer.getRandomSample();

			// Encode the selected image into SDR.
			encoder.encode(*(sample.first), sdr);

			// Decode SDR.
			char dec_char;
			encoder.decode(dec_char, sdr);

			// Display result.
			LOG(LINFO)<<" Orig = '" << *(sample.first) << "' decoded SDR = '" << dec_char << "' label = '" << *(sample.second) << "'";

			// Get random batch.
			char_char_batch_t batch = importer.getNextBatch();
			LOG(LINFO)<<" Batch: ";
			for (size_t i=0; i < batch.first.size(); i++ ) {
				LOG(LINFO)<<" ["<<i<< "] = '" << *(batch.first[i]) <<"'";
			}//: for


			// Encode the whole batch.
			mic::types::MatrixXf batch_matrix (sdr_size, batch_size);
			encoder.encode(batch.first, batch_matrix);
			LOG(LINFO)<<" Batched matrix: ";
			LOG(LINFO) << batch_matrix;

			// Decode batch matrix.
			char_char_batch_t decoded_batch;
			encoder.decode(decoded_batch.first, batch_matrix);


			LOG(LINFO)<<" Decoded batch: ";
			for (size_t i=0; i < decoded_batch.first.size(); i++ ) {
				LOG(LINFO)<<" ["<<i<< "] = '" << *(decoded_batch.first[i]) <<"'";
			}//: for


			// Check if the batch was the last one.
			if (importer.isLastBatch())
				break;
		}//: if ! paused

		// Sleep.
		APP_SLEEP();
	}//: while



	LOG(LINFO) << "Terminating application";
}//: main
