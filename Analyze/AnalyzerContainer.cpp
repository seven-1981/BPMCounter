#include "AnalyzerContainer.hpp"


AnalyzerContainer::AnalyzerContainer() : m_analyzerList(), m_index(0) { }

AnalyzerContainer::~AnalyzerContainer() { }

AnalyzerContainer::FLOAT_TYPE AnalyzerContainer::analyze()
{
	if (m_analyzerList.size() == 0)
		return 0.0;
	if (m_analyzerList.at(m_index) == nullptr)
		return 0.0;
	return m_analyzerList.at(m_index)->analyze();
}

void AnalyzerContainer::init_analyzer(IAnalyzer* analyzer)
{
	m_analyzerList.push_back(analyzer);
}

void AnalyzerContainer::set_analyzer(unsigned int index)
{
	if (index < m_analyzerList.size())
		m_index = index;
}

AnalyzerContainer::IT AnalyzerContainer::begin()
{
	return m_analyzerList.begin();
}

AnalyzerContainer::IT AnalyzerContainer::end()
{
	return m_analyzerList.end();
}

void AnalyzerContainer::clear()
{
	m_analyzerList.clear();
}

