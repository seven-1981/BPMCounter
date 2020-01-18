#ifndef _ANALYZERCONTROLLER_H
#define _ANALYZERCONTROLLER_H

#include "IAnalyzer.hpp"
#include "AnalyzerContainer.hpp"
#include "Analyzer1.hpp"
#include "Buffer.hpp"


template <typename T>
class AnalyzerController : public IAnalyzer
{
public:
	AnalyzerController(Buffer<T>& buffer) : m_container()
	{
		m_container.init_analyzer(
			new Analyzer1<T>(buffer) );
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
		return m_container.analyze();
	}

private:
	AnalyzerContainer m_container;
};

#endif
