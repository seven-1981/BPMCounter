#ifndef _ANALYZERCONTAINER_H
#define _ANALYZERCONTAINER_H

#include "IAnalyzer.hpp"
#include <vector>


class AnalyzerContainer : public IAnalyzer
{
public:
	using VEC = std::vector<IAnalyzer*>;
	using IT = VEC::iterator;

	AnalyzerContainer();
	~AnalyzerContainer();

	FLOAT_TYPE analyze() override;

	void init_analyzer(IAnalyzer* analyzer);
	void set_analyzer(unsigned int index);

	IT begin();
	IT end();
	void clear();

private:
	VEC m_analyzerList;
	unsigned int m_index;
};

#endif
