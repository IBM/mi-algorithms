/*!
 * \file Logger_test.cpp
 * \brief Program for testing logger (with console output).
 * \author tkornuta
 * \date Dec 1, 2015
 */

#include <logger/Log.hpp>
#include <logger/ConsoleOutput.hpp>
using namespace mic::logger;


/*!
 * \brief Main program function for testing logger with console output.
 * \author tkornuta
 * @param[in] argc Number of parameters (not used).
 * @param[in] argv List of parameters (not used).
 * @return (not used).
 */
int main(int argc, char* argv[]) {
	// Set console output.
	ConsoleOutput* co = new ConsoleOutput();
	LOGGER->addOutput(co);


	int logger_level = 6;
	while(1) {
		// Change logger level.
		co->setLvl((Severity_t)logger_level);

		LOG(LTRACE) << "First: trace message ";
		LOG(LDEBUG) << "Second: debug message ";
		LOG(LINFO) << "Third: info message ";
		LOG(LWARNING) << "Fourth: warning message ";
		LOG(LERROR) << "Fifth: error message ";
		LOG(LFATAL) << "Sixth: fatal error message ";

		// Change logger level.
		if (logger_level > 0)
			logger_level--;

		// Sleep.
		usleep(1000);
	}//: while

}//: main
