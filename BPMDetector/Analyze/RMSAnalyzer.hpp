#ifndef _RMSANALYZER_H
#define _RMSANALYZER_H

#include "AnalyzerParam.hpp"
#include "Buffer.hpp"
#include "DSP.hpp"
#include <iostream>


template <typename T>
class RMSAnalyzer : public AnalyzerParam
{
public:
	explicit RMSAnalyzer(Buffer<T>& buffer) :
		AnalyzerParam( {
			new TypedParam<ParamType::Double>("rms threshold", 2000.0L) } ),
		m_buffer(buffer),
		m_rmsThreshold(0)
	{
		m_initState = init_params();
	}

	~RMSAnalyzer() { }

	FLOAT_TYPE analyze() override
	{
		if (m_initState != 0) return 0.0;
		StopWatch watch;
		double rmsVal = DSP::get_rms_value(m_buffer);
		//watch.tic(std::cout);
		bool belowThreshold = (rmsVal < m_rmsThreshold);
		if (belowThreshold == true) rmsVal *= -1.0;
		return rmsVal;
	}

private:
	Buffer<T>& m_buffer;
	int m_initState;

	FLOAT_TYPE m_rmsThreshold;

	int init_params()
	{
		if (AnalyzerParam::get("rms threshold", m_rmsThreshold) == -1) return -1;
		return 0;
	}
};

#endif
