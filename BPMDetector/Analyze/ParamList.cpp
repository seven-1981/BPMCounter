#include "ParamList.hpp"


ParamList::ParamList() { }

ParamList::ParamList(LIST list)
{
	using IT = LIST::iterator;
	for (IT it = list.begin(); it != list.end(); ++it)
		add(*it);
}

ParamList::~ParamList() { }

ParamList& ParamList::add(Param* param)
{
	m_params.push_back(param);
	return *this;
}

unsigned int ParamList::size()
{
	return m_params.size();
}

ParamList::iterator ParamList::begin()
{
	return m_params.begin();
}

ParamList::iterator ParamList::end()
{
	return m_params.end();
}

ParamList::const_iterator ParamList::cbegin() const
{
	return m_params.cbegin();
}

ParamList::const_iterator ParamList::cend() const
{
	return m_params.cend();
}

void ParamList::clear()
{
	m_params.clear();
}
