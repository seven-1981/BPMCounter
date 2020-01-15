#ifndef _IANALYZER_H
#define _IANALYZER_H


class IAnalyzer
{
public:
	using FLOAT_TYPE = float;

	virtual ~IAnalyzer() { }

	virtual FLOAT_TYPE analyze() = 0;
};

#endif
