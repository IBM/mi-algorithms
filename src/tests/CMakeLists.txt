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
# Build and install - matrix test application.
# =======================================================================

set(BUILD_TEST_MATRIX ON CACHE BOOL "Build the application for testing matrix multiplication")

if(${BUILD_TEST_MATRIX})
	# Create exeutable.
	ADD_EXECUTABLE(matrix_test matrix_test.cpp)
	# Link it with shared libraries.
	target_link_libraries(matrix_test 
		types
		${Boost_LIBRARIES}
		${EIGEN_LIBRARIES} 
		)
	if(OpenBLAS_FOUND)
		target_link_libraries(matrix_test  ${OpenBLAS_LIB} )
	endif(OpenBLAS_FOUND)

	# install test to bin directory
	install(TARGETS matrix_test RUNTIME DESTINATION bin)

endif(${BUILD_TEST_MATRIX})

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


# =======================================================================
# Build and install - matrix array test.
# =======================================================================

set(BUILD_TEST_MATRIX_ARRAY ON CACHE BOOL "Build the application for testing creation/serialization of matrix array class")

if(${BUILD_TEST_MATRIX_ARRAY})
	# Create exeutable.
	ADD_EXECUTABLE(matrix_array_test matrix_array_test.cpp)
	# Link it with shared libraries.
	target_link_libraries(matrix_array_test
		configuration
		types
		data_io
		${Boost_LIBRARIES}
		)

	# install test to bin directory
	install(TARGETS matrix_array_test RUNTIME DESTINATION bin)

endif(${BUILD_TEST_MATRIX_ARRAY})

# =======================================================================
# Build and install - mlnn test.
# =======================================================================

set(BUILD_TEST_MLNN ON CACHE BOOL "Build the application for testing creation/serialization of multi-layer neural networks")

if(${BUILD_TEST_MLNN})
	# Create exeutable.
	ADD_EXECUTABLE(mlnn_test mlnn_test.cpp)
	# Link it with shared libraries.
	target_link_libraries(mlnn_test 
		configuration
		types
		data_io
		mlnn
		${Boost_LIBRARIES}
		)

	# install test to bin directory
	install(TARGETS mlnn_test RUNTIME DESTINATION bin)

endif(${BUILD_TEST_MLNN})

# =======================================================================
# Build and install - MNIST recognition based on CNNs.
# =======================================================================

set(BUILD_MNIST_CNN_APP ON CACHE BOOL "Build the application using Convolutional Neural Network for recognition of MNIST digits")

if(${BUILD_MNIST_CNN_APP})
        # Create exeutable.
        ADD_EXECUTABLE(mnist_convnet mnist_convnet.cpp)
        # Link it with shared libraries.
        target_link_libraries(mnist_convnet
			logger
	        types
			data_io
			encoders
			mlnn
		    ${Boost_LIBRARIES}
	    )
        if(OpenBLAS_FOUND)
                target_link_libraries(mnist_convnet  ${OpenBLAS_LIB} )
        endif(OpenBLAS_FOUND)

        # install test to bin directory
        install(TARGETS mnist_convnet RUNTIME DESTINATION bin)

endif(${BUILD_MNIST_CNN_APP})


# =======================================================================
# Build and install -  MNIST recognition based on MLP.
# =======================================================================

set(BUILD_MNIST_MLP_APP ON CACHE BOOL "Build the application using Multi-Layer Perceptron for recognition of MNIST digits")

if(${BUILD_MNIST_MLP_APP})
        # Create exeutable.
        ADD_EXECUTABLE(mnist_simple_mlp_app mnist_simple_mlp.cpp)
        # Link it with shared libraries.
        target_link_libraries(mnist_simple_mlp_app
			logger
	        types
			data_io
			encoders
			mlnn
	        ${Boost_LIBRARIES}
	        )
        if(OpenBLAS_FOUND)
                target_link_libraries(mnist_simple_mlp_app  ${OpenBLAS_LIB} )
        endif(OpenBLAS_FOUND)

        # install test to bin directory
        install(TARGETS mnist_simple_mlp_app RUNTIME DESTINATION bin)

endif(${BUILD_MNIST_MLP_APP})
