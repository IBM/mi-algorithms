# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to DATA IO used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp)
install(FILES ${files} DESTINATION include/data_io)

# Create shared library containing DATA IO.
file(GLOB data_io_src *.cpp)
add_library(data_io SHARED ${data_io_src})
target_link_libraries(data_io types data_utils configuration)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "data_io" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS data_io LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
