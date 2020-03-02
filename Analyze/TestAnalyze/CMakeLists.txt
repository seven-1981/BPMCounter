cmake_minimum_required(VERSION 2.6)

# Set project path
set(PROJ_PATH "/home/pi/eclipse-workspace/BPMDetector")

# Set include directories
list(APPEND includes "${PROJ_PATH}"
					 "${PROJ_PATH}/Audio"
		     		 "${PROJ_PATH}/Analyze"
		     		 "${PROJ_PATH}/Analyze/TestAnalyze")

# Locate sources
list(APPEND sources "${PROJ_PATH}/Analyze/ParamList.cpp"
					"${PROJ_PATH}/Analyze/AnalyzerParam.cpp"
					"${PROJ_PATH}/Analyze/AnalyzerContainer.cpp")

include_directories(${includes})
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS}) 

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests TestMain.cpp ${sources})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)                 

# Set compiler flags
list(APPEND flags "-Wall" "-std=c++11" "-g")
target_compile_options(runTests PUBLIC ${flags})
