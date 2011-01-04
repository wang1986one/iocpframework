#ifndef __UN_INITIALIZED_HPP
#define __UN_INITIALIZED_HPP

#include "Pair.hpp"

namespace DataStructure
{

	// ���copy construction��ͬ��assignment������destructor��trivial��������Ч
	template<typename InputIterator, typename ForwardIterator>
	inline ForwardIterator __UninitializedCopyAux(InputIterator first, InputIterator last, ForwardIterator result, __TrueType)
	{
		return Copy(first, last, result);	// copy()
	}

	// �����non-POD �̈́e��ִ�����̾�ת�����º�ʽ��
	template<typename InputIterator, typename ForwardIterator>
	inline ForwardIterator __UninitializedCopyAux(InputIterator first, InputIterator last, ForwardIterator result, __FalseType) 
	{
		ForwardIterator cur = result;
		
		try
		{
			for( ; first != last; ++first, ++cur)
				__Construct(&*cur, *first);	// ���һ��һ��Ԫ�ص�ִ��

			return cur;
		}
		catch(...)
		{
			__Destroy(result, cur);
		}

		return NULL;
	}


	template<typename InputIterator, typename ForwardIterator, typename T>
	inline ForwardIterator __UninitializedCopy(InputIterator first, InputIterator last, ForwardIterator result, T*)
	{
		// ���ϣ����� is_POD() ����õĽY��
		typedef typename __TypeTraits<T>::is_POD_type is_POD;
		return __UninitializedCopyAux(first, last, result, is_POD());
	}

	template<typename InputIterator, typename ForwardIterator>
	inline ForwardIterator __UninitializedCopy(InputIterator first, InputIterator last, ForwardIterator result)
	{
		typedef typename IteratorTraits<ForwardIterator>::value_type value_type;
		value_type *pType = NULL;
		// ���� value_type() ȡ�� first �� value type.
		return __UninitializedCopy(first, last, result, pType);
	}

	// ��������� const char* ���ػ��汾
	inline char* __UninitializedCopy(const char* first, const char* last, char* result) 
	{
		::memmove(result, first, last - first);
		return result + (last - first);
	}

	// ��������� const wchar_t* ���ػ��汾
	inline wchar_t* __UninitializedCopy(const wchar_t* first, const wchar_t* last, wchar_t* result) 
	{
		::memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}


	// ������ __UninitializedCopyN

	template<typename InputIterator, typename size, typename ForwardIterator>
	inline Pair<InputIterator, ForwardIterator> __UninitializedCopyN(InputIterator first, size count, ForwardIterator result, InputIteratorTag) 
	{
		ForwardIterator cur = result;
		
		try
		{
			for( ; count > 0 ; --count, ++first, ++cur) 
				__Construct(&*cur, *first);	

			return Pair<InputIterator, ForwardIterator>(first, cur);
		}
		catch(...)
		{
			__Destroy(result, cur);
		}
	}

	template<typename RandomAccessIterator, typename size, typename ForwardIterator>
	inline Pair<RandomAccessIterator, ForwardIterator> __UninitializedCopyN(RandomAccessIterator first, size count, ForwardIterator result, RandomAccessIteratorTag) 
	{
		RandomAccessIterator last = first + count;
		return MakePair(last, __UninitializedCopy(first, last, result));
	}

	template<typename InputIterator, typename size, typename ForwardIterator>
	inline Pair<InputIterator, ForwardIterator> __UninitializedCopyN(InputIterator first, size count, ForwardIterator result)
	{
		return __UninitializedCopyN(first, count, result, iterator_category(first));
	}


	// ������ __UninitializedFill

	template<typename ForwardIterator, typename T>
	inline void __UninitializedFillAux(ForwardIterator first, ForwardIterator last, const T& x, __TrueType)
	{
		Fill(first, last, x);	// fill()
	}

	// �����non-POD �̈́e
	template<typename ForwardIterator, typename T>
	void __UninitializedFillAux(ForwardIterator first, ForwardIterator last, const T& x, __FalseType)
	{
		ForwardIterator cur = first;
		
		try
		{
			for( ; cur != last; ++cur)
				__Construct(&*cur, x);	// ����һ��һ��Ԫ�صĽ���
		}
		catch(...)
		{
			__Destroy(first, cur);
		}
	}

	template<typename ForwardIterator, typename T, typename T1>
	inline void __UninitializedFill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
	{
		typedef typename __TypeTraits<T1>::is_POD_type is_POD;
		__UninitializedFillAux(first, last, x, is_POD());
	}

	template<typename ForwardIterator, typename T>
	inline void __UninitializedFill(ForwardIterator first, ForwardIterator last, const T& x) 
	{
		__UninitializedFill(first, last, x, value_type(first));
	}



	// __UninitializedFillN

	template<typename ForwardIteratorT, typename size, typename T>
	inline ForwardIteratorT __UninitializedFillNAux(ForwardIteratorT first, size n, const T& x, __TrueType) 
	{
		FillN(first, n, x);
		//::memset(first, x, n * sizeof(typename IteratorTraits<ForwardIteratorT>::value_type));


		return first;
	}

	template<typename ForwardIterator, typename size, typename T>
	ForwardIterator __UninitializedFillNAux(ForwardIterator first, size n, const T& x, __FalseType)
	{
		ForwardIterator cur = first;

		try 
		{
			for( ; n > 0; --n, ++cur)
				__Construct(&*cur, x);
			return cur;
		}
		catch(...)
		{
			__Destroy(first, cur);
		}

		return cur;
	}

	template<typename ForwardIterator, typename size, typename T, typename T1>
	inline ForwardIterator __UninitializedFillN(ForwardIterator first, size n, const T& x, T1) 
	{
		typedef typename __TypeTraits<T1>::is_POD_type is_POD;
		return __UninitializedFillNAux(first, n, x, is_POD());
	}

	template<typename ForwardIterator, typename size, typename T>
	inline ForwardIterator __UninitializedFillN(ForwardIterator first, size n, const T& x)
	{
		return __UninitializedFillN(first, n, x, IteratorTraits<ForwardIterator>::value_type());
	}


	template<typename InputIterator1, typename InputIterator2, typename ForwardIterator>
	inline ForwardIterator __UninitializedCopyCopy(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, ForwardIterator result) 
	{
		ForwardIterator mid = __UninitializedCopy(first1, last1, result);
		
		try
		{
			return __UninitializedCopy(first2, last2, mid);
		}
		catch(...)
		{
			__Destroy(result, mid);
		}
	}


	template<typename ForwardIterator, typename T, typename InputIterator>
	inline ForwardIterator __UninitializedFillCopy(ForwardIterator result, ForwardIterator mid, const T& x, InputIterator first, InputIterator last)
	{
		__UninitializedFill(result, mid, x);
		
		try
		{
			return uninitialized_copy(first, last, mid);
		}
		catch(...)
		{
			__Destroy(result, mid);
		}
	}

	template<typename InputIterator, typename ForwardIterator, typename T>
	inline void __UninitializedCopyFill(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2, const T& x) 
	{
		ForwardIterator mid2 = __UninitializedCopy(first1, last1, first2);
		
		try
		{
			__UninitializedFill(mid2, last2, x);
		}
		catch(...)
		{
			__Destroy(first2, mid2);
		}
	}

}






#endif