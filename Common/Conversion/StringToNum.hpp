#ifndef __CONVERSION_STRING_TO_NUMBER_HPP
#define __CONVERSION_STRING_TO_NUMBER_HPP


#include "../Constraints.hpp"


namespace DataStructure
{

	/*
		模板函数不具有偏特化，只能具有全特化，即重载。一个看似函数模板偏特化的函数模板实际上是一个单独的主函数模板。
		重载规则：
			1. 普通函数的优先级最高
			2. 非特化的模板函数（主模板）其次
			3. 特化模板函数最低
		
		只有在某个主模板被重在决议选中的前提下，其特化模板版本才可能被使用。编译器在选择主模板的时候并不关心他是否有某个特化版本
	*/

	// Function Str2NumberHelper

	template<typename NumType, typename char_type>
	NumType Str2NumberHelper(const char_type *szVal, char_type **pEnd, int nRadix);

	template<>
	inline __int64 Str2NumberHelper<__int64, char>(const char *szVal, char **pEnd, int nRadix)
	{
		return _strtoi64(szVal, pEnd, nRadix);
	}

	template<>
	inline unsigned __int64 Str2NumberHelper<unsigned __int64, char>(const char *szVal, char **pEnd, int nRadix)
	{
		return _strtoui64(szVal, pEnd, nRadix);
	}

	template<>
	inline long Str2NumberHelper<long, char>(const char *szVal, char **pEnd, int nRadix)
	{
		return strtol(szVal, pEnd, nRadix);
	}

	template<>
	inline unsigned long Str2NumberHelper<unsigned long, char>(const char *szVal, char **pEnd, int nRadix)
	{
		return strtoul(szVal, pEnd, nRadix);
	}

	template<>
	inline int Str2NumberHelper<int, char>(const char *szVal, char **pEnd, int nRadix)
	{
		return strtol(szVal, pEnd, nRadix);
	}

	template<>
	inline unsigned int Str2NumberHelper<unsigned int, char>(const char *szVal, char **pEnd, int nRadix)
	{
		return strtoul(szVal, pEnd, nRadix);
	}

	template<>
	inline __int64 Str2NumberHelper<__int64, wchar_t>(const wchar_t *szVal, wchar_t **pEnd, int nRadix)
	{
		return _wcstoi64(szVal, pEnd, nRadix);
	}

	template<>
	inline unsigned __int64 Str2NumberHelper<unsigned __int64, wchar_t>(const wchar_t *szVal, wchar_t **pEnd, int nRadix)
	{
		return _wcstoui64(szVal, pEnd, nRadix);
	}

	template<>
	inline long Str2NumberHelper<long, wchar_t>(const wchar_t *szVal, wchar_t **pEnd, int nRadix)
	{
		return wcstol(szVal, pEnd, nRadix);
	}

	template<>
	inline unsigned long Str2NumberHelper<unsigned long, wchar_t>(const wchar_t *szVal, wchar_t **pEnd, int nRadix)
	{
		return wcstoul(szVal, pEnd, nRadix);
	}

	template<>
	inline int Str2NumberHelper<int, wchar_t>(const wchar_t *szVal, wchar_t **pEnd, int nRadix)
	{
		return wcstol(szVal, pEnd, nRadix);
	}

	template<>
	inline unsigned int Str2NumberHelper<unsigned int, wchar_t>(const wchar_t *szVal, wchar_t **pEnd, int nRadix)
	{
		return wcstoul(szVal, pEnd, nRadix);
	}


	// Function Str2NumberHelper 

	template<typename NumType, typename char_type>
	NumType Str2NumberHelper(const char_type *szVal, char_type **pEnd);

	template<>
	inline double Str2NumberHelper<double, char>(const char *szVal, char **pEnd)
	{
		return strtod(szVal, pEnd);
	}

	template<>
	inline double Str2NumberHelper<double, wchar_t>(const wchar_t *szVal, wchar_t **pEnd)
	{
		return wcstod(szVal, pEnd);
	}



	// Str2Num Interface
	template<typename NumType, typename char_type>
	inline NumType Str2Num(const char_type *szVal, char_type **pEnd, int nRadix)
	{
		return Str2NumberHelper<NumType, char_type>(szVal, pEnd, nRadix);
	}

	template<typename NumType, typename char_type>
	inline double Str2Num(const char_type *szVal, char_type **pEnd)
	{
		return Str2NumberHelper<NumType, char_type>(szVal, pEnd);
	}
}







#endif