# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to CLASSIFIERS used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp)
install(FILES ${files} DESTINATION include/classifiers)

# Create shared library containing CLASSIFIERS.
file(GLOB classifiers_src *.cpp)
add_library(classifiers SHARED ${classifiers_src})
target_link_libraries(classifiers types data_utils)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "classifiers" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS classifiers LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
