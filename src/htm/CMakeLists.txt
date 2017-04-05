# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to HTM used by other libraries.
# =======================================================================

FILE(GLOB files *.h *.hpp)
install(FILES ${files} DESTINATION include/htm)

FILE(GLOB files shared/*.h shared/*.hpp)
install(FILES ${files} DESTINATION include/htm/shared)

# Create HTM shared library.
file(GLOB htm_src shared/*.cpp *.cpp)
add_library(htm SHARED ${htm_src})
target_link_libraries(htm types data_utils)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "htm" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS htm LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
