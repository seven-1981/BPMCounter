#ifndef _ANALYZER1_H
#define _ANALYZER1_H

#include "AnalyzerParam.hpp"
#include "Buffer.hpp"
#include "DSP.hpp"
//#include "Plotting.hpp"
#include "StopWatch.hpp"
#include <iostream>


template <typename T>
class Analyzer1 : public AnalyzerParam
{
public:
	explicit Analyzer1(Buffer<T>& buffer, unsigned int sample_rate_Hz = 44100) :
		AnalyzerParam( {
			new TypedParam<ParamType::Double>("downsample cutoff", 2750.0L),
			new TypedParam<ParamType::UInt>("downsample factor", 8U),
			new TypedParam<ParamType::Double>("envelope filter recovery", 0.005L),
			new TypedParam<ParamType::UInt>("autocorr resolution", 1200U),
			new TypedParam<ParamType::Double>("bpm min value", 80.0L),
			new TypedParam<ParamType::Double>("bpm max value", 200.0L) } ),
		m_buffer(buffer),
		m_sample_rate_Hz(sample_rate_Hz),
		m_origSize(0),
		m_dsFactor(1),
		m_dsSize(0),
		m_cutoffFreq(0),
		m_env_filt_rec(0),
		m_autocorrRes(0),
		m_bpm_min(0),
		m_bpm_max(0)
	{
		m_initState = init_params();
		init_sizes();
		init_buffers();
	}

	~Analyzer1()
	{
		delete m_bf_lp;
		delete m_bf_ds;
		delete m_bf_ef;
		delete m_bf_ac;
	}

	FLOAT_TYPE analyze() override
	{
		if (m_initState != 0) return 0.0;
		if (m_origSize == 0) return 0.0;
		StopWatch watch;
		FLOAT_TYPE dt = 1.0f / static_cast<FLOAT_TYPE>(m_sample_rate_Hz);
		DSP::lowpass_filter(m_buffer, *m_bf_lp, m_cutoffFreq, dt);
		std::cout << "Time = " << watch.get_time_us() / 1000.0f << std::endl;
		DSP::resample_buffer(*m_bf_lp, *m_bf_ds);
		std::cout << "Time = " << watch.get_time_us() / 1000.0f << std::endl;
		DSP::envelope_filter(*m_bf_ds, *m_bf_ef, m_env_filt_rec, m_sample_rate_Hz / m_dsFactor);
		std::cout << "Time = " << watch.get_time_us() / 1000.0f << std::endl;
		//PLOT::plot(*m_bf_ef, "Env");
		DSP::autocorr_bpm(*m_bf_ef, *m_bf_ac, m_sample_rate_Hz / m_dsFactor, m_bpm_min, m_bpm_max);
		//PLOT::plot(*m_bf_ac, "Ac");
		std::cout << "Time = " << watch.get_time_us() / 1000.0f << std::endl;
		return DSP::extract_bpm_value(*m_bf_ac, m_bpm_min, m_bpm_max);
	}

private:
	Buffer<T>& m_buffer;
	unsigned int m_sample_rate_Hz;
	unsigned int m_origSize;
	int m_initState;

	Buffer<T>* m_bf_lp;
	Buffer<T>* m_bf_ds;
	Buffer<T>* m_bf_ef;
	Buffer<FLOAT_TYPE>* m_bf_ac;

	unsigned int m_dsFactor;
	unsigned int m_dsSize;

	FLOAT_TYPE m_cutoffFreq;
	FLOAT_TYPE m_env_filt_rec;
	unsigned int m_autocorrRes;
	FLOAT_TYPE m_bpm_min;
	FLOAT_TYPE m_bpm_max;

	int init_params()
	{
		if (AnalyzerParam::get("downsample cutoff", m_cutoffFreq) == -1) return -1;
		if (AnalyzerParam::get("downsample factor", m_dsFactor) == -1) return -1;
		if (AnalyzerParam::get("envelope filter recovery", m_env_filt_rec) == -1) return -1;
		if (AnalyzerParam::get("autocorr resolution", m_autocorrRes) == -1) return -1;
		if (AnalyzerParam::get("bpm min value", m_bpm_min) == -1) return -1;
		if (AnalyzerParam::get("bpm max value", m_bpm_max) == -1) return -1;
		return 0;
	}

	void init_sizes()
	{
		m_origSize = m_buffer.get_size();
		m_dsSize = m_origSize / m_dsFactor;
	}

	void init_buffers()
	{
		m_bf_lp = new Buffer<T>(m_origSize);
		m_bf_ds = new Buffer<T>(m_dsSize);
		m_bf_ef = new Buffer<T>(m_dsSize);
		m_bf_ac = new Buffer<FLOAT_TYPE>(m_autocorrRes);
	}
};

#endif
