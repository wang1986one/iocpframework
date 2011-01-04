#ifndef __MATH_ALGORITHM_HPP
#define __MATH_ALGORITHM_HPP


namespace DataStructure
{
	// Accumulate��������init��[first,last)������Ԫ�ص��ܺ͡�ע����Ҫ�ṩ��ʼ��ֵinit��
	// ������Ϊ��[first,last)Ϊ������ʱ���ܻ��һ����ȷ�����ֵ�����ֻϣ������[first,last)�����е���ֵ�ܺͣ�
	// Ӧ��init��Ϊ0
	template<typename InIter, typename T>
	inline T Accumulate(InIter first, InIter last, T init)
	{
		for(; first != last; ++first)
			init = init + *first;

		return init;
	}
	template<typename InIter, typename T, typename BinaryOperation>
	inline T Accumulate(InIter first, InIter last, T init, BinaryOperation op)
	{
		for(; first != last; ++first)
			init = op(init, *first);

		return init;
	}

	// InnerProduct����[first1,last1)��[first2,last2+last1-first1)���ڻ���
	// ��һ���汾�����������ڵ��ڻ��������init
	// �ڶ����汾���÷º�����ȡ��operator+��operator*
	template<typename InIter1, typename InIter2, typename T>
	inline T InnerProduct(InIter1 first1, InIter1 last1, InIter2 first2, T init)
	{
		// �Ե�һ����Ԫ�صĸ���Ϊ����������ѭ��
		for(; first1 != last1; ++first1, ++first2)
			init = init + (*first1) * (*first2);

		return init;
	}
	template<typename InIter1, typename InIter2, typename T, typename BinaryOperation1, typename BinaryOperation2>
	inline T InnerProduct(InIter1 first1, InIter1 last1, InIter2 first2, T init, BinaryOperation1 op1, BinaryOperation2 op2)
	{
		for(; first1 != last1; ++first1, ++first2)
			init = op1(init , op2(*first1, *first2));

		return init;
	}


	// PartialSum����ֲ��ܺͣ���*first��ֵ��*out����*first��*(first+1)�ĺ͸�ֵ��*(out+1),�Դ�����
	template<typename InIter, typename OutIter>
	inline OutIter PartialSum(InIter first, InIter last, OutIter out)
	{
		if( first == last )
			return out;

		*out = *first;
		return __PartialSum(first, last, out, IteratorTraits<InIter>::pointer());
	}
	template<typename InIter, typename OutIter, typename BinaryOperation>
	inline OutIter PartialSum(InIter first, InIter last, OutIter out, BinaryOperation op)
	{
		if( first == last )
			return out;

		*out = *first;
		return __PartialSum(first, last, out, IteratorTraits<InIter>::pointer(), op);
	}

	template<typename InIter, typename OutIter, typename T>
	inline OutIter __PartialSum(InIter first, InIter last, OutIter out, T *)
	{
		T value = *first;

		while(++first != last)
		{
			value = value + *first;	// ǰn��Ԫ�غ�
			*++out = value;			// ָ��Ŀ�Ķ�
		}

		return ++out;
	}
	template<typename InIter, typename OutIter, typename T, typename BinaryOperation>
	inline OutIter __PartialSum(InIter first, InIter last, OutIter out, T *, BinaryOperation op)
	{
		T value = *first;

		while(++first != last)
		{
			value = op(value, *first);	// ǰn��Ԫ�غ�
			*++out = value;				// ָ��Ŀ�Ķ�
		}

		return ++out;
	}


	// AdjacentDifference����[first,last)������Ԫ�صĲ���PartialSum���
	template<typename InIter, typename OutIter>
	inline OutIter AdjacentDifference(InIter first, InIter last, OutIter out)
	{
		if( first == last )
			return out;

		*out = *first;		// ���ȼ�¼��һ��Ԫ��
		return __AdjacenetDifference(first, last, out, IteratorTraits<InIter>::pointer());
	}
	template<typename InIter, typename OutIter, typename BinaryOperation>
	inline OutIter AdjacentDifference(InIter first, InIter last, OutIter out, BinaryOperation op)
	{
		if( first == last )
			return out;

		*out = *first;		// ���ȼ�¼��һ��Ԫ��
		return __AdjacenetDifference(first, last, out, IteratorTraits<InIter>::pointer(), op);
	}
	template<typename InIter, typename OutIter, typename T>
	inline OutIter __AdjacenetDifference(InIter first, InIter last, OutIter out, T *)
	{
		T value = *first;

		while(++first != last)
		{
			T tmp	= *first;
			*++out	= tmp - value;
			value	= tmp;
		}

		return out;
	}
	template<typename InIter, typename OutIter, typename T, typename BinaryOperation>
	inline OutIter __AdjacenetDifference(InIter first, InIter last, OutIter out, T *, BinaryOperation op)
	{
		T value = *first;

		while(++first != last)
		{
			T tmp	= *first;
			*++out	= op(tmp, value);
			value	= tmp;
		}

		return out;
	}


	// Power��SGI�ṩ��������STL��׼֮�С���������ĳ����nڤ�η�
	template<typename T, typename Integer>
	inline T Power(T x, Integer n)
	{
		return Power(x, n, Multiplies<T>());	// ָ��������ʽΪ�˷�
	}
	// ���ָ��Ϊ�˷����㣬��n>=0ʱ����x^n
	template<typename T, typename Integer, typename MonoidOperation>
	inline T Power(T x, Integer n, MonoidOperation op)
	{
		if( n == 0 )
			return IdentityElement(op);	// ȡ��֤ͬԪ��
		else
		{
			while((n & 1) == 0)			// ���nΪż��
			{
				n >>= 1;
				x= op(x, x);
			}

			T result = x;
			n >>= 1;

			while(n != 0)				// n��Ϊ0ʱ��������
			{
				x = op(x, x);
				if( (n & 1) != 0 )
					result = op(result, x);

				n >>= 1;
			}

			return result;
		}
	}


	// iota--�����趨ĳ�����������,ʹ���ڵ�ÿ��Ԫ�ض���ָ����valueֵ��ʼ�����ֵ���״̬
	template<typename ForwardIterator, typename T>
	void Iota(ForwardIterator first, ForwardIterator last, T value)
	{
		while(first != last)
			*first++ = value++;
	}
}


#endif