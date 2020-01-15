#include "Buffer.hpp"
#include "DSP.hpp"
#include <iostream>
#include <gnuplot-iostream.h>
#include <vector>
#include <utility>
#include <string>


template <typename T>
void plot(Buffer<T>& buffer, std::string title)
{
	Gnuplot gp;
	gp << "plot '-' with lines title '" << title << "'\n";
	std::vector<T> data;
	unsigned int size = buffer.get_size();
	for (unsigned int i = 0; i < size; i++)
		data.push_back(buffer[i]);
	gp.send1d(data);
}

template <typename T>
void plot(Buffer<T>& b1, Buffer<T>& b2, std::string title)
{
	using Pair = std::pair<T, T>;
	assert(b1.get_size() == b2.get_size());
	Gnuplot gp;
	gp << "plot '-' with lines title '" << title << "'\n";
	std::vector<Pair> pairs;
	for (unsigned int i = 0; i < b1.get_size(); i++)
		pairs.push_back(Pair(b1[i], b2[i]));
	gp.send1d(pairs);
}

template <typename T1, typename T2>
void plot2(Buffer<T1>& b1, Buffer<T2>& b2, std::string title1, std::string title2)
{
	unsigned int size1 = b1.get_size();
	unsigned int size2 = b2.get_size();
	assert(size1 == size2);
	Gnuplot gp;
	gp << "plot '-' with lines title '" << title1 << "',";
	gp << "'-' with lines title '" << title2 << "'\n";
	std::vector<T1> data1;
	std::vector<T2> data2;

	for (unsigned int i = 0; i < size1; i++)
	{
		data1.push_back(b1[i]);
		data2.push_back(b2[i]);
	}

	gp.send1d(data1);
	gp.send1d(data2);
}

int main()
{
	std::cout << "Start." << std::endl;

	std::cout << "Select plots: ";
	std::string sel = "000000";
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
		plot2(y, outAuto, "Input data", "Autocorrelation - no mean");
		DSP::auto_correlation(y, outAuto);
		plot2(y, outAuto, "Input data", "Autocorrelation");
	}
	
	if (sel[1] == '1')
	{
		//Downsampling
		plot(ysin, "Input data");
		unsigned int downsized = 73;
		Buffer<double> yDownsampled(downsized);
		DSP::resample_buffer(ysin, yDownsampled);
		plot(yDownsampled, "Downsampled");
	}

	if (sel[2] == '1')
	{
		//Envelope filter
		Buffer<double> buffer_env(y.get_size());
		DSP::envelope_filter(y, buffer_env, 2.5, 1.0);
		plot2(y, buffer_env, "Input data", "Envelope filter");
	}

	if (sel[3] == '1')
	{
		//FFT
		Buffer<float> dirac = { 1,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 };
		Buffer<float> fft(dirac.get_size() * 2);
		Buffer<float> f_real(dirac.get_size());
		Buffer<float> f_imag(dirac.get_size());
		DSP::perform_fft(dirac, fft, +1);
		DSP::separate_buffers(fft, f_real, f_imag);
		plot(f_real, "FFT - dirac");

		Buffer<float> sine = { 0,0, 1,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 };
		DSP::perform_fft(sine, fft, +1);
		DSP::separate_buffers(fft, f_real, f_imag);
		plot(f_real, "FFT - sine");
	}

	if (sel[4] == '1')
	{
		//Windowing
		unsigned int size = 100;
		Buffer<float> buf_base(size);
		for (unsigned int i = 0; i < size; ++i)
			buf_base[i] = 1.0;

		Buffer<float> w1(buf_base);
		DSP::apply_window(w1, DSP::hanning);

		Buffer<float> w2(buf_base);
		DSP::apply_window(w2, DSP::hamming);
		plot2(w1, w2, "Hanning window", "Hamming window");

		Buffer<float> w3(buf_base);
		DSP::apply_window(w3, DSP::blackman, 0.16);

		Buffer<float> w4(buf_base);
		DSP::apply_window(w4, DSP::blackman_harris);
		plot2(w3, w4, "Blackman window", "Blackman harris window");

		Buffer<float> w5(buf_base);
		DSP::apply_window(w5, DSP::flat_top);
		plot(w5, "Flat top window");

		Buffer<float> w6(buf_base);
		DSP::apply_window(w6, DSP::tukey, 0.5);
		plot(w6, "Tukey window");

		Buffer<float> w7(buf_base);
		DSP::apply_window(w7, DSP::bartlett);
		plot(w7, "Bartlett window");
	}

	if (sel[5] == '1')
	{
		Buffer<float> mov_buf = { 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 };
		Buffer<float> avged(mov_buf.get_size());
		DSP::moving_average(mov_buf, avged, 4);
		plot2(mov_buf, avged, "Base data", "Moving average");
	}

	std::cout << "Ende." << std::endl;
	return 0;
}
