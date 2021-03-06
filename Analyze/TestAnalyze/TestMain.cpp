#include <gtest/gtest.h>
#include "TestDSP.cpp"
#include "TestParam.cpp"
#include "TestParamList.cpp"
#include "TestAnalyzer.cpp"
#include "TestAnalyzerController.cpp"
#include "TestAveraging.cpp"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
