/*!
 * \file data_collector_test.cpp
 * \brief Program for testing data collector.
 * \author tkornuta
 * \date Dec 1, 2015
 */

#include <data_io/DataCollector.hpp>


/*!
 * \brief Main function for testing data collector.
 * \author tkornuta
 * @param[in] argc Number of parameters (not used).
 * @param[in] argv List of parameters (not used).
 * @return (not used).
 */
int main(int argc, char* argv[]) {

	// Create data collector.
	mic::data_io::DataCollectorPtr<std::string, float> collector_ptr;
	collector_ptr = std::make_shared < mic::data_io::DataCollector<std::string, float> >( );//new mic::data_io::DataCollector<std::string, float>() );

	// Create containers.
	collector_ptr->createContainer("Iteration");
	collector_ptr->createContainer("Max(Pm)");
	collector_ptr->createContainer("FixedRangeTest",-1,1);

	// Add data to containers.
	collector_ptr->addDataToContainer("Iteration", 1 );
	collector_ptr->addDataToContainer("Iteration", 2);
	collector_ptr->addDataToContainer("Max(Pm)", 0.3);
	collector_ptr->addDataToContainer("FixedRangeTest", -0.99);
	collector_ptr->addDataToContainer("FixedRangeTest", 0.99);
	collector_ptr->addDataToContainer("FixedRangeTest", 0);

	// Export collected data.
	collector_ptr->exportDataToCsv("test1.csv");

	// Export vector to file.
	std::vector<double> doubles;
	doubles.push_back(0.1);
	doubles.push_back(0.2);
	doubles.push_back(0.3);
	// Truncate file.
	mic::data_io::DataCollector<std::string, double>::exportVectorToCsv("test2.csv", "doubles", doubles);

	double value = 0.4;
	mic::data_io::DataCollector<std::string, double>::exportValueToCsv("test2.csv", "next double", value, true);
	doubles.push_back(value);

	// Append the  data once again.
	mic::data_io::DataCollector<std::string, double>::exportVectorToCsv("test2.csv", "doubles", doubles, true);
}//: main
