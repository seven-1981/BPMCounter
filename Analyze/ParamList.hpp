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

	ParamList();

	explicit ParamList(LIST list);

	~ParamList();

	ParamList& add(Param* param);

	unsigned int size();

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

	iterator begin();
	iterator end();
	const_iterator cbegin() const;
	const_iterator cend() const;
	void clear();

private:
	VEC m_params;
};

#endif
