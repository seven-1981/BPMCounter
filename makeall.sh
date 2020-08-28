#!/bin/bash
reset_build() {
echo Dir=$1
BUILD_DIR=$1
cd $BUILD_DIR
rm -r build
mkdir build
cd build
}

if [ "$1" == "clean" ]; then
	echo Cleaning BPMDetector...
	reset_build ..
	exit 0
fi

echo Building BPMDetector...
echo Build Audio tests...
reset_build Audio/TestAudio
cmake ..
if cmake --build .; then
./runTests

echo Build Analyze tests...
reset_build ../../../Analyze/TestAnalyze
cmake ..
if cmake --build .; then
./runTests

echo Build Plot...
reset_build ../../../Plot
cmake ..
if cmake --build .; then
echo Successfully built Plot.

echo Build Application...
reset_build ../..
cmake ..
if cmake --build .; then
echo Successfully built application.

fi
fi
fi
fi