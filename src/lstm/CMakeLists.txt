# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to LOGGER used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp)
install(FILES ${files} DESTINATION include/lstm)
  
# Create shared library containing LOGGER used by all other libraries.
file(GLOB lstm_src *.cpp)
add_library(lstm SHARED ${lstm_src})
target_link_libraries(lstm ${Boost_LIBRARIES} )

# Add to variable storing all libraries/targets.
set(MIToolchain_LIBRARIES ${MIToolchain_LIBRARIES} "lstm" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS lstm LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)

# Install target library.
install(TARGETS lstm LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
