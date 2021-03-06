#include <gtest/gtest.h>
#include "TestAudioCardLister.cpp"
#include "TestAudioCardFilter.cpp"
#include "TestAudioCardManager.cpp"
#include "TestBuffer.cpp"
#include "TestAudioRecorder.cpp"
#include "TestAudioCardConfigurator.cpp"
#include "TestWavFile.cpp"
#include "TestWavFileWriter.cpp"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
