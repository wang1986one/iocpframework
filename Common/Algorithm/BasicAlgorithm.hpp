#ifndef __BASIC_ALGORITHM_HPP
#define __BASIC_ALGORITHM_HPP

#include "../Basic.hpp"
#include "../Constraints.hpp"

namespace DataStructure
{
	// Mismatch--比较两个序列，指出两者之间的第一个不匹配点。
	// 返回一对迭代器，分别指向两序列中的不匹配点。
	// 如果第二序列的元素个数比第一序列多，多出来的元素忽略不计，如果第二序列的元素个数少于第一序列，则会发生不可预期的行为
	template<typename InIter1, typename InIter2>
	inline Pair<InIter1, InIter2> Mismatch(InIter1 first1, InIter1 last1, InIter2 first2)
	{
		while(first1 != last1 && *first1 == *first2)
		{
			++first1;
			++first2;
		}

		return Pair<InIter1, InIter2>(first1, first2);
	}
	template<typename InIter1, typename InIter2, typename BinaryOperation>
	inline Pair<InIter1, InIter2> Mismatch(InIter1 first1, InIter1 last1, InIter2 first2, BinaryOperation op)
	{
		while(first1 != last1 && op(*first1, *first2))
		{
			++first1;
			++first2;
		}

		return Pair<InIter1, InIter2>(first1, first2);
	}


	// Equal--如果两个序列在[first,last)区间内相等，则返回true。如果第二个序列的元素个数更多，则忽略多余的元素
	// 若要保证两个序列完全相同则需要先判断其元素个数是否相等
	// vec1.size() == vec2.size() && Equal(vec1.begin(), vec1.end(), vec2.begin())
	template<typename InIter1, typename InIter2>
	inline bool Equal(InIter1 first1, InIter1 last1, InIter2 first2)
	{
		for(; first1 != last1; ++first1, ++first2)
		{
			if( *first1 != *first2 )
				return false;
		}

		return true;
	}
	template<typename InIter1, typename InIter2, typename BinaryOperation>
	inline bool Equal(InIter1 first1, InIter1 last1, InIter2 first2, BinaryOperation op)
	{
		for(; first1 != last1; ++first1, ++first2)
		{
			if( !op(*first1, *first2) )
				return false;
		}

		return true;
	}


	// Fill--将[first,last)内的所有元素改填新值
	// 由于每次迭代进行的都是覆写操作，所以一旦操作区间超越了容器大小，则会造成不可预期的结果
	// 可以利用inserter()产生一个适配器来进行插入
	// FillN(inserter(iv, iv.begin()), 5, 7)
	template<typename ForwardIterator, typename T>
	void Fill(ForwardIterator first, ForwardIterator last, const T &value)
	{
		for(; first != last; ++first)
			*first = value;
	}
	template<typename ForwardIteratorT, typename size, typename T>
	void FillN(ForwardIteratorT first, size n, const T &value)
	{
		for(; n > 0; --n, ++first)
			*first = value;
	}

	// swap--交换两个对象的内容
	template<typename T>
	inline void Swap(T &a, T &b)
	{
		T tmp	= a;
		a		= b;
		b		= tmp;
	}

	// IterSwap--将两个ForwardIterator所指的对象对调
	template<typename ForwardIterator1, typename ForwardIterator2>
	inline void IterSwap(ForwardIterator1 left, ForwardIterator2 right)
	{
		typename IteratorTraits<ForwardIterator1>::value_type tmp = *left;
		*left = *right;
		*right = tmp;
	}
	

	// Max--取两个对象中的较大值
	template<typename T>
	inline const T &Max(const T &a, const T &b)
	{
		return a < b ? b : a;
	}
	template<typename T, typename Compare>
	inline const T &Max(const T &a, const T &b, Compare compare)
	{
		return compare(a, b) ? b : a;
	}

	// Min--去两个对象中较小值
	template<typename T>
	inline const T &Min(const T &a, const T &b)
	{
		return a > b ? b : a;
	}
	template<typename T, typename Compare>
	inline const T &Min(const T &a, const T &b, Compare compare)
	{
		return compare(a, b) ? b : a;
	}


	// Lexicographical_compare--以字典排列方式对两个序列[first1, last1)和[first2, last2)进行比较
	// 比较操作针对两序列中的对应位置上的元素进行，并持续到某组对应元素不相等
	template<typename InIter1, typename InIter2>
	bool LexicographicalCompare(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2)
	{
		for(; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if( *first1 < *first2 )			// 第一序列元素之值小于第二序列元素值
				return true;
			if( *first2 < *first1 )			// 第一序列元素之值大于第二序列元素值
				return false;
		}

		// 如果第一序列达到尾端儿第二序列尚有余额，那么低一些列小于第二序列
		return first1 == last1 && first2 != last2;
	}
	template<typename InIter1, typename InIter2, typename Compare>
	bool LexicographicalCompare(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2, Compare comp)
	{
		for(; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if( comp(*first1, *first2) )			// 第一序列元素之值小于第二序列元素值
				return true;
			if( comp(*first2, *first1) )			// 第一序列元素之值大于第二序列元素值
				return false;
		}

		// 如果第一序列达到尾端儿第二序列尚有余额，那么低一些列小于第二序列
		return first1 == last1 && first2 != last2;
	}


	//
	template<typename InIter, typename OutIter>
	inline OutIter Copy(InIter first, InIter last, OutIter out)
	{
		return __CopyDispatch<InIter, OutIter>()(first, last, out);
	}
	inline char *Copy(const char *first, const char *last, char *out)
	{
		::memmove(out, first, last - first);

		return out + (last - first);
	}
	inline wchar_t *Copy(const wchar_t *first, const wchar_t *last, wchar_t *out)
	{
		::memmove(out, first, sizeof(wchar_t) *(last - first));

		return out + (last - first);
	}
	template<typename InIter, typename OutIter>
	struct __CopyDispatch
	{
		OutIter operator()(InIter first, InIter last, OutIter out)
		{
			return __Copy(first, last, out, IteratorTraits<InIter>::iterator_category());
		}
	};
	template<typename T>
	struct __CopyDispatch<T *, T *>
	{
		T * operator()(T * first, T * last, T * out)
		{
			typedef typename __TypeTraits<T>::has_trivial_assignment_operator t;

			return __CopyT(first, last, out, t());
		}
	};
	template<typename T>
	struct __CopyDispatch<const T *, T *>
	{
		T * operator()(const T * first, const T * last, T * out)
		{
			typedef typename __TypeTraits<T>::has_trivial_assignment_operator t;

			return __CopyT(first, last, out, t());
		}
	};

	template<typename InIter, typename OutIter>
	inline OutIter __Copy(InIter first, InIter last, OutIter out, InputIteratorTag)
	{
		for(; first != last; ++out, ++first)
			*out = *first;

		return out;
	}
	template<typename RandomAccessiterator, typename OutIter>
	inline OutIter __Copy(RandomAccessiterator first, RandomAccessiterator last, OutIter out, RandomAccessIteratorTag)
	{
		return __CopyD(first, last, out, Type2Type<typename IteratorTraits<RandomAccessiterator>::difference_type>());
	}

	template<typename T>
	inline T *__CopyT(const T *first, const T *last, T *out, __TrueType)
	{
		::memmove(out, first, sizeof(T) * (last - first));
		return out + (last - first);
	}
	template<typename T>
	inline T *__CopyT(const T *first, const T *last, T *out, __FalseType)
	{
		return __CopyD(first, last, out, reinterpret_cast<ptrdiff_t *>(0));
	}
	template<typename RandomAccessiterator, typename OutIter>
	inline OutIter __CopyD(RandomAccessiterator first, RandomAccessiterator last, OutIter out, Type2Type<typename IteratorTraits<RandomAccessiterator>::difference_type> )
	{
		typedef typename Type2Type<typename IteratorTraits<RandomAccessiterator>::difference_type>::value_type value_type;
		for(value_type n = last - first; n > 0; --n, ++out, ++first)
			*out = *first;

		return out;
	}


	// CopyBackward--以逆行的方向复制*(result - 1) = *(last - 1)  *(result - 2) = *(last - 2)
	template<typename BidirectionalIterator1, typename BidirectionalIterator2>
	inline BidirectionalIterator2 __CopyBackward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) 
	{
		while( first != last ) 
			*--result = *--last;

		return result;
	}


	template<typename BidirectionalIterator1, typename BidirectionalIterator2>
	struct __CopyBackwardDispatch
	{
		BidirectionalIterator2 operator()(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) 
		{
			return __CopyBackward(first, last, result);
		}
	};
	template<typename T>
	struct __CopyBackwardDispatch<T*, T*>
	{
		T* operator()(T* first, T* last, T* result)
		{
			typedef typename __TypeTraits<T>::has_trivial_assignment_operator t; 
			return __CopyBackwardT(first, last, result, t());
		}
	};
	template<typename T>
	struct __CopyBackwardDispatch<const T*, T*>
	{
		T* operator()(const T* first, const T* last, T* result) 
		{
			typedef typename __TypeTraits<T>::has_trivial_assignment_operator t; 
			return __CopyBackwardT(first, last, result, t());
		}
	};

	template<typename T>
	inline T* __CopyBackwardT(const T* first, const T* last, T* result, __TrueType) 
	{
		const ptrdiff_t N = last - first;
		::memmove(result - N, first, sizeof(T) * N);
			
		return result - N;
	}
	template<typename T>
	inline T* __CopyBackwardT(const T* first, const T* last, T* result, __FalseType) 
	{
		return __CopyBackward(first, last, result);
	}

	template<typename BidirectionalIterator1, typename BidirectionalIterator2>
	inline BidirectionalIterator2 CopyBackward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) 
	{
		return __CopyBackwardDispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, result);
	}

}



#endif