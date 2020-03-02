#include <gtest/gtest.h>

#include "AnalyzerController.hpp"
#include "FakeAnalyzerParam.hpp"
#include "Buffer.hpp"


class TestAnalyzerController : public testing::Test
{
public:
	Buffer<float> buffer;
	const int SIZE = 5;

	void SetUp() override
	{
		buffer = { 1, 2, 3, 4, 5 };
	}

	void TearDown() override
	{

	}
};


TEST_F(TestAnalyzerController, CreatesInstance)
{
	AnalyzerController<float> controller(buffer);
}

TEST_F(TestAnalyzerController, PerformsAnalysis)
{
	AnalyzerController<float> controller(buffer);
	FakeAnalyzerParam<float>* fakeAnalyzer2 = new FakeAnalyzerParam<float>(buffer);

	//AnalyzerController already has 2 instances
	controller.init_analyzer(fakeAnalyzer2);
	controller.set_analyzer(2);
	controller.set_index(2, 2);

	ASSERT_EQ(controller.analyze(), FACTOR1);
}
