#ifndef _ANALYZER1_H
#define _ANALYZER1_H

#include "AnalyzerParam.hpp"
#include "Buffer.hpp"
#include "DSP.hpp"


template <typename T>
class Analyzer1 : public AnalyzerParam
{
public:
	explicit Analyzer1(Buffer<T>& buffer, unsigned int sample_rate_Hz = 44100) :
		AnalyzerParam( {
			new TypedParam<ParamType::Float>("downsample cutoff", 5500.0f),
			new TypedParam<ParamType::Int>("downsample factor", 4),
			new TypedParam<ParamType::Float>("envelope filter recovery", 0.005f),
			new TypedParam<ParamType::Int>("autocorr resolution", 1200),
			new TypedParam<ParamType::Float>("bpm min value", 80.0f),
			new TypedParam<ParamType::Float>("bpm max value", 200.0f) } ),
		m_buffer(buffer),
		m_sample_rate_Hz(sample_rate_Hz),
		m_origSize(0),
		m_cutoffFreq(0),
		m_dsFactor(1),
		m_dsSize(0),
		m_env_filt_rec(0),
		m_autocorrRes(0),
		m_bpm_min(0),
		m_bpm_max(0)
	{
		m_initState = init_params();
	}

	FLOAT_TYPE analyze() override
	{
		if (m_initState != 0) return 0.0;

		FLOAT_TYPE dt = 1.0f / static_cast<FLOAT_TYPE>(m_sample_rate_Hz);
		Buffer<T> bf_lp(m_dsSize);
		DSP::lowpass_filter(m_buffer, bf_lp, m_cutoffFreq, dt);
		Buffer<T> bf_ds(m_dsSize);
		DSP::resample_buffer(bf_lp, bf_ds);
		Buffer<T> bf_ef(m_dsSize);
		DSP::envelope_filter(bf_ds, bf_ef, m_env_filt_rec, m_sample_rate_Hz);
		Buffer<FLOAT_TYPE> bf_ac(m_autocorrRes);
		DSP::autocorr_bpm(bf_ef, bf_ac, m_sample_rate_Hz, m_bpm_min, m_bpm_max);
		return DSP::extract_bpm_value(bf_ac, m_bpm_min, m_bpm_max);
	}

private:
	Buffer<T>& m_buffer;
	unsigned int m_sample_rate_Hz;
	int m_initState;

	unsigned int m_origSize;
	FLOAT_TYPE m_cutoffFreq;
	int m_dsFactor;
	unsigned int m_dsSize;
	FLOAT_TYPE m_env_filt_rec;
	int m_autocorrRes;
	FLOAT_TYPE m_bpm_min;
	FLOAT_TYPE m_bpm_max;

	int init_params()
	{
		m_origSize = m_buffer.get_size();
		if (AnalyzerParam::get("downsample cutoff", m_cutoffFreq) == -1) return -1;
		if (AnalyzerParam::get("downsample factor", m_dsFactor) == -1) return -1;
		m_dsSize = m_origSize / static_cast<unsigned int>(m_dsFactor);
		if (AnalyzerParam::get("envelope filter recovery", m_env_filt_rec) == -1) return -1;
		if (AnalyzerParam::get("autocorr resolution", m_autocorrRes) == -1) return -1;
		if (AnalyzerParam::get("bpm min value", m_bpm_min) == -1) return -1;
		if (AnalyzerParam::get("bpm max value", m_bpm_max) == -1) return -1;
		return 0;
	}
};

#endif
