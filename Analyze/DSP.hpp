#ifndef _DSP_H
#define _DSP_H

#include "Buffer.hpp"
#include <limits>
#include <cmath>
#include <functional>
#include <algorithm>
#include <vector>
#include <string>
#include <cassert>


namespace DSP
{
	using FLOAT_TYPE = float;

	template <typename T>
	void SWAP(T& v1, T& v2)
	{
		T temp = v1;
		v1 = v2;
		v2 = temp;
	}

	template <typename T>
	T get_max_value(const Buffer<T>& buffer)
	{
		SIZE_TYPE size = buffer.get_size();
		T max_value = std::numeric_limits<T>::lowest();
		for (SIZE_TYPE i = 0; i < size; ++i)
		{
			if (buffer[i] > max_value)
				max_value = buffer[i];
		}
		return max_value;
	}

	template <typename T>
	T get_min_value(const Buffer<T>& buffer)
	{
		SIZE_TYPE size = buffer.get_size();
		T min_value = std::numeric_limits<T>::max();
		for (SIZE_TYPE i = 0; i < size; ++i)
		{
			if (buffer[i] < min_value)
				min_value = buffer[i];
		}
		return min_value;
	}

	template <typename T>
	SIZE_TYPE get_max_index(const Buffer<T>& buffer)
	{
		SIZE_TYPE size = buffer.get_size();
		T max_value = std::numeric_limits<T>::lowest();
		SIZE_TYPE index = 0;
		for (SIZE_TYPE i = 0; i < size; ++i)
		{
			if (buffer[i] > max_value)
			{
				max_value = buffer[i];
				index = i;
			}
		}
		return index;
	}

	template <typename T>
	void maximize(Buffer<T>& buffer)
	{
		SIZE_TYPE size = buffer.get_size();
		T max_value = DSP::get_max_value(buffer);
		T min_value = DSP::get_min_value(buffer);
		FLOAT_TYPE factor = 1.0;

		if (max_value >= fabs(min_value))
			factor = static_cast<FLOAT_TYPE>(std::numeric_limits<T>::max() / max_value);
		else
			factor = fabs(static_cast<FLOAT_TYPE>(std::numeric_limits<T>::lowest()) / min_value);

		for (SIZE_TYPE i = 0; i < size; ++i)
		{
			buffer[i] = static_cast<T>(buffer[i] * factor);
		}
	}

	template <typename T>
	void gain(Buffer<T>& buffer, FLOAT_TYPE gain)
	{
		SIZE_TYPE size = buffer.get_size();

		for (SIZE_TYPE i = 0; i < size; ++i)
		{
			buffer[i] = static_cast<T>(buffer[i] * gain);
		}
	}

	template <typename T>
	FLOAT_TYPE get_mean_value(const Buffer<T>& buffer)
	{
		SIZE_TYPE size = buffer.get_size();
		FLOAT_TYPE mean = 0.0;

		for (SIZE_TYPE i = 0; i < size; ++i)
			mean += static_cast<FLOAT_TYPE>(buffer[i]) / size;

		return mean;
	}

	template <typename T>
	FLOAT_TYPE get_variance_value(const Buffer<T>& buffer)
	{
		SIZE_TYPE size = buffer.get_size();
		FLOAT_TYPE var = 0.0;
		double square = 0.0, sum = 0.0; //double to prevent overflow

		for (SIZE_TYPE i = 0; i < size; ++i)
		{
			double sample = static_cast<double>(buffer[i]);
			square += sample * sample;
			sum += sample;
		}

		var = (square - (sum * sum / size)) / (size - 1.0);
		return static_cast<FLOAT_TYPE>(var);
	}

	template <typename T>
	void combine_buffers(const Buffer<T>& buffer_X, const Buffer<T>& buffer_Y, Buffer<T>& outbuffer)
	{
		assert(buffer_X.get_size() == buffer_Y.get_size());
		assert(outbuffer.get_size() == 2 * buffer_X.get_size());
		SIZE_TYPE size = buffer_X.get_size();

		for (SIZE_TYPE i = 0; i < size; i++)
		{
			outbuffer[i * 2] = buffer_X[i];
			outbuffer[i * 2 + 1] = buffer_Y[i];
		}
	}

	template <typename T>
	void separate_buffers(const Buffer<T>& inbuffer, Buffer<T>& outbuffer_X, Buffer<T>& outbuffer_Y)
	{
		assert(outbuffer_X.get_size() == outbuffer_Y.get_size());
		assert(inbuffer.get_size() == 2 * outbuffer_X.get_size());
		SIZE_TYPE size = outbuffer_X.get_size();

		for (SIZE_TYPE i = 0; i < size; i++)
		{
			outbuffer_X[i] = inbuffer[i * 2];
			outbuffer_Y[i] = inbuffer[i * 2 + 1];
		}
	}

	template <typename FROM, typename TO>
	void convert_buffer(const Buffer<FROM>& from_buffer, Buffer<TO>& to_buffer)
	{
		assert(from_buffer.get_size() == to_buffer.get_size());
		FROM max_value = DSP::get_max_value(from_buffer);
		FROM min_value = DSP::get_min_value(from_buffer);
		TO max_limit = std::numeric_limits<TO>::max();
		TO min_limit = std::numeric_limits<TO>::lowest();
		assert((max_value <= max_limit) && (min_value >= min_limit));
		SIZE_TYPE size = from_buffer.get_size();

		for (SIZE_TYPE i = 0; i < size; ++i)
			to_buffer[i] = static_cast<TO>(from_buffer[i]);
	}

	template <typename T>
	void normalize(Buffer<T>& buffer, T maxvalue)
	{
		SIZE_TYPE size = buffer.get_size();
		T max_value = DSP::get_max_value(buffer);
		T min_value = DSP::get_min_value(buffer);
		FLOAT_TYPE factor = 1.0;

		if (max_value >= fabs(min_value))
			factor = static_cast<FLOAT_TYPE>(maxvalue) / max_value;
		else
			factor = -static_cast<FLOAT_TYPE>(maxvalue) / min_value;

		for (SIZE_TYPE i = 0; i < size; ++i)
			buffer[i] = static_cast<T>(buffer[i] * factor);
	}

	template <typename T>
	void rectify(Buffer<T>& buffer)
	{
		SIZE_TYPE size = buffer.get_size();

		for (SIZE_TYPE i = 0; i < size; ++i)
			buffer[i] = fabs(buffer[i]);
	}
	
	template <typename T>
	FLOAT_TYPE get_autocorr_value(const Buffer<T>& buffer, SIZE_TYPE lag_samples)
	{
		SIZE_TYPE size = buffer.get_size();
		FLOAT_TYPE mean_value = DSP::get_mean_value(buffer);
		FLOAT_TYPE variance_value = DSP::get_variance_value(buffer);
		FLOAT_TYPE autocv = 0.0;
		for (SIZE_TYPE i = 0; i < (size - lag_samples); ++i)
		{
			autocv += ((buffer[i] - mean_value) * (buffer[i + lag_samples] - mean_value));
		}
		autocv = (1.0 / (size - lag_samples)) * autocv;
		return autocv / variance_value;
	}
	
	template <typename T>
	FLOAT_TYPE get_autocorr_value(const Buffer<T>& buffer, SIZE_TYPE lag_samples, SIZE_TYPE size, FLOAT_TYPE mean_value, FLOAT_TYPE variance_value)
	{
		FLOAT_TYPE autocv = 0.0;
		for (SIZE_TYPE i = 0; i < (size - lag_samples); ++i)
		{
			autocv += ((buffer[i] - mean_value) * (buffer[i + lag_samples] - mean_value));
		}
		autocv = (1.0 / (size - lag_samples)) * autocv;
		return autocv / variance_value;
	}

	template <typename T>
	void auto_correlation(const Buffer<T>& inbuffer, Buffer<T>& outbuffer)
	{
		SIZE_TYPE size = inbuffer.get_size();
		
		for (SIZE_TYPE i = 0; i < size; ++i)
		{
			outbuffer[i] = DSP::get_autocorr_value(inbuffer, i);
		}
	}	

	template <typename T>
	void auto_correlation_no_mean(const Buffer<T>& inbuffer, Buffer<T>& outbuffer)
	{
		SIZE_TYPE size = inbuffer.get_size();
	    FLOAT_TYPE sum = 0.0;

		for (SIZE_TYPE lag = 0; lag < size; ++lag) 
		{
			sum = 0.0;
			for (SIZE_TYPE i = 0; i < size - lag; ++i) 
			{
				sum += inbuffer[i] * inbuffer[i + lag];
			}
			sum = sum / size;
			outbuffer[lag] = sum;
		}
	}

	template <typename T>
	FLOAT_TYPE get_rms_value(const Buffer<T>& buffer)
	{
		SIZE_TYPE size = buffer.get_size();
		FLOAT_TYPE rms_value = 0.0;

		for (SIZE_TYPE i = 0; i < size; ++i)
			rms_value += (buffer[i] * buffer[i]) / size;

		return sqrt(rms_value);
	}

	FLOAT_TYPE lanczos_window(FLOAT_TYPE x, FLOAT_TYPE radius)
	{
		if (x == 0.0) return 1.0;
		if (x <= -radius || x >= radius) return 0.0;
		FLOAT_TYPE pi_x = x * M_PI;
		return radius * sin(pi_x) * sin(pi_x / radius) / (pi_x * pi_x);
	}

	template <typename T>
	void resample_buffer(const Buffer<T>& inbuffer, Buffer<T>& outbuffer)
	{
		//See https://en.m.wikipedia.org/wiki/Lanczos_resampling
		SIZE_TYPE src_len = inbuffer.get_size();
		SIZE_TYPE dest_len = outbuffer.get_size();

	    const FLOAT_TYPE blur = 1.0;
	    const FLOAT_TYPE factor = dest_len / static_cast<FLOAT_TYPE>(src_len);
	    const FLOAT_TYPE filter_radius = 3.0;

	    FLOAT_TYPE scale   = std::min(factor, 1.0f) / blur;
	    FLOAT_TYPE support = filter_radius / scale;

	    //5 -> room for rounding up in calculations of start, stop and support
	    std::vector<FLOAT_TYPE> contribution(std::min(static_cast<FLOAT_TYPE>(src_len), 5.0f + 2.0f * support));

	    if (support <= 0.5f)
	    {
	    	support = 0.5f + 1E-12;
	    	scale = 1.0f;
	    }

	    for (SIZE_TYPE x = 0; x < dest_len; ++x)
	    {
	    	FLOAT_TYPE center = (x + 0.5f) / factor;
	        SIZE_TYPE start = static_cast<SIZE_TYPE>(std::max(center - support + 0.5f, 0.0f));
	        SIZE_TYPE stop  = static_cast<SIZE_TYPE>(std::min(center + support + 0.5f, static_cast<FLOAT_TYPE>(src_len)));
	        FLOAT_TYPE density = 0.0f;
	        SIZE_TYPE nmax = stop - start;
	        FLOAT_TYPE s = start - center + 0.5f;
	        outbuffer[x] = 0;
	        for (SIZE_TYPE n = 0; n < nmax; ++n, ++s)
	        {
	            contribution[n] = lanczos_window(s * scale, filter_radius);
	            density += contribution[n];
	            outbuffer[x] += inbuffer[start + n] * contribution[n];
	        }

	        if (density != 0.0 && density != 1.0)
	        {
	            //Normalize
	            outbuffer[x] /= density;
	        }
	    }
	}

	template <typename T>
	void downsample_buffer(const Buffer<T>& inbuffer, Buffer<T>& outbuffer, SIZE_TYPE N)
	{
		//Not optimal algorithm, produces aliasing
		SIZE_TYPE size_in = inbuffer.get_size();
		SIZE_TYPE size_out = outbuffer.get_size();
		assert(size_in == N * size_out);

		for (SIZE_TYPE i = 0; i < size_out * N; i += N)
			outbuffer[i / N] = inbuffer[i];
	}

	template <typename SRC, typename DST>
	void envelope_filter(const Buffer<SRC>& inbuffer, Buffer<DST>& outbuffer, FLOAT_TYPE recovery, SIZE_TYPE sample_rate)
	{
		SIZE_TYPE size_in = inbuffer.get_size();
		SIZE_TYPE size_out = outbuffer.get_size();
		assert(size_in == size_out);

		FLOAT_TYPE env_in = 0.0;
		FLOAT_TYPE peak_env = 0.0;
		FLOAT_TYPE exp_fact = recovery * static_cast<FLOAT_TYPE>(sample_rate);
		FLOAT_TYPE release = exp(-1.0 / exp_fact);

		for (SIZE_TYPE i = 0; i < size_in; ++i)
		{
			env_in = fabs(inbuffer[i]);
			if (env_in > peak_env)
			{
				peak_env = env_in;
			}
			else
			{
				peak_env = peak_env * release;
				peak_env = peak_env + (1.0 - release) * env_in;
			}

			outbuffer[i] = static_cast<DST>(peak_env);
		}
	}

	template <typename T>
	void perform_fft(const Buffer<T>& time_dom, Buffer<FLOAT_TYPE>& freq_dom, int sign)
	{
		SIZE_TYPE size = time_dom.get_size();
		SIZE_TYPE size_f = freq_dom.get_size();
		assert(size_f == 2 * size);

		//Put data in complex buffer - only real data, imaginary is zero
		for (SIZE_TYPE i = 0; i < size; ++i)
		{
			freq_dom[i * 2] = static_cast<FLOAT_TYPE>(time_dom[i]);
			freq_dom[i * 2 + 1] = 0;
		}

		SIZE_TYPE n, mmax, m, j, istep, i;
		FLOAT_TYPE wtemp, wr, wpr, wpi, wi, theta;
		FLOAT_TYPE tempr, tempi;

		//Reverse-binary reindexing
		n = size << 1;
		j = 1;
		for (i = 1; i < n; i += 2)
		{
			if (j > i)
			{
				DSP::SWAP(freq_dom[j - 1], freq_dom[i - 1]);
				DSP::SWAP(freq_dom[j], freq_dom[i]);
			}
			m = size;
			while (m >= 2 && j > m)
			{
				j -= m;
				m >>= 1;
			}
			j += m;
		}

		//Danielson-Lanczos section
		mmax = 2;
		while (n > mmax)
		{
			istep = mmax << 1;
			theta = -sign * (2 * M_PI / mmax); // - for forward, + for inverse
			wtemp = sin(0.5 * theta);
			wpr = -2.0 * wtemp * wtemp;
			wpi = sin(theta);
			wr = 1.0;
			wi = 0.0;
			for (m = 1; m < mmax; m += 2)
			{
				for (i = m; i <= n; i += istep)
				{
					j = i + mmax;
					tempr = wr * freq_dom[j - 1] - wi * freq_dom[j];
					tempi = wr * freq_dom[j] + wi * freq_dom[j - 1];

					freq_dom[j - 1] = freq_dom[i - 1] - tempr;
					freq_dom[j] = freq_dom[i] - tempi;
					freq_dom[i - 1] += tempr;
					freq_dom[i] += tempi;
				}
				wtemp = wr;
				wr += wr * wpr - wi * wpi;
				wi += wi * wpr + wtemp * wpi;
			}
			mmax = istep;
		}
	}

	template <typename T, typename F, typename... Args>
	void apply_window(Buffer<T>& buffer, F func, Args&&... args)
	{
		SIZE_TYPE size = buffer.get_size();
		for (SIZE_TYPE i = 0; i < size; ++i)
			buffer[i] *= func(i, size, std::forward<Args>(args)...);
	}

	FLOAT_TYPE cosw(FLOAT_TYPE x, FLOAT_TYPE N)
	{
		return cos(M_PI * x / (N - 1));
	}

	FLOAT_TYPE hanning(FLOAT_TYPE x, FLOAT_TYPE N)
	{
		return 0.5 * (1 - DSP::cosw(2 * x, N));
	}

	FLOAT_TYPE hamming(FLOAT_TYPE x, FLOAT_TYPE N)
	{
		return 0.54 - 0.46 * DSP::cosw(2 * x, N);
	}

	FLOAT_TYPE blackman(FLOAT_TYPE x, FLOAT_TYPE N, FLOAT_TYPE alpha)
	{
		FLOAT_TYPE a0 = (1 - alpha) / 2;
		FLOAT_TYPE a1 = 0.5;
		FLOAT_TYPE a2 = alpha / 2;
		return a0 - a1 * DSP::cosw(2 * x, N) + a2 * DSP::cosw(4 * x, N);
	}

	FLOAT_TYPE blackman_harris(FLOAT_TYPE x, FLOAT_TYPE N)
	{
		FLOAT_TYPE a0 = 0.35875;
		FLOAT_TYPE a1 = 0.48829;
		FLOAT_TYPE a2 = 0.14128;
		FLOAT_TYPE a3 = 0.01168;
		return a0 - a1 * DSP::cosw(2 * x, N) + a2 * DSP::cosw(4 * x, N) - a3 * DSP::cosw(6 * x, N);
	}

	FLOAT_TYPE flat_top(FLOAT_TYPE x, FLOAT_TYPE N)
	{
		FLOAT_TYPE a0 = 0.21557895;
		FLOAT_TYPE a1 = 0.41663158;
		FLOAT_TYPE a2 = 0.277263158;
		FLOAT_TYPE a3 = 0.083578947;
		FLOAT_TYPE a4 = 0.006947368;
		return a0 - a1 * DSP::cosw(2 * x, N) + a2 * DSP::cosw(4 * x, N) - a3 * DSP::cosw(6 * x, N) + a4 * DSP::cosw(8 * x, N);
	}

	FLOAT_TYPE tukey(FLOAT_TYPE x, FLOAT_TYPE N, FLOAT_TYPE alpha)
	{
		if (x >= 0 && x < (alpha * (N - 1) / 2))
			return 0.5 * (1 + cos(M_PI * ((2 * x / (alpha * (N - 1))) - 1)));
		else if ((N - 1) * (1 - alpha / 2) < x && x <= (N - 1))
			return 0.5 * (1 + cos(M_PI * (1 - 2 / alpha + (2 * x / (alpha * (N - 1))))));
		else
			return 1.0;
	}

	FLOAT_TYPE bartlett(FLOAT_TYPE x, FLOAT_TYPE N)
	{
		FLOAT_TYPE xinv = (N - 1) /2;
		return 1 / xinv * (xinv - fabs(x - xinv));
	}

	template <typename T>
	void moving_average(const Buffer<T>& inbuffer, Buffer<FLOAT_TYPE>& outbuffer, SIZE_TYPE N)
	{
		SIZE_TYPE size_in = inbuffer.get_size();
		SIZE_TYPE size_out = outbuffer.get_size();
		assert(size_in == size_out);

		FLOAT_TYPE ma_value = 0.0;
		SIZE_TYPE start, num;

		for (SIZE_TYPE i = 0; i < size_in; ++i)
		{
			//Calculate start and stop indexes
			if (i < N)
			{
				start = 0;
				num = i + 1;
			}
			else
			{
				start = i - N + 1;
				num = N;
			}

			//Calculate moving average
			for (SIZE_TYPE j = start; j < start + num; ++j)
			{
				ma_value += inbuffer[j] / static_cast<FLOAT_TYPE>(num);
			}
			outbuffer[i] = ma_value;
			ma_value = 0.0;
		}
	}

	template <typename T1, typename T2>
	void lowpass_filter(Buffer<T1>& inbuffer, Buffer<T2>& filtbuffer, FLOAT_TYPE cutoff_Hz, FLOAT_TYPE sampleT_s)
	{
		FLOAT_TYPE e_pow = 1.0f - exp(-sampleT_s * 2.0f * M_PI * cutoff_Hz);

		unsigned int size_in = inbuffer.get_size();
		unsigned int size_filt = filtbuffer.get_size();
		assert(size_in == size_filt);

		FLOAT_TYPE output = 0;
		for (unsigned int i = 0; i < size_in; ++i)
		{
			output += (static_cast<FLOAT_TYPE>(inbuffer[i] - output) * e_pow);
			filtbuffer[i] = static_cast<T2>(output);
		}
	}

	template <typename T>
	void autocorr_bpm(const Buffer<T>& inbuffer, Buffer<FLOAT_TYPE>& autocorr_values, SIZE_TYPE sample_rate, FLOAT_TYPE bpm_min, FLOAT_TYPE bpm_max)
	{
		SIZE_TYPE size_autocorr = autocorr_values.get_size();
		SIZE_TYPE size_inbuffer = inbuffer.get_size();

		FLOAT_TYPE mean = DSP::get_mean_value(inbuffer);
		FLOAT_TYPE variance = DSP::get_variance_value(inbuffer);

		//Calculate time axis - lag is in seconds
		FLOAT_TYPE time_max = static_cast<FLOAT_TYPE>(size_inbuffer) / sample_rate;

		//Calculate lag values -> beats/minute to seconds/beat
		FLOAT_TYPE min_lag = 60.0 / bpm_max;
		FLOAT_TYPE max_lag = 60.0 / bpm_min;
		FLOAT_TYPE lag;
		SIZE_TYPE lag_samples;

		for (SIZE_TYPE i = 0; i < size_autocorr; ++i)
		{
			lag = min_lag + (max_lag - min_lag) / size_autocorr * i;
			//Calculate number of samples corresponding to desired lag
			lag_samples = static_cast<SIZE_TYPE>(ceil(lag / time_max * size_inbuffer));
			//autocorr_values[i] = DSP::get_autocorr_value(inbuffer, lag); //Slow
			autocorr_values[i] = DSP::get_autocorr_value(inbuffer, lag_samples, size_inbuffer, mean, variance);

		}
	}

	FLOAT_TYPE extract_bpm_value(const Buffer<FLOAT_TYPE>& autocorr_values, FLOAT_TYPE bpm_min, FLOAT_TYPE bpm_max)
	{
		SIZE_TYPE size = autocorr_values.get_size();

		//Calculate lag values -> beats/minute to seconds/beat
		FLOAT_TYPE min_lag = 60.0 / bpm_max;
		FLOAT_TYPE max_lag = 60.0 / bpm_min;

		SIZE_TYPE max_array_index = DSP::get_max_index(autocorr_values);
		FLOAT_TYPE bpm_lag = min_lag + (max_lag - min_lag) / size * max_array_index;

		return 60.0 / bpm_lag;
	}
}

#endif
