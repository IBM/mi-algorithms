# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to DATA UTILS used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp data_utils/*.h)
install(FILES ${files} DESTINATION include/data_utils)

# Create shared library containing DATA UTILS.
file(GLOB data_utils_src *.cpp)
add_library(data_utils SHARED ${data_utils_src})
target_link_libraries(data_utils types )

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "data_utils" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS data_utils LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
