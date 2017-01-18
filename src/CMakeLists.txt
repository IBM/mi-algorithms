# Check compiler flags
include(CheckCXXCompilerFlag)

# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Set compiler/linker flags.
# =======================================================================

if (WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -fpermissive -std=c++11 -save-temps")
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif ()

# Check, whether all necessary libraries are linked
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}") 

# =======================================================================
# Build MI Algorithms.
# =======================================================================

# =======================================================================
# Install includes related to CORE TYPES used by other libraries.
FILE(GLOB files types/*.hpp types/*.h)
install(FILES ${files} DESTINATION include/types)

# Create shared library containing CORE TYPES and related functions.
file(GLOB types_src types/*.cpp)
add_library(types SHARED ${types_src})
target_link_libraries(types logger ${Boost_LIBRARIES} )
if(OpenBLAS_FOUND)
	target_link_libraries(types  ${OpenBLAS_LIB} )
endif(OpenBLAS_FOUND)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "types" CACHE INTERNAL "" FORCE)
 
# =======================================================================
# Install includes related to DATA UTILS used by other libraries.
FILE(GLOB files data_utils/*.hpp data_utils/*.h)
install(FILES ${files} DESTINATION include/data_utils)

# Create shared library containing DATA UTILS.
file(GLOB data_utils_src data_utils/*.cpp)
add_library(data_utils SHARED ${data_utils_src})
target_link_libraries(data_utils types )

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "data_utils" CACHE INTERNAL "" FORCE)

# =======================================================================
# Install includes related to DATA IO used by other libraries.
FILE(GLOB files data_io/*.hpp)
install(FILES ${files} DESTINATION include/data_io)

# Create shared library containing DATA IO.
file(GLOB data_io_src data_io/*.cpp)
add_library(data_io SHARED ${data_io_src})
target_link_libraries(data_io types data_utils configuration)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "data_io" CACHE INTERNAL "" FORCE)

# =======================================================================
# Install includes related to (AUTO) ENCODERS used by other libraries.
FILE(GLOB files auto_encoders/*.hpp )
install(FILES ${files} DESTINATION include/auto_encoders)

FILE(GLOB files encoders/*.hpp )
install(FILES ${files} DESTINATION include/encoders)

# Create shared library containing AUTO ENCODERS.
file(GLOB encoders_src auto_encoders/*.cpp encoders/*.cpp)
add_library(encoders SHARED ${encoders_src})
target_link_libraries(encoders types )

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "encoders" CACHE INTERNAL "" FORCE)

# =======================================================================
# Install includes related to CLASSIFIERS used by other libraries.
FILE(GLOB files classifiers/*.hpp)
install(FILES ${files} DESTINATION include/classifiers)

# Create shared library containing CLASSIFIERS.
file(GLOB classifiers_src classifiers/*.cpp)
add_library(classifiers SHARED ${classifiers_src})
target_link_libraries(classifiers types data_utils)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "classifiers" CACHE INTERNAL "" FORCE)

# =======================================================================
# Install includes related to HTM used by other libraries.
FILE(GLOB files htm/*.h htm/*.hpp)
install(FILES ${files} DESTINATION include/htm)

FILE(GLOB files htm/shared/*.h htm/shared/*.hpp)
install(FILES ${files} DESTINATION include/htm/shared)

# Create HTM shared library.
file(GLOB htm_src htm/shared/*.cpp htm/*.cpp)
add_library(htm SHARED ${htm_src})
target_link_libraries(htm types data_utils system_utils)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "htm" CACHE INTERNAL "" FORCE)

## =======================================================================
## Install includes related to RBM used by other libraries.
#FILE(GLOB files rbm/*.hpp)
#install(FILES ${files} DESTINATION include/rbm)
#
## Create RBM shared library.
#file(GLOB rbm_src rbm/*.cpp )
#add_library(rbm SHARED ${rbm_src})
#target_link_libraries(rbm types data_utils)
#
## Add to variable storing all libraries/targets.
#set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "rbm" CACHE INTERNAL "" FORCE)

## =======================================================================
## Install includes related to MLNN used by other libraries.
#FILE(GLOB files mlnn/*.hpp mlnn/*.h)
#install(FILES ${files} DESTINATION include/mlnn)
#
## Create RBM shared library.
#file(GLOB mlnn_src mlnn/*.cpp )
#add_library(mlnn SHARED ${mlnn_src})
#target_link_libraries(mlnn types)
#
## Add to variable storing all libraries/targets.
#set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "mlnn" CACHE INTERNAL "" FORCE)

# =======================================================================
# Install target libraries.
# =======================================================================

install(TARGETS ${MIAlgorithms_LIBRARIES} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)

# =======================================================================
# Build and install - char encoder test application.
# =======================================================================

set(BUILD_TEST_CHAR_ENCODER ON CACHE BOOL "Build the 1-of-k Char Encoder test application")

if(${BUILD_TEST_CHAR_ENCODER})
	# Create exeutable.
	add_executable(char_encoder_test tests/char_encoder_test.cpp)
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


## =======================================================================
## Build and install - matrix test application.
## =======================================================================
#
#set(BUILD_TEST_MATRIX ON CACHE BOOL "Build the application for testing matrix multiplication")
#
#if(${BUILD_TEST_MATRIX})
#	# Create exeutable.
#	ADD_EXECUTABLE(matrix_test tests/matrix_test.cpp)
#	# Link it with shared libraries.
#	target_link_libraries(matrix_test 
#		types
#		${Boost_LIBRARIES}
#		${EIGEN_LIBRARIES} 
#		)
#	if(OpenBLAS_FOUND)
#		target_link_libraries(matrix_test  ${OpenBLAS_LIB} )
#	endif(OpenBLAS_FOUND)
#
#	# install test to bin directory
#	install(TARGETS matrix_test RUNTIME DESTINATION bin)
#
#endif(${BUILD_TEST_MATRIX})
#
# =======================================================================
# Build and install - tensor test application.
# =======================================================================

set(BUILD_TEST_TENSOR ON CACHE BOOL "Build the application for testing tensors/eigen map")

if(${BUILD_TEST_TENSOR})
	# Create exeutable.
	ADD_EXECUTABLE(tensor_test tests/tensor_test.cpp)
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
	ADD_EXECUTABLE(data_collector_test tests/data_collector_test.cpp)
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


## =======================================================================
## Build and install - matrix array test.
## =======================================================================
#
#set(BUILD_TEST_MATRIX_ARRAY ON CACHE BOOL "Build the application for testing creation/serialization of matrix array class")
#
#if(${BUILD_TEST_MATRIX_ARRAY})
#	# Create exeutable.
#	ADD_EXECUTABLE(matrix_array_test tests/matrix_array_test.cpp)
#	# Link it with shared libraries.
#	target_link_libraries(matrix_array_test
#		configuration
#		types
#		data_io
#		${Boost_LIBRARIES}
#		)
#
#	# install test to bin directory
#	install(TARGETS matrix_array_test RUNTIME DESTINATION bin)
#
#endif(${BUILD_TEST_MATRIX_ARRAY})

## =======================================================================
## Build and install - mlnn test.
## =======================================================================
#
#set(BUILD_TEST_MLNN ON CACHE BOOL "Build the application for testing creation/serialization of multi-layer neural networks")
#
#if(${BUILD_TEST_MLNN})
#	# Create exeutable.
#	ADD_EXECUTABLE(mlnn_test tests/mlnn_test.cpp)
#	# Link it with shared libraries.
#	target_link_libraries(mlnn_test 
#		configuration
#		types
#		data_io
#		mlnn
#		${Boost_LIBRARIES}
#		)
#
#	# install test to bin directory
#	install(TARGETS mlnn_test RUNTIME DESTINATION bin)
#
#endif(${BUILD_TEST_MLNN})

# =======================================================================
# Build and install - MNIST recogntion based on CNNs.
# =======================================================================

#set(BUILD_MNIST_CNN_APP ON CACHE BOOL "Build the application using Convolutional Neural Network for recognition of MNIST digits")
#
#if(${BUILD_MNIST_CNN_APP})
#        # Create exeutable.
#        ADD_EXECUTABLE(mnist_convnet tests/mnist_convnet.cpp)
#        # Link it with shared libraries.
#        target_link_libraries(mnist_convnet
#			logger
#	        types
#			data_io
#			encoders
#			mlnn
#		    ${Boost_LIBRARIES}
#		    ${EIGEN_LIBRARIES}
#	    )
#        if(OpenBLAS_FOUND)
#                target_link_libraries(mnist_convnet  ${OpenBLAS_LIB} )
#        endif(OpenBLAS_FOUND)
#
#        # install test to bin directory
#        install(TARGETS mnist_convnet RUNTIME DESTINATION bin)
#
#endif(${BUILD_MNIST_CNN_APP})


## =======================================================================
## Build and install -  MNIST recogntion based on MLP.
## =======================================================================
#
#set(BUILD_MNIST_MLP_APP ON CACHE BOOL "Build the application using Multi-Layer Perceptron for recognition of MNIST digits")
#
#if(${BUILD_MNIST_MLP_APP})
#        # Create exeutable.
#        ADD_EXECUTABLE(mnist_simple_mlp_app tests/mnist_simple_mlp.cpp)
#        # Link it with shared libraries.
#        target_link_libraries(mnist_simple_mlp_app
#			logger
#	        types
#			data_io
#			encoders
#			mlnn
#	        ${Boost_LIBRARIES}
#	        ${EIGEN_LIBRARIES}
#	        )
#        if(OpenBLAS_FOUND)
#                target_link_libraries(mnist_simple_mlp_app  ${OpenBLAS_LIB} )
#        endif(OpenBLAS_FOUND)
#
#        # install test to bin directory
#        install(TARGETS mnist_simple_mlp_app RUNTIME DESTINATION bin)
#
#endif(${BUILD_MNIST_MLP_APP})

