#ifndef _ANALYZERCONTROLLER_H
#define _ANALYZERCONTROLLER_H

#include "IAnalyzer.hpp"
#include "AnalyzerContainer.hpp"
#include "BPMAnalyzer.hpp"
#include "RMSAnalyzer.hpp"
#include "Buffer.hpp"


template <typename T>
class AnalyzerController : public IAnalyzerTuple
{
public:
	AnalyzerController(Buffer<T>& buffer) :
		m_container(),
		m_rmsValue(0),
		m_indexRms(0),
		m_indexBpm(1)
	{
		init_analyzer( new RMSAnalyzer<T>(buffer) );
		init_analyzer( new BPMAnalyzer<T>(buffer) );
	}

	~AnalyzerController()
	{
		using IT = AnalyzerContainer::VEC::iterator;
		for (IT it = m_container.begin(); it != m_container.end(); ++it)
			delete *it;
		m_container.clear();
	}

	FLOAT_TYPE analyze() override
	{
		//Determine RMS value - if below threshold, no detection is performed
		set_analyzer(m_indexRms);
		m_rmsValue = m_container.analyze();
		//if (m_rmsValue < 0)	return 0.0;
		set_analyzer(m_indexBpm);
		return m_container.analyze();
	}

	FLOAT_TYPE analyze_rms() override
	{
		return m_rmsValue;
	}

	void init_analyzer(IAnalyzer* analyzer)
	{
		m_container.init_analyzer(analyzer);
	}

	void set_analyzer(unsigned int index)
	{
		m_container.set_analyzer(index);
	}

	void set_index(int rmsIndex, int bpmIndex)
	{
		m_indexRms = rmsIndex;
		m_indexBpm = bpmIndex;
	}

private:
	AnalyzerContainer m_container;

	FLOAT_TYPE m_rmsValue;
	int m_indexRms;
	int m_indexBpm;
};

#endif
