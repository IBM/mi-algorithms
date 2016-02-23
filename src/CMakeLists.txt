# Check compiler flags
include(CheckCXXCompilerFlag)

# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Set compiler/linker flags.
# =======================================================================

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Check, whether all necessary libraries are linked
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl") 

# =======================================================================
# Add CMAKE variables.
# =======================================================================

set(BUILD_TEST_CHAR_ENCODER ON CACHE BOOL "Build the 1-of-k Char Encoder test application")

set(BUILD_TEST_MATRIX ON CACHE BOOL "Build the application for testing matrix multiplication")

set(BUILD_TEST_COLLECTOR ON CACHE BOOL "Build the application for testing data collector")


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
target_link_libraries(types logger ${Boost_LIBRARIES}  ${OpenBLAS_LIB} )


# =======================================================================
# Install includes related to DATA UTILS used by other libraries.
FILE(GLOB files data_utils/*.hpp data_utils/*.h)
install(FILES ${files} DESTINATION include/data_utils)

# Create shared library containing DATA UTILS.
file(GLOB data_utils_src data_utils/*.cpp)
add_library(data_utils SHARED ${data_utils_src})
target_link_libraries(data_utils types )

# =======================================================================
# Install includes related to DATA IO used by other libraries.
FILE(GLOB files data_io/*.hpp)
install(FILES ${files} DESTINATION include/data_io)

# Create shared library containing DATA IO.
file(GLOB data_io_src data_io/*.cpp)
add_library(data_io SHARED ${data_io_src})
target_link_libraries(data_io system_utils types data_utils configuration)

# =======================================================================
# Install includes related to AUTO ENCODERS used by other libraries.
FILE(GLOB files auto_encoders/*.hpp)
install(FILES ${files} DESTINATION include/auto_encoders)

# Create shared library containing AUTO ENCODERS.
file(GLOB auto_encoders_src auto_encoders/*.cpp)
add_library(auto_encoders SHARED ${auto_encoders_src})
target_link_libraries(auto_encoders types )

# =======================================================================
# Install includes related to CLASSIFIERS used by other libraries.
FILE(GLOB files classifiers/*.hpp)
install(FILES ${files} DESTINATION include/classifiers)

# Create shared library containing CLASSIFIERS.
file(GLOB classifiers_src classifiers/*.cpp)
add_library(classifiers SHARED ${classifiers_src})
target_link_libraries(classifiers system_utils types data_utils )

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

# =======================================================================
# Install includes related to RBM used by other libraries.
FILE(GLOB files rbm/*.hpp)
install(FILES ${files} DESTINATION include/rbm)

# Create RBM shared library.
file(GLOB rbm_src rbm/*.cpp )
add_library(rbm SHARED ${rbm_src})
target_link_libraries(rbm types data_utils ${OpenBLAS_LIB} )


# =======================================================================
# Install target libraries.
# =======================================================================

install(TARGETS types data_io data_utils auto_encoders classifiers htm rbm LIBRARY DESTINATION lib)

# =======================================================================
# Build and install - char encoder test application.
# =======================================================================

if(${BUILD_TEST_CHAR_ENCODER})
	# Create exeutable.
	add_executable(char_encoder_test tests/char_encoder_test.cpp)
	# Link it with shared libraries.
	target_link_libraries(char_encoder_test 
		logger
		application
		types
		data_io
		auto_encoders
		configuration
		${Boost_LIBRARIES}
		)

	# install test to bin directory
	install(TARGETS char_encoder_test RUNTIME DESTINATION bin)

endif(${BUILD_TEST_CHAR_ENCODER})


# =======================================================================
# Build and install - matrix test application.
# =======================================================================

if(${BUILD_TEST_MATRIX})
	# Create exeutable.
	ADD_EXECUTABLE(matrix_test tests/matrix_test.cpp)
	# Link it with shared libraries.
	target_link_libraries(matrix_test 
		types
		${Boost_LIBRARIES}
		${EIGEN_LIBRARIES} 
		${OpenBLAS_LIB}
		)

	# install test to bin directory
	install(TARGETS matrix_test RUNTIME DESTINATION bin)

endif(${BUILD_TEST_MATRIX})

# =======================================================================
# Build and install - data collector test application.
# =======================================================================

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
