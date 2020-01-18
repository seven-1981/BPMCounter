#ifndef _ANALYZER1_H
#define _ANALYZER1_H

#include "AnalyzerParam.hpp"
#include "Buffer.hpp"
#include "DSP.hpp"


template <typename T>
class Analyzer1 : public AnalyzerParam
{
public:
	explicit Analyzer1(Buffer<T>& buffer) :
		AnalyzerParam( {
			new TypedParam<ParamType::Int>("downsample factor", 4)
		} ), m_buffer(buffer),
		m_origSize(0),
		m_dsFactor(1),
		m_dsSize(0)
	{
		m_initState = init_params();
	}

	FLOAT_TYPE analyze() override
	{
		if (m_initState != 0) return 0.0;
		Buffer<T> bf_ds(m_dsSize);
		DSP::resample_buffer(m_buffer, bf_ds);
		return DSP::get_mean_value(m_buffer);
	}

private:
	Buffer<T>& m_buffer;
	int m_initState;

	unsigned int m_origSize;
	int m_dsFactor;
	unsigned int m_dsSize;

	int init_params()
	{
		m_origSize = m_buffer.get_size();
		if (AnalyzerParam::get("downsample factor", m_dsFactor) == -1) return -1;
		m_dsSize = m_origSize / static_cast<unsigned int>(m_dsFactor);
		return 0;
	}
};

#endif
