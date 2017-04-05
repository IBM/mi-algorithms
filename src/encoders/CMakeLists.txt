# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to ENCODERS used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp )
install(FILES ${files} DESTINATION include/encoders)

# Create shared library containing AUTO ENCODERS.
file(GLOB encoders_src *.cpp)
add_library(encoders SHARED ${encoders_src})
target_link_libraries(encoders types)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "encoders" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS encoders LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
