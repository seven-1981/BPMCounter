#ifndef _PARAM_H
#define _PARAM_H

#include <string>


enum class ParamType { Bool, Int, Float, String, ConstChar };

class Param
{
public:
	explicit Param(std::string name, ParamType type) : m_type(type), m_name(name) { }
	virtual ~Param() { }

	ParamType type() { return m_type; }
	std::string name() { return m_name; }

protected:
	ParamType m_type;

private:
	std::string m_name;
};

template<ParamType T>
struct Int2Type;
template <> struct Int2Type<ParamType::Bool> { using type = bool; };
template <> struct Int2Type<ParamType::Int> { using type = int; };
template <> struct Int2Type<ParamType::Float> { using type = float; };
template <> struct Int2Type<ParamType::String> { using type = std::string; };
template <> struct Int2Type<ParamType::ConstChar> { using type = const char*; };

template <typename T>
struct Type2Int;
template <> struct Type2Int<bool> { static const ParamType type = ParamType::Bool; };
template <> struct Type2Int<int> { static const ParamType type = ParamType::Int; };
template <> struct Type2Int<float> { static const ParamType type = ParamType::Float; };
template <> struct Type2Int<std::string> { static const ParamType type = ParamType::String; };
template <> struct Type2Int<const char*> { static const ParamType type = ParamType::ConstChar; };

template <ParamType PT, typename IT = typename Int2Type<PT>::type>
class TypedParam : public Param
{
public:
	explicit TypedParam(std::string name, IT value) : Param(name, PT), m_value(value) { }

	template <typename T>
	void set(T value)
	{
		static_assert(std::is_same<IT, T>::value, "No valid type provided for TypedParam::set!");
		m_value = value;
	}

	IT get()
	{
		return m_value;
	}

private:
	IT m_value;
};

#endif
