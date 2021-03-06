#ifndef _BPMANALYZER_H
#define _BPMANALYZER_H

#include "AnalyzerParam.hpp"
#include "Buffer.hpp"
#include "DSP.hpp"
#include "StopWatch.hpp"
#include "Averaging.hpp"

#ifdef ENABLE_BPMANALYZER_PLOT
#include "Plotting.hpp"
#define PLOT_BUF(bf, title) PLOT::plot(bf, title)
#else
#define PLOT_BUF(bf, title)
#endif


template <typename T>
class BPMAnalyzer : public AnalyzerParam
{
public:
	explicit BPMAnalyzer(Buffer<T>& buffer, unsigned int sample_rate_Hz = 44100) :
		AnalyzerParam( {
			new TypedParam<ParamType::Double>("downsample cutoff", 2750.0L),
			new TypedParam<ParamType::UInt>("downsample factor", 8U),
			new TypedParam<ParamType::Double>("envelope filter recovery", 0.005L),
			new TypedParam<ParamType::UInt>("autocorr resolution", 1200U),
			new TypedParam<ParamType::Double>("bpm min value", 100.0L),
			new TypedParam<ParamType::Double>("bpm max value", 200.0L),
			new TypedParam<ParamType::Bool>("averaging active", true),
			new TypedParam<ParamType::Double>("averaging threshold 1", 3.0f),
			new TypedParam<ParamType::Double>("averaging threshold 2", 20.0f),
			new TypedParam<ParamType::Int>("averaging max bad values", 3) } ),
		m_buffer(buffer),
		m_sample_rate_Hz(sample_rate_Hz),
		m_origSize(0),
		m_dsFactor(1),
		m_dsSize(0),
		m_cutoffFreq(0),
		m_env_filt_rec(0),
		m_autocorrRes(0),
		m_bpm_min(0),
		m_bpm_max(0),
		m_averagingActive(false),
		m_avgThreshold1(0.0f),
		m_avgThreshold2(0.0f),
		m_avgMaxBadValues(0)
	{
		m_initState = init_params();
		init_sizes();
		init_buffers();
		// Create dynamically because of init order.
		m_averaging = new Averaging<AVERAGING_SIZE>(m_avgThreshold1, m_avgThreshold2, m_avgMaxBadValues);
	}

	~BPMAnalyzer()
	{
		delete m_bf_lp;
		delete m_bf_ds;
		delete m_bf_ef;
		delete m_bf_ac;
		delete m_averaging;
	}

	FLOAT_TYPE analyze() override
	{
		if (m_initState != 0) return 0.0;
		if (m_origSize == 0) return 0.0;
		StopWatch watch;
		FLOAT_TYPE dt = 1.0f / static_cast<FLOAT_TYPE>(m_sample_rate_Hz);
		DSP::lowpass_filter(m_buffer, *m_bf_lp, m_cutoffFreq, dt);
		//watch.tic(std::cout);
		DSP::resample_buffer(*m_bf_lp, *m_bf_ds);
		//watch.tic(std::cout);
		PLOT_BUF(*m_bf_ds, "Resampled");
		DSP::envelope_filter(*m_bf_ds, *m_bf_ef, m_env_filt_rec, m_sample_rate_Hz / m_dsFactor);
		//watch.tic(std::cout);
		PLOT_BUF(*m_bf_ef, "Envelope filter");
		DSP::autocorr_bpm(*m_bf_ef, *m_bf_ac, m_sample_rate_Hz / m_dsFactor, m_bpm_min, m_bpm_max);
		PLOT_BUF(*m_bf_ac, "Autocorrelation");
		//watch.tic(std::cout);
		double bpm_value = DSP::extract_bpm_value(*m_bf_ac, m_bpm_min, m_bpm_max);
		return perform_averaging(bpm_value);
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

	static const int AVERAGING_SIZE = 10;
	Averaging<AVERAGING_SIZE>* m_averaging;

	bool m_averagingActive;
	double m_avgThreshold1;
	double m_avgThreshold2;
	int m_avgMaxBadValues;

	int init_params()
	{
		if (AnalyzerParam::get("downsample cutoff", m_cutoffFreq) == -1) return -1;
		if (AnalyzerParam::get("downsample factor", m_dsFactor) == -1) return -1;
		if (AnalyzerParam::get("envelope filter recovery", m_env_filt_rec) == -1) return -1;
		if (AnalyzerParam::get("autocorr resolution", m_autocorrRes) == -1) return -1;
		if (AnalyzerParam::get("bpm min value", m_bpm_min) == -1) return -1;
		if (AnalyzerParam::get("bpm max value", m_bpm_max) == -1) return -1;
		if (AnalyzerParam::get("averaging active", m_averagingActive) == -1) return -1;
		if (AnalyzerParam::get("averaging threshold 1", m_avgThreshold1) == -1) return -1;
		if (AnalyzerParam::get("averaging threshold 2", m_avgThreshold2) == -1) return -1;
		if (AnalyzerParam::get("averaging max bad values", m_avgMaxBadValues) == -1) return -1;
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

	double perform_averaging(double value)
	{
		if (m_averagingActive == true)
			return m_averaging->process_value(value);
		else
			return value;
	}
};

#endif
