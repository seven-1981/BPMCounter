#include "FrequencySpectrum.hpp"


FrequencySpectrum::FrequencySpectrum(unsigned int numPoints) : m_elements(numPoints) { }

void FrequencySpectrum::reset()
{
	for (iterator it = begin(); it != end(); ++it)
		*it = Element();
}

unsigned int FrequencySpectrum::count() const
{
	return m_elements.count();
}

FrequencySpectrum::Element& FrequencySpectrum::operator[](unsigned int index)
{
	return m_elements[index];
}

const FrequencySpectrum::Element& FrequencySpectrum::operator[](unsigned int index) const
{
	return m_elements[index];
}

FrequencySpectrum::iterator FrequencySpectrum::begin()
{
	return m_elements.begin();
}

FrequencySpectrum::iterator FrequencySpectrum::end()
{
	return m_elements.end();
}

FrequencySpectrum::const_iterator FrequencySpectrum::cbegin() const
{
	return m_elements.cbegin();
}

FrequencySpectrum::const_iterator FrequencySpectrum::cend() const
{
	return m_elements.cend();
}
