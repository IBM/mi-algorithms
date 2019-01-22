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
