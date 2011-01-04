#ifndef __HASH_FUNCTION_HPP
#define __HASH_FUNCTION_HPP


#include "../Algorithm.hpp"


namespace DataStructure
{

	// long至少有32bits
	static const int __NumPrimes = 28;
	static const unsigned long __PrimeList[__NumPrimes] = 
	{
		53,         97,           193,         389,       769,
		1543,       3079,         6151,        12289,     24593,
		49157,      98317,        196613,      393241,    786433,
		1572869,    3145739,      6291469,     12582917,  25165843,
		50331653,   100663319,    201326611,   402653189, 805306457, 
		1610612741, 3221225473ul, 4294967291ul
	};

	// 找出28个质数中，最接近大与n的那个质数
	inline unsigned long __NextPrime(unsigned long n)
	{
		const unsigned long *pFirst = __PrimeList;
		const unsigned long *pLast	= __PrimeList + __NumPrimes;
		const unsigned long *pos	= LowerBound(pFirst, pLast, n);	// 需先排序

		return pos == pLast ? *(pLast - 1) : *pos;
	}



	inline size_t __HashString(const char *pStr)
	{
		unsigned long uL = 0;

		for(; *pStr != NULL; ++pStr)
			uL += 5 * uL + *pStr;

		return size_t(uL);
	}

	template<typename Key>
	struct Hash
	{};

	// 偏特化
	template<>
	struct Hash<char *>
	{
		size_t operator()(const char *pStr) const
		{
			return __HashString(pStr);
		}
	};

	template<>
	struct Hash<const char *>
	{
		size_t operator()(const char *pStr) const
		{
			return __HashString(pStr);
		}
	};

	template<>
	struct Hash<char>
	{
		size_t operator()(char x) const
		{
			return x;
		}
	};

	template<>
	struct Hash<unsigned char>
	{
		size_t operator()(unsigned char x) const
		{
			return x;
		}
	};

	template<>
	struct Hash<signed char>
	{
		size_t operator()(unsigned char x) const
		{
			return x;
		}
	};

	template<>
	struct Hash<short>
	{
		size_t operator()(short x) const
		{
			return x;
		}
	};

	template<>
	struct Hash<unsigned short>
	{
		size_t operator()(unsigned short x) const
		{
			return x;
		}
	};

	template<>
	struct Hash<int>
	{
		size_t operator()(int x) const
		{
			return x;
		}
	};

	template<>
	struct Hash<unsigned int>
	{
		size_t operator()(unsigned int x) const
		{
			return x;
		}
	};

	template<>
	struct Hash<long>
	{
		size_t operator()(long x) const
		{
			return x;
		}
	};

	template<>
	struct Hash<unsigned long>
	{
		size_t operator()(unsigned long x) const
		{
			return x;
		}
	};
}



#endif