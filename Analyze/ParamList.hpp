#ifndef _PARAMLIST_H
#define _PARAMLIST_H

#include <vector>
#include <cassert>
#include "Param.hpp"


class ParamList
{
public:
	using VEC = std::vector<Param*>;
	using LIST = std::initializer_list<Param*>;

	ParamList() { }

	explicit ParamList(LIST list)
	{
		using IT = LIST::iterator;
		for (IT it = list.begin(); it != list.end(); ++it)
			add(*it);
	}

	~ParamList() { }

	ParamList& add(Param* param)
	{
		m_params.push_back(param);
		return *this;
	}

	unsigned int size()
	{
		return m_params.size();
	}

	template <typename T, ParamType DT = Type2Int<T>::type>
	int get(std::string name, T& param)
	{
		using IT = std::vector<Param*>::iterator;
		using PT = TypedParam<DT>*;
		for (IT it = m_params.begin(); it != m_params.end(); ++it)
		{
			Param* p = *it;
			if (p->name() == name)
			{
				assert(p->type() == DT);
				PT typedParam = dynamic_cast<PT>(p);
				param = typedParam->get();
				return 0;
			}
		}
		return -1;
	}

	using iterator = VEC::iterator;
	using const_iterator = VEC::const_iterator;

	iterator begin() { return m_params.begin(); }
	iterator end() { return m_params.end();	}
	const_iterator cbegin() const { return m_params.cbegin(); }
	const_iterator cend() const	{ return m_params.cend(); }
	void clear() { m_params.clear(); }

private:
	VEC m_params;
};

#endif
