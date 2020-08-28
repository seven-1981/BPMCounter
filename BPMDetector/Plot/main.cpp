#include "Buffer.hpp"
#include "DSP.hpp"
#include <iostream>
#include "Plotting.hpp"


int main()
{
	std::cout << "Start." << std::endl;

	std::cout << "Select plots: ";
	std::string sel = "0000000";
	std::cin >> sel;

	Buffer<double> y = { 0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0 };

	unsigned int size = 1000;
	unsigned int sample_rate_hz = 1000;
	double amp = 10.0;
	double f = 10.0;
	double tmax = size / static_cast<double>(sample_rate_hz);
	Buffer<double> ysin(size);
	for (unsigned int i = 0; i < size; ++i)
		ysin[i] = amp * sin(2.0 * M_PI * f * (i / static_cast<double>(size) * tmax));

	if (sel[0] == '1')
	{
		//Autocorrelation
		Buffer<double> outAuto(y.get_size());
		DSP::auto_correlation_no_mean(y, outAuto);
		PLOT::plot2(y, outAuto, "Input data", "Autocorrelation - no mean");
		DSP::auto_correlation(y, outAuto);
		PLOT::plot2(y, outAuto, "Input data", "Autocorrelation");
	}
	
	if (sel[1] == '1')
	{
		//Downsampling
		PLOT::plot(ysin, "Input data");
		unsigned int downsized = 73;
		Buffer<double> yDownsampled(downsized);
		DSP::resample_buffer(ysin, yDownsampled);
		PLOT::plot(yDownsampled, "Downsampled");
	}

	if (sel[2] == '1')
	{
		//Envelope filter
		Buffer<double> buffer_env(y.get_size());
		DSP::envelope_filter(y, buffer_env, 2.5, 1.0);
		PLOT::plot2(y, buffer_env, "Input data", "Envelope filter");
	}

	if (sel[3] == '1')
	{
		//FFT
		Buffer<double> dirac = { 1,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 };
		Buffer<double> fft(dirac.get_size() * 2);
		Buffer<double> f_real(dirac.get_size());
		Buffer<double> f_imag(dirac.get_size());
		DSP::perform_fft(dirac, fft, +1);
		DSP::separate_buffers(fft, f_real, f_imag);
		PLOT::plot(f_real, "FFT - dirac");

		Buffer<double> sine = { 0,0, 1,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 };
		DSP::perform_fft(sine, fft, +1);
		DSP::separate_buffers(fft, f_real, f_imag);
		PLOT::plot(f_real, "FFT - sine");
	}

	if (sel[4] == '1')
	{
		//Windowing
		unsigned int size = 100;
		Buffer<double> buf_base(size);
		for (unsigned int i = 0; i < size; ++i)
			buf_base[i] = 1.0;

		Buffer<double> w1(buf_base);
		DSP::apply_window(w1, DSP::hanning);

		Buffer<double> w2(buf_base);
		DSP::apply_window(w2, DSP::hamming);
		PLOT::plot2(w1, w2, "Hanning window", "Hamming window");

		Buffer<double> w3(buf_base);
		DSP::apply_window(w3, DSP::blackman, 0.16);

		Buffer<double> w4(buf_base);
		DSP::apply_window(w4, DSP::blackman_harris);
		PLOT::plot2(w3, w4, "Blackman window", "Blackman harris window");

		Buffer<double> w5(buf_base);
		DSP::apply_window(w5, DSP::flat_top);
		PLOT::plot(w5, "Flat top window");

		Buffer<double> w6(buf_base);
		DSP::apply_window(w6, DSP::tukey, 0.5);
		PLOT::plot(w6, "Tukey window");

		Buffer<double> w7(buf_base);
		DSP::apply_window(w7, DSP::bartlett);
		PLOT::plot(w7, "Bartlett window");
	}

	if (sel[5] == '1')
	{
		Buffer<double> mov_buf = { 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 };
		Buffer<double> avged(mov_buf.get_size());
		DSP::moving_average(mov_buf, avged, 4);
		PLOT::plot2(mov_buf, avged, "Base data", "Moving average");
	}

	if (sel[6] == '1')
	{
		Buffer<double> filt(size);
		Buffer<double> unfilt(size);
		unsigned int cycle = 50, cnt = cycle;
		float ampl = 10.0f;
		for (unsigned int i = 0; i < size; ++i)
		{
			if (cnt-- <= 0)
			{
				cnt = cycle;
				ampl *= -1;
			}
			unfilt[i] = ampl;
		}

		DSP::lowpass_filter(ysin, filt, 10.0f, 1.0f / sample_rate_hz);
		PLOT::plot2(ysin, filt, "Base data", "Lowpass filter");
	}

	std::cout << "Ende." << std::endl;
	return 0;
}
