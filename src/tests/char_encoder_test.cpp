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

	mic::auto_encoders::DummyCharEncoder encoder;
	mic::types::floatSDR sdr;

	// Load dataset.
	mic::data_io::RawTextImporter importer;
	// Manually set paths. DEPRICATED!
	importer.setDataFilename("/Users/tkornut/Documents/workspace/machine-intelligence-core/data/txt/pl/ep-06-01-16-003.txt");
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
			char_pair_t sample = importer.getRandomSample();
			//LOG(LINFO)<<" Orig = '" << *(sample.first) << "' label = '" << *(sample.second) << "'";

			// Encode the selected image into SDR.
			encoder.encode(*(sample.first), sdr);

			// Process - change a single element of SDR...
			//sdr[0]=255;

			// Decode SDR.
			char dec_char;
			encoder.decode(dec_char, sdr);

			// Display result.
			LOG(LINFO)<<" Orig = '" << *(sample.first) << "' decoded SDR = '" << dec_char << "' label = '" << *(sample.second) << "'";

		}//: if ! paused

		// Sleep.
		APP_SLEEP();
	}//: while



	LOG(LINFO) << "Terminating application";
}//: main
