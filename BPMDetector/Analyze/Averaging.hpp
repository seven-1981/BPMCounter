#ifndef _AVERAGING_H
#define _AVERAGING_H

#include <cmath>


template <int SIZE>
class Averaging
{
public:
	Averaging(double threshold1, double threshold2, int badValues) :
		m_threshold1(threshold1),
		m_threshold2(threshold2),
		m_maxBadValues(badValues),
		m_badValues(0),
		m_firstValueSet(false)
	{
		init_array(0.0);
	}

	~Averaging() { }

	double process_value(double value)
	{
		if (m_firstValueSet == false)
		{
			m_firstValueSet = true;
			init_array(value);
			return value;
		}
		else
		{
			double avg = calculate_average();
			double diff = fabs(value - avg);

			switch (check_threshold(diff))
			{
			case THRES2:
				m_badValues++;
				if (m_badValues > m_maxBadValues)
				{
					init_array(value);
					return value;
				}
				else
					return avg;
				break;
			case THRES1:
				init_array(value);
				return value;
				break;
			case IN_RANGE:
			default:
				push_new_value(value);
				return calculate_average();
				break;
			}
		}
	}

private:
	double m_array[SIZE];
	double m_threshold1;
	double m_threshold2;
	int m_maxBadValues;

	int m_badValues;
	bool m_firstValueSet;

	enum : int { THRES2, THRES1, IN_RANGE };

	void init_array(double value)
	{
		for (int i = 0; i < SIZE; ++i)
			m_array[i] = value;
		m_badValues = 0;
	}

	void push_new_value(double value)
	{
		for (int i = SIZE - 1; i > 0; --i)
			m_array[i] = m_array[i - 1];
		m_array[0] = value;
		m_badValues = 0;
	}

	double calculate_average()
	{
		double avg = 0.0;
		for (int i = 0; i < SIZE; ++i)
			avg += m_array[i] / SIZE;
		return avg;
	}

	int check_threshold(double threshold)
	{
		if (threshold >= m_threshold2) return THRES2;
		if (threshold >= m_threshold1) return THRES1;
		return IN_RANGE;
	}
};

#endif
