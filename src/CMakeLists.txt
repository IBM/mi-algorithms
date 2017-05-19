# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_ADD_INCLUDE_PATH} ${CMAKE_INSTALL_PREFIX}/include)
# Include current directory BEFORE the others - this will result in using LOCAL headers before the ones pointed by the CMAKE_INSTALL_PREFIX path!
# Include in here will result in enabling to use global headers paths e.g. <opengl/visualization/Window.hpp>
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

# Also add library paths
link_directories(${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_ADD_LIB_PATH})

# =======================================================================
# Add subdirectories
# =======================================================================

add_subdirectory(types)

add_subdirectory(data_utils)

add_subdirectory(data_io)

# Depricated - HTM.
add_subdirectory(auto_encoders)

add_subdirectory(encoders)
 
add_subdirectory(classifiers)

add_subdirectory(htm)

add_subdirectory(rbm)

# Not working.
#add_subdirectory(gru)
#add_subdirectory(lstm)

add_subdirectory(tests)

