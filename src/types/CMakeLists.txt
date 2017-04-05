# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to CORE TYPES used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp )
install(FILES ${files} DESTINATION include/types)

# Create shared library containing CORE TYPES and related functions.
file(GLOB types_src *.cpp)
add_library(types SHARED ${types_src})
target_link_libraries(types logger ${Boost_LIBRARIES} )
if(OpenBLAS_FOUND)
	target_link_libraries(types  ${OpenBLAS_LIB} )
endif(OpenBLAS_FOUND)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "types" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS types LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
