#ifndef _ANALYZER_H
#define _ANALYZER_H

#include "IAnalyzer.hpp"
#include "ParamList.hpp"


class Analyzer : protected ParamList, public IAnalyzer
{
public:
	using LIST = ParamList::LIST;

	explicit Analyzer(LIST list) : ParamList(list) { }

	~Analyzer()
	{
		using IT = ParamList::iterator;
		for (IT it = ParamList::begin(); it != ParamList::end(); ++it)
			delete *it;
		ParamList::clear();
	}

	FLOAT_TYPE analyze() override
	{
		return 0.0;
	}
};

#endif
