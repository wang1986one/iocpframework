#ifndef __PAIR_HPP
#define __PAIR_HPP


namespace DataStructure
{
	template<typename T1, typename T2>
	struct Pair 
	{
		typedef T1 FirstType;
		typedef T2 SecondType;

		T1 first;		// public
		T2 second;		// public

		Pair() 
			: first(T1())
			, second(T2()) 
		{}
		Pair(const T1& a, const T2& b) 
			: first(a)
			, second(b) 
		{}
		template<typename U1, typename U2>
		Pair(const Pair<U1, U2>& p) 
			: first(p.first)
			, second(p.second) 
		{}

	};

	template<typename T1, typename T2>
	inline bool operator==(const Pair<T1, T2>& x, const Pair<T1, T2>& y)
	{ 
		// 第一元素和第二元素都相等，才相等。
		return x.first == y.first && x.second == y.second; 
	}

	template<typename T1, typename T2>
	inline bool operator<(const Pair<T1, T2>& x, const Pair<T1, T2>& y)
	{ 
		return x.first < y.first || (!(y.first < x.first) && x.second < y.second); 
	}

	template<typename T1, typename T2>
	inline Pair<T1, T2> MakePair(const T1& x, const T2& y)
	{
		return Pair<T1, T2>(x, y);
	}

}




#endif