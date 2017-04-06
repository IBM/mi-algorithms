# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Get list of subdirectories.
FILE(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

# First, add "main" directory
SET(mlnn_INCLUDE_DIRS ".")
FOREACH(child ${children})
    IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
        LIST(APPEND mlnn_INCLUDE_DIRS ${child})
    ENDIF()
ENDFOREACH()

# Iterate through subdirectories...
foreach (dir ${mlnn_INCLUDE_DIRS})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${dir})
    # Get list of headers.
    FILE(GLOB mlnn_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/*.hpp)
    # Install headers.
    install(FILES ${mlnn_HEADERS} DESTINATION include/mlnn/${dir})
endforeach()


# Create MLNN shared library using all cpp files (recursive).
FILE(GLOB_RECURSE mlnn_SRC *.cpp)
add_library(mlnn SHARED
	${mlnn_SRC}
	)

# Link MLNN with required libraries. 
target_link_libraries(mlnn
	types
	)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "mlnn" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS mlnn LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)

