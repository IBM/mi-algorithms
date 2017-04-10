# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Build and install - char encoder test application.
# =======================================================================

set(BUILD_TEST_CHAR_ENCODER ON CACHE BOOL "Build the 1-of-k Char Encoder test application")

if(${BUILD_TEST_CHAR_ENCODER})
	# Create exeutable.
	add_executable(char_encoder_test char_encoder_test.cpp)
	# Link it with shared libraries.
	target_link_libraries(char_encoder_test 
		logger
		application
		types
		data_io
		encoders
		configuration
		${Boost_LIBRARIES}
		)

	# install test to bin directory
	install(TARGETS char_encoder_test RUNTIME DESTINATION bin)

endif(${BUILD_TEST_CHAR_ENCODER})

# =======================================================================
# Build and install - tensor test application.
# =======================================================================

set(BUILD_TEST_TENSOR ON CACHE BOOL "Build the application for testing tensors/eigen map")

if(${BUILD_TEST_TENSOR})
	# Create exeutable.
	ADD_EXECUTABLE(tensor_test tensor_test.cpp)
	# Link it with shared libraries.
	target_link_libraries(tensor_test 
		types
		${Boost_LIBRARIES}
		${EIGEN_LIBRARIES} 
		)
	if(OpenBLAS_FOUND)
		target_link_libraries(tensor_test  ${OpenBLAS_LIB} )
	endif(OpenBLAS_FOUND)

	# install test to bin directory
	install(TARGETS tensor_test RUNTIME DESTINATION bin)

endif(${BUILD_TEST_TENSOR})

# =======================================================================
# Build and install - data collector test application.
# =======================================================================

set(BUILD_TEST_COLLECTOR ON CACHE BOOL "Build the application for testing data collector")

if(${BUILD_TEST_COLLECTOR})
	# Create exeutable.
	ADD_EXECUTABLE(data_collector_test data_collector_test.cpp)
	# Link it with shared libraries.
	target_link_libraries(data_collector_test 
		configuration
		types
		data_io
		${Boost_LIBRARIES}
		)

	# install test to bin directory
	install(TARGETS data_collector_test RUNTIME DESTINATION bin)

endif(${BUILD_TEST_COLLECTOR})

