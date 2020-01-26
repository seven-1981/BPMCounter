#ifndef _IANALYZER_H
#define _IANALYZER_H


class IAnalyzer
{
public:
	using FLOAT_TYPE = double;

	virtual ~IAnalyzer() { }

	virtual FLOAT_TYPE analyze() = 0;
};

#endif
