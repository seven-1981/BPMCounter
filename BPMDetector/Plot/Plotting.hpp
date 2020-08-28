#include "Buffer.hpp"
#include <gnuplot-iostream.h>
#include <vector>
#include <utility>
#include <string>


namespace PLOT
{

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
}
