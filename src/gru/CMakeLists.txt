# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to GRU used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp)
install(FILES ${files} DESTINATION include/gru)
  
# Create shared library containing LOGGER used by all other libraries.
file(GLOB gru_src *.cpp)
add_library(gru SHARED ${gru_src})
target_link_libraries(gru ${Boost_LIBRARIES} )

# Add to variable storing all libraries/targets.
set(MIToolchain_LIBRARIES ${MIToolchain_LIBRARIES} "gru" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS gru LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
