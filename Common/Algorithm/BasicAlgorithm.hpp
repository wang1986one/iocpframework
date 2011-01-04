#ifndef __BASIC_ALGORITHM_HPP
#define __BASIC_ALGORITHM_HPP

#include "../Basic.hpp"
#include "../Constraints.hpp"

namespace DataStructure
{
	// Mismatch--�Ƚ��������У�ָ������֮��ĵ�һ����ƥ��㡣
	// ����һ�Ե��������ֱ�ָ���������еĲ�ƥ��㡣
	// ����ڶ����е�Ԫ�ظ����ȵ�һ���ж࣬�������Ԫ�غ��Բ��ƣ�����ڶ����е�Ԫ�ظ������ڵ�һ���У���ᷢ������Ԥ�ڵ���Ϊ
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


	// Equal--�������������[first,last)��������ȣ��򷵻�true������ڶ������е�Ԫ�ظ������࣬����Զ����Ԫ��
	// ��Ҫ��֤����������ȫ��ͬ����Ҫ���ж���Ԫ�ظ����Ƿ����
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


	// Fill--��[first,last)�ڵ�����Ԫ�ظ�����ֵ
	// ����ÿ�ε������еĶ��Ǹ�д����������һ���������䳬Խ��������С�������ɲ���Ԥ�ڵĽ��
	// ��������inserter()����һ�������������в���
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

	// swap--�����������������
	template<typename T>
	inline void Swap(T &a, T &b)
	{
		T tmp	= a;
		a		= b;
		b		= tmp;
	}

	// IterSwap--������ForwardIterator��ָ�Ķ���Ե�
	template<typename ForwardIterator1, typename ForwardIterator2>
	inline void IterSwap(ForwardIterator1 left, ForwardIterator2 right)
	{
		typename IteratorTraits<ForwardIterator1>::value_type tmp = *left;
		*left = *right;
		*right = tmp;
	}
	

	// Max--ȡ���������еĽϴ�ֵ
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

	// Min--ȥ���������н�Сֵ
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


	// Lexicographical_compare--���ֵ����з�ʽ����������[first1, last1)��[first2, last2)���бȽ�
	// �Ƚϲ�������������еĶ�Ӧλ���ϵ�Ԫ�ؽ��У���������ĳ���ӦԪ�ز����
	template<typename InIter1, typename InIter2>
	bool LexicographicalCompare(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2)
	{
		for(; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if( *first1 < *first2 )			// ��һ����Ԫ��ֵ֮С�ڵڶ�����Ԫ��ֵ
				return true;
			if( *first2 < *first1 )			// ��һ����Ԫ��ֵ֮���ڵڶ�����Ԫ��ֵ
				return false;
		}

		// �����һ���дﵽβ�˶��ڶ�������������ô��һЩ��С�ڵڶ�����
		return first1 == last1 && first2 != last2;
	}
	template<typename InIter1, typename InIter2, typename Compare>
	bool LexicographicalCompare(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2, Compare comp)
	{
		for(; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if( comp(*first1, *first2) )			// ��һ����Ԫ��ֵ֮С�ڵڶ�����Ԫ��ֵ
				return true;
			if( comp(*first2, *first1) )			// ��һ����Ԫ��ֵ֮���ڵڶ�����Ԫ��ֵ
				return false;
		}

		// �����һ���дﵽβ�˶��ڶ�������������ô��һЩ��С�ڵڶ�����
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


	// CopyBackward--�����еķ�����*(result - 1) = *(last - 1)  *(result - 2) = *(last - 2)
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