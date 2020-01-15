#ifndef _TESTPROCEDURE_H
#define _TESTPROCEDURE_H

#include <string>
#include "Analyzer.hpp"
#include "Buffer.hpp"
#include "DSP.hpp"


const float FACTOR = 33.3f;

//States used in this mock
enum TestProcedureStates_e
{
	State_Success,		//Procedure executed successfully
	State_ParamFailure 	//Error upon param retrieval
};


template <typename T>
class TestProcedure : public Analyzer
{
public:
	using FLOAT_TYPE = Analyzer::FLOAT_TYPE;

	explicit TestProcedure(Buffer<T>& buffer) :
		Analyzer( {
			new TypedParam<ParamType::Float>("factor", FACTOR) } ),
		m_buffer(buffer),
		m_state(State_Success),
		m_paramName("factor") { }

	FLOAT_TYPE analyze() override
	{
		FLOAT_TYPE test = 0.0;
		if (Analyzer::ParamList::get(m_paramName, test) == -1) return 0.0;
		DSP::gain(m_buffer, test);
		return test;
	}

	void set_state(TestProcedureStates_e state)
	{
		m_state = state;
		if (state == State_ParamFailure)
			m_paramName = "something else";
		else
			m_paramName = "factor";
	}

private:
	Buffer<T>& m_buffer;
	TestProcedureStates_e m_state;
	std::string m_paramName;
};

#endif
