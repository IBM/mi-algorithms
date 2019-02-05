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
# Install includes related to ENCODERS used by other libraries.
# =======================================================================

FILE(GLOB files *.hpp )
install(FILES ${files} DESTINATION include/encoders)

# Create shared library containing AUTO ENCODERS.
file(GLOB encoders_src *.cpp)
add_library(encoders SHARED ${encoders_src})
target_link_libraries(encoders logger ${Boost_LIBRARIES})

# Add to variable storing all libraries/targets.
set(MIAlgorithms_LIBRARIES ${MIAlgorithms_LIBRARIES} "encoders" CACHE INTERNAL "" FORCE)

# Install target library.
install(TARGETS encoders LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
