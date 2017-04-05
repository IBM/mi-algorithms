# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to AUTO ENCODERS used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp )
install(FILES ${files} DESTINATION include/auto_encoders)

# Create shared library containing AUTO ENCODERS.
file(GLOB auto_encoders_src *.cpp)
add_library(auto_encoders SHARED ${auto_encoders_src})
target_link_libraries(auto_encoders types)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "auto_encoders" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS auto_encoders LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
