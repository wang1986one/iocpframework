#ifndef __STRING_ALGORITHM_HPP
#define __STRING_ALGORITHM_HPP


#include <algorithm>
#include <functional>
#include "../Utility/UnicodeStl.hpp"

namespace DataStructure
{
	inline void ToUpper(tString &str)
	{
		transform(str.begin(), str.end(), str.begin(), ::toupper);
	}

	inline void ToLower(tString &str)
	{
		transform(str.begin(), str.end(), str.begin(), ::tolower);
	}

	inline void TrimLeft(tString &str)
	{
		str.erase(0, str.find_first_not_of(_T(' ')));
	}

	inline void TrimRight(tString &str)
	{
		str.erase(str.find_last_not_of(_T(' ')) + 1);
	}

	inline void Trim(tString &str)
	{
		str.erase(0, str.find_first_not_of(_T(' ')));
		str.erase(str.find_last_not_of(_T(' ')) + 1);
	}

	inline void erase(tString &str, const TCHAR charactor)
	{
		str.erase(remove_if(str.begin(), str.end(), bind2nd(std::equal_to<tChar>(), charactor)), str.end());
	}

	inline void Replace(tString &str, const tString &strObj, const tString &strDest)
	{
		str.replace(str.find(strObj), strObj.size(), strDest);
	}

	inline bool IsStartWith(tString &str, const tString &strObj)
	{
		return str.compare(0, strObj.size(), strObj) == 0;
	}

	inline bool IsEndWith(tString &str, const tString &strObj)
	{
		return str.compare(str.size() - strObj.size(), strObj.size(), strObj) == 0;
	}

	template<typename T, typename U>
	inline T& Split(T &seq, const tString& str, const U separator)
	{
		tIstringstream iss(str);
		for( std::string s; std::getline(iss, s, separator); )
		{
			T::value_type val;
			tIstringstream isss(s);
			isss >> val;
			seq.insert(seq.end(), val);
		}

		return seq;    
	}

	template<typename T>
	inline void ToNumber(tString &str, T &num)
	{
		tStringstream(str) >> num;
	}
	template<>
	inline void ToNumber<bool>(tString &str, bool &num)
	{
		tStringstream(str) >> std::boolalpha >> num;
	}

	template<typename T>
	inline void ToString(const T &value, tString &str)
	{
		tStringstream temp;
		temp << value;
	
		str = temp.str();
	}
}







#endif