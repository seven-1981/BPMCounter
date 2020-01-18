#ifndef _ANALYZERPARAM_H
#define _ANALYZERPARAM_H

#include "IAnalyzer.hpp"
#include "ParamList.hpp"


class AnalyzerParam : protected ParamList, public IAnalyzer
{
public:
	using LIST = ParamList::LIST;

	explicit AnalyzerParam(LIST list);

	~AnalyzerParam();

	FLOAT_TYPE analyze() override;
};

#endif
