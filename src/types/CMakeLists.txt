# Copyright (C) tkornuta, IBM Corporation 2015-2019
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =======================================================================
# Install includes related to CORE TYPES used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp )
install(FILES ${files} DESTINATION include/types)

# Create shared library containing CORE TYPES and related functions.
set(types_src 
	image_types.cpp
	)
add_library(types SHARED ${types_src})
target_link_libraries(types logger ${Boost_LIBRARIES} )
if(OpenBLAS_FOUND)
	target_link_libraries(types  ${OpenBLAS_LIB} )
endif(OpenBLAS_FOUND)

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "types" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS types LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)

# =======================================================================
# Build matrix tests
# =======================================================================

# Link tests with GTest
if(GTEST_FOUND AND BUILD_UNIT_TESTS)

	add_executable(unit_tests_matrix MatrixTests.cpp)
	target_link_libraries(unit_tests_matrix
		${GTEST_LIBRARIES}
		${Boost_LIBRARIES}
		)
	if(OpenBLAS_FOUND)
		target_link_libraries(unit_tests_matrix  ${OpenBLAS_LIB} )
	endif(OpenBLAS_FOUND)
		
	add_test(unit_tests_matrix ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unit_tests_matrix)

	install(TARGETS unit_tests_matrix LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)

endif(GTEST_FOUND AND BUILD_UNIT_TESTS)


# =======================================================================
# Build matrix array tests
# =======================================================================

# Link tests with GTest
if(GTEST_FOUND AND BUILD_UNIT_TESTS)

	add_executable(unit_tests_matrix_array MatrixArrayTests.cpp)
	target_link_libraries(unit_tests_matrix_array
		${GTEST_LIBRARIES}
		${Boost_LIBRARIES}
		)
	if(OpenBLAS_FOUND)
		target_link_libraries(unit_tests_matrix_array  ${OpenBLAS_LIB} )
	endif(OpenBLAS_FOUND)
		
	add_test(unit_tests_matrix_array ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unit_tests_matrix_array)

	install(TARGETS unit_tests_matrix_array LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)

endif(GTEST_FOUND AND BUILD_UNIT_TESTS)


# =======================================================================
# Build tensor tests
# =======================================================================

# Link tests with GTest
if(GTEST_FOUND AND BUILD_UNIT_TESTS)

	add_executable(unit_tests_tensor TensorTests.cpp)
	target_link_libraries(unit_tests_tensor
		${GTEST_LIBRARIES}
		${Boost_LIBRARIES}
		)
	if(OpenBLAS_FOUND)
		target_link_libraries(unit_tests_tensor  ${OpenBLAS_LIB} )
	endif(OpenBLAS_FOUND)
		
	add_test(unit_tests_tensor ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unit_tests_tensor)

	install(TARGETS unit_tests_tensor LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)

endif(GTEST_FOUND AND BUILD_UNIT_TESTS)


