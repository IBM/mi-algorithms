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
# Install includes related to DATA UTILS used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp *.h)
install(FILES ${files} DESTINATION include/utils)

# Create shared library containing DATA UTILS.
file(GLOB data_utils_src *.cpp)
add_library(data_utils SHARED ${data_utils_src})
target_link_libraries(data_utils logger ${Boost_LIBRARIES} )

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "data_utils" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS data_utils LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
