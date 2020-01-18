#include <gtest/gtest.h>

#include "AnalyzerController.hpp"
#include "Buffer.hpp"


class TestAnalyzerController : public testing::Test
{
public:
	Buffer<char> buffer;
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
	AnalyzerController<char> controller(buffer);
}

TEST_F(TestAnalyzerController, PerformsAnalysis)
{
	AnalyzerController<char> controller(buffer);

	ASSERT_EQ(controller.analyze(), 1.0f);
}
