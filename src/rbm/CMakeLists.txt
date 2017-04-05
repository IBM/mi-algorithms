# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to RBM used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp)
install(FILES ${files} DESTINATION include/rbm)

# Create RBM shared library.
file(GLOB rbm_src *.cpp )
add_library(rbm SHARED ${rbm_src})
target_link_libraries(rbm types data_utils)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "rbm" CACHE INTERNAL "" FORCE)
