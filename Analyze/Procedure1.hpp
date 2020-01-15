#ifndef _PROCEDURE1_H
#define _PROCEDURE1_H

#include "Analyzer.hpp"
#include "Buffer.hpp"
#include "DSP.hpp"


template <typename T>
class Procedure1 : public Analyzer
{
public:
	using FLOAT_TYPE = Analyzer::FLOAT_TYPE;

	explicit Procedure1(Buffer<T>& buffer) : m_buffer(buffer),
		Analyzer( {
			new TypedParam<ParamType::Float>("first", 33.3f)
		} ) { }

	FLOAT_TYPE analyze() override
	{
		FLOAT_TYPE test = 0.0;
		if (Analyzer::ParamList::get("first", test) == -1) return 0.0;
		DSP::gain(m_buffer, test);
		return test;
	}

private:
	Buffer<T>& m_buffer;
};

#endif
