# Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

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

add_subdirectory(mlnn)

# Not working.
#add_subdirectory(gru)
#add_subdirectory(lstm)

add_subdirectory(tests)

