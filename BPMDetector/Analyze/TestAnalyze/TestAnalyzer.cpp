#include <gtest/gtest.h>

#include "AnalyzerContainer.hpp"
#include "AnalyzerParam.hpp"
#include "Buffer.hpp"
#include "FakeAnalyzerParam.hpp"


class TestAnalyzerParam : public testing::Test
{
public:

};

class TestAnalyzerContainer : public testing::Test
{
public:
	Buffer<char> buffer;
	const int SIZE = 5;

	AnalyzerContainer container;
	FakeAnalyzerParam<char>* analyzerProc1;
	FakeAnalyzerParam<char>* analyzerProc2;

	void SetUp() override
	{
		buffer = { 1, 2, 3, 4, 5 };

		analyzerProc1 = new FakeAnalyzerParam<char>(buffer);
		analyzerProc2 = new FakeAnalyzerParam<char>(buffer, FACTOR2);
	}

	void TearDown() override
	{
		delete analyzerProc1;
		delete analyzerProc2;
	}
};


TEST_F(TestAnalyzerParam, InitializesParamList)
{
	AnalyzerParam analyzer
	{
    	new TypedParam<ParamType::Bool>("My bool", true),
    	new TypedParam<ParamType::Float>("My float", -3.1f),
    	new TypedParam<ParamType::Int>("My int", 55),
    	new TypedParam<ParamType::ConstChar>("My char", "My char param"),
    	new TypedParam<ParamType::String>("My string", "My string param")
	};
}

TEST_F(TestAnalyzerContainer, CreatesInstance)
{
	AnalyzerContainer container;
}


TEST_F(TestAnalyzerContainer, PerformsAnalysis)
{
	container.init_analyzer(analyzerProc1);

	ASSERT_EQ(container.analyze(), FACTOR1);
}

TEST_F(TestAnalyzerContainer, CorrectBehaviourUponParamFailure)
{
	analyzerProc1->set_state(State_ParamFailure);
	container.init_analyzer(analyzerProc1);

	ASSERT_EQ(container.analyze(), 0.0f);
}

TEST_F(TestAnalyzerContainer, ReturnsZeroIfNoAnalyzerConfigured)
{
	ASSERT_EQ(container.analyze(), 0.0f);
}

TEST_F(TestAnalyzerContainer, SelectsProperAnalyzer)
{
	container.init_analyzer(analyzerProc1);
	container.init_analyzer(analyzerProc2);

	container.set_analyzer(1);

	ASSERT_EQ(container.analyze(), FACTOR2);
}

TEST_F(TestAnalyzerContainer, SelectsProperAnalyzerIfInvalidIndex)
{
	container.init_analyzer(analyzerProc1);
	container.init_analyzer(analyzerProc2);

	container.set_analyzer(77);

	ASSERT_EQ(container.analyze(), FACTOR1);
}
