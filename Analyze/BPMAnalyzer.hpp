#ifndef _BPMANALYZER_H
#define _BPMANALYZER_H

#include "IAnalyzer.hpp"


class BPMAnalyzer :  public IAnalyzer
{
public:
	BPMAnalyzer() : m_analyzer(nullptr) { }
	~BPMAnalyzer() { }

	FLOAT_TYPE analyze()
	{
		return m_analyzer->analyze();
	}

	void set_procedure(IAnalyzer* analyzer)
	{
		m_analyzer = analyzer;
	}

	template <typename T>
	void set_buffer(Buffer<T>& buffer) { }

private:
	IAnalyzer* m_analyzer;
};

#endif
