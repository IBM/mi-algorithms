# Check compiler flags
include(CheckCXXCompilerFlag)

# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Set compiler flags.
# =======================================================================

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Check, whether all necessary libraries are linked
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl") 

# =======================================================================
# Build MI Algorithms.
# =======================================================================

# =======================================================================
# Install includes related to CORE TYPES UTILS used by other libraries.
FILE(GLOB files core_types/*.hpp core_types/*.h)
install(FILES ${files} DESTINATION include/core_types)

# Create shared library containing CORE TYPES and related functions.
file(GLOB core_types_src types/*.cpp)
add_library(core_types SHARED ${core_types_src})
target_link_libraries(core_types ${Boost_LIBRARIES} ${MIToolchain_LIBRARIES} )


# =======================================================================
# Install includes related to DATA UTILS used by other libraries.
FILE(GLOB files data_utils/*.hpp)
install(FILES ${files} DESTINATION include/data_utils)

# Create shared library containing DATA UTILS.
file(GLOB data_utils_src data_utils/*.cpp)
add_library(data_utils SHARED ${data_utils_src})
target_link_libraries(data_utils core_types configuration)

# =======================================================================
# Install includes related to DATA IO used by other libraries.
FILE(GLOB files data_io/*.hpp)
install(FILES ${files} DESTINATION include/data_io)

# Create shared library containing DATA IO.
file(GLOB data_io_src data_io/*.cpp)
add_library(data_io SHARED ${data_io_src})
target_link_libraries(data_io system_utils core_types data_utils configuration)

# =======================================================================
# Install includes related to AUTO ENCODERS used by other libraries.
FILE(GLOB files auto_encoders/*.hpp)
install(FILES ${files} DESTINATION include/auto_encoders)

# Create shared library containing AUTO ENCODERS.
file(GLOB auto_encoders_src auto_encoders/*.cpp)
add_library(auto_encoders SHARED ${auto_encoders_src})
target_link_libraries(auto_encoders core_types configuration)

# =======================================================================
# Install includes related to CLASSIFIERS used by other libraries.
FILE(GLOB files classifiers/*.hpp)
install(FILES ${files} DESTINATION include/classifiers)

# Create shared library containing CLASSIFIERS.
file(GLOB classifiers_src classifiers/*.cpp)
add_library(classifiers SHARED ${classifiers_src})
target_link_libraries(classifiers system_utils core_types data_utils configuration)

# =======================================================================
# Install includes related to HTM used by other libraries.
FILE(GLOB files htm/*.hpp)
install(FILES ${files} DESTINATION include/htm)

# Create HTM shared library.
file(GLOB htm_src htm/shared/*.cpp htm/*.cpp)
add_library(htm SHARED ${htm_src})
target_link_libraries(htm core_types data_utils)

# =======================================================================
# Install includes related to RBM used by other libraries.
FILE(GLOB files rbm/*.hpp)
install(FILES ${files} DESTINATION include/rbm)

# Create RBM shared library.
file(GLOB rbm_src rbm/*.cpp )
add_library(rbm SHARED ${rbm_src})
target_link_libraries(rbm core_types data_utils ${OpenBLAS_LIB})


# =======================================================================
# Install target libraries.
# =======================================================================

install(TARGETS  LIBRARY DESTINATION lib)
