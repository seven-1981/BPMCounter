#include <gtest/gtest.h>
#include "Analyzer.hpp"
#include "BPMAnalyzer.hpp"
#include "TestProcedure.hpp"
#include "Buffer.hpp"


class TestAnalyzer : public testing::Test
{
public:

};

class TestBPMAnalyzer : public testing::Test
{
public:
	Buffer<char> buffer;
	const int SIZE = 5;

	void SetUp() override
	{
		buffer = { 1, 2, 3, 4, 5 };
	}
};


TEST_F(TestAnalyzer, InitializesParamList)
{
	Analyzer analyzer
	{
    	new TypedParam<ParamType::Bool>("My bool", true),
    	new TypedParam<ParamType::Float>("My float", -3.1f),
    	new TypedParam<ParamType::Int>("My int", 55),
    	new TypedParam<ParamType::ConstChar>("My char", "My char param"),
    	new TypedParam<ParamType::String>("My string", "My string param")
	};
}

TEST_F(TestBPMAnalyzer, CreatesInstance)
{
	BPMAnalyzer analyzer;
}


TEST_F(TestBPMAnalyzer, PerformsAnalysis)
{
	BPMAnalyzer analyzer;
	TestProcedure<char> procedure(buffer);
	analyzer.set_procedure(&procedure);

	ASSERT_EQ(analyzer.analyze(), FACTOR);
}

TEST_F(TestBPMAnalyzer, CorrectBehaviourUponParamFailure)
{
	BPMAnalyzer analyzer;
	TestProcedure<char> procedure(buffer);
	procedure.set_state(State_ParamFailure);
	analyzer.set_procedure(&procedure);

	ASSERT_EQ(analyzer.analyze(), 0.0f);
}
