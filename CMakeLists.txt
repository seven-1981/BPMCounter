cmake_minimum_required(VERSION 2.6)

# Set project path
set(PROJ_PATH "/home/pi/eclipse-workspace/BPMDetector")

# Set include directories
list(APPEND includes "${PROJ_PATH}"
		     		 "${PROJ_PATH}/Audio"
		     		 "${PROJ_PATH}/Audio/Wavfiles"
		     		 "${PROJ_PATH}/Analyze"
		     		 "${PROJ_PATH}/Gui")

# Locate sources
list(APPEND sources "${PROJ_PATH}/Audio/AudioCardLister.cpp"
		    		"${PROJ_PATH}/Audio/ALSACardInfoGetter.cpp"
				    "${PROJ_PATH}/Audio/AudioCardConfigurator.cpp"
				    "${PROJ_PATH}/Audio/AudioCardManager.cpp"
				    "${PROJ_PATH}/Audio/AudioRecorder.cpp"
				    "${PROJ_PATH}/Audio/ALSARecordingService.cpp"
					"${PROJ_PATH}/Audio/ALSACardConfigProcedure.cpp"
				    "${PROJ_PATH}/Audio/Wavfiles/WavFile.cpp"
				    "${PROJ_PATH}/Audio/AudioController.cpp"
				    "${PROJ_PATH}/Gui/MainWindow.cpp"
				    "${PROJ_PATH}/Gui/FrequencySpectrum.cpp"
				    "${PROJ_PATH}/Gui/Spectrograph.cpp"
				    "${PROJ_PATH}/Analyze/ParamList.cpp"
				    "${PROJ_PATH}/Analyze/AnalyzerParam.cpp"
				    "${PROJ_PATH}/Analyze/AnalyzerContainer.cpp")

# Locate ALSA
find_package(ALSA REQUIRED) 
include_directories(${ALSA_INCLUDE_DIRS} ${includes})  

# Locate Qt
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Qwt includes
include_directories("/usr/include/qwt")
# Qwt library
link_directories("/usr/lib")
set(CMAKE_AUTOMOC ON)

# Link application
add_executable(BPMDetector main.cpp ${sources})
target_link_libraries(BPMDetector pthread ${ALSA_LIBRARIES})
target_link_libraries(BPMDetector Qt5::Widgets)  
target_link_libraries(BPMDetector qwt-qt5)      

# Set compiler flags
list(APPEND flags "-Wall" "-std=c++11" "-g")
target_compile_options(BPMDetector PUBLIC ${flags})

