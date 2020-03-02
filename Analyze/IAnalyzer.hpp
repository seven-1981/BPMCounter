#ifndef _IANALYZER_H
#define _IANALYZER_H


class IAnalyzer
{
public:
	using FLOAT_TYPE = double;

	virtual ~IAnalyzer() { }

	virtual FLOAT_TYPE analyze() = 0;
};


class IAnalyzerTuple : public IAnalyzer
{
public:
	virtual ~IAnalyzerTuple() { }

	virtual FLOAT_TYPE analyze_rms() = 0;
};

#endif
