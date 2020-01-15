cmake_minimum_required(VERSION 2.6)

# Set project path
set(PROJ_PATH "/home/pi/eclipse-workspace/BPMDetector")

# Set include directories
list(APPEND includes "${PROJ_PATH}"
		     "${PROJ_PATH}/Analyze"
		     "${PROJ_PATH}/Audio")

# Locate sources
list(APPEND sources "${PROJ_PATH}/Plot/main.cpp")

# Locate ALSA
include_directories(${includes})  

# Link application
add_executable(Plot main.cpp ${sources})
target_link_libraries(Plot pthread)
target_link_libraries(Plot boost_iostreams boost_system boost_filesystem)       

# Set compiler flags
list(APPEND flags "-Wall" "-std=c++11" "-g" "-Wno-psabi")
target_compile_options(Plot PUBLIC ${flags})           
