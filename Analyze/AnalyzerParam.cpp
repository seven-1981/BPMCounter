#include "AnalyzerParam.hpp"


AnalyzerParam::AnalyzerParam(LIST list) :
	ParamList(list) { }

AnalyzerParam::~AnalyzerParam()
{
	using IT = ParamList::iterator;
	for (IT it = ParamList::begin(); it != ParamList::end(); ++it)
		delete *it;
	ParamList::clear();
}

AnalyzerParam::FLOAT_TYPE AnalyzerParam::analyze()
{
	return 0.0;
}
