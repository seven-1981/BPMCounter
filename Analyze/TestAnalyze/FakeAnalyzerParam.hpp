#ifndef _FAKEANALYZERPARAM_H
#define _FAKEANALYZERPARAM_H

#include <string>

#include "AnalyzerParam.hpp"
#include "Buffer.hpp"
#include "DSP.hpp"


const float FACTOR1 = 33.3f;
const float FACTOR2 = 66.6f;

//States used in this mock
enum TestProcedureStates_e
{
	State_Success,		//Procedure executed successfully
	State_ParamFailure 	//Error upon param retrieval
};


template <typename T>
class FakeAnalyzerParam : public AnalyzerParam
{
public:
	explicit FakeAnalyzerParam(Buffer<T>& buffer, FLOAT_TYPE value = FACTOR1) :
		AnalyzerParam( {
			new TypedParam<ParamType::Float>("factor", value) } ),
		m_buffer(buffer),
		m_state(State_Success),
		m_paramName("factor") { }

	FLOAT_TYPE analyze() override
	{
		FLOAT_TYPE test = 0.0;
		if (AnalyzerParam::ParamList::get(m_paramName, test) == -1) return 0.0;
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
