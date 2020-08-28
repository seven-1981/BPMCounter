#ifndef _FREQUENCYSPECTRUM_H
#define _FREQUENCYSPECTRUM_H

#include <QVector>


class FrequencySpectrum
{
public:
	explicit FrequencySpectrum(unsigned int numPoints = 0);

	struct Element
	{
		Element() :
			frequency(0.0),
			amplitude(0.0),
			phase(0.0),
			clipped(false) { }

		qreal frequency; //Hz
		qreal amplitude; //0...1
		qreal phase; //0...2Pi
		bool clipped;
	};

	using iterator = QVector<Element>::iterator;
	using const_iterator = QVector<Element>::const_iterator;

	iterator begin();
	iterator end();
	const_iterator cbegin() const;
	const_iterator cend() const;

	void reset();
	unsigned int count() const;

	Element& operator[](unsigned int index);
	const Element& operator[](unsigned int index) const;

private:
	QVector<Element> m_elements;
};

#endif
