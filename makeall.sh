#!/bin/bash

# Get the project path
PROJ_PATH=$(git rev-parse --show-toplevel)
echo Current project path = $PROJ_PATH

# Reset the build (erase the build directory)
reset_build() {
BUILD_DIR=$1
cd $BUILD_DIR
rm -r build
mkdir build
cd build
}

# Check if a clean command was issued
if [ "$1" == "clean" ]; then
	echo Cleaning BPMDetector...
	reset_build ..
	exit 0
fi

# Start the build process
echo Building BPMDetector...
echo Build Audio tests...
reset_build ${PROJ_PATH}/Audio/TestAudio
cmake ..
if cmake --build .; then
./runTests

echo Build Analyze tests...
reset_build ${PROJ_PATH}/Analyze/TestAnalyze
cmake ..
if cmake --build .; then
./runTests

echo Build Plot...
reset_build ${PROJ_PATH}/Plot
cmake ..
if cmake --build .; then
echo Successfully built Plot.

echo Build Application...
reset_build ${PROJ_PATH}
cmake ..
if cmake --build .; then
echo Successfully built application.

fi
fi
fi
fi