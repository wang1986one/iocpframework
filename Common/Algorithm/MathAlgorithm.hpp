#ifndef __MATH_ALGORITHM_HPP
#define __MATH_ALGORITHM_HPP


namespace DataStructure
{
	// Accumulate用来计算init和[first,last)内所有元素的总和。注意需要提供初始化值init。
	// 这是因为当[first,last)为空区间时仍能获得一个明确定义的值，如果只希望计算[first,last)中所有的数值总和，
	// 应将init设为0
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

	// InnerProduct计算[first1,last1)和[first2,last2+last1-first1)的内积。
	// 第一个版本将两个区间内的内积结果加上init
	// 第二个版本利用仿函数来取代operator+和operator*
	template<typename InIter1, typename InIter2, typename T>
	inline T InnerProduct(InIter1 first1, InIter1 last1, InIter2 first2, T init)
	{
		// 以第一序列元素的个数为基础，进行循环
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


	// PartialSum计算局部总和，将*first赋值给*out，将*first与*(first+1)的和赋值给*(out+1),以此类推
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
			value = value + *first;	// 前n个元素和
			*++out = value;			// 指定目的端
		}

		return ++out;
	}
	template<typename InIter, typename OutIter, typename T, typename BinaryOperation>
	inline OutIter __PartialSum(InIter first, InIter last, OutIter out, T *, BinaryOperation op)
	{
		T value = *first;

		while(++first != last)
		{
			value = op(value, *first);	// 前n个元素和
			*++out = value;				// 指定目的端
		}

		return ++out;
	}


	// AdjacentDifference计算[first,last)中相邻元素的差额，与PartialSum相对
	template<typename InIter, typename OutIter>
	inline OutIter AdjacentDifference(InIter first, InIter last, OutIter out)
	{
		if( first == last )
			return out;

		*out = *first;		// 首先记录第一个元素
		return __AdjacenetDifference(first, last, out, IteratorTraits<InIter>::pointer());
	}
	template<typename InIter, typename OutIter, typename BinaryOperation>
	inline OutIter AdjacentDifference(InIter first, InIter last, OutIter out, BinaryOperation op)
	{
		if( first == last )
			return out;

		*out = *first;		// 首先记录第一个元素
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


	// Power由SGI提供，并不在STL标准之列。用来计算某数的n冥次方
	template<typename T, typename Integer>
	inline T Power(T x, Integer n)
	{
		return Power(x, n, Multiplies<T>());	// 指定运算型式为乘法
	}
	// 如果指定为乘法运算，则当n>=0时返回x^n
	template<typename T, typename Integer, typename MonoidOperation>
	inline T Power(T x, Integer n, MonoidOperation op)
	{
		if( n == 0 )
			return IdentityElement(op);	// 取出证同元素
		else
		{
			while((n & 1) == 0)			// 如果n为偶数
			{
				n >>= 1;
				x= op(x, x);
			}

			T result = x;
			n >>= 1;

			while(n != 0)				// n不为0时进行运算
			{
				x = op(x, x);
				if( (n & 1) != 0 )
					result = op(result, x);

				n >>= 1;
			}

			return result;
		}
	}


	// iota--用来设定某个区间的内容,使其内的每个元素都从指定的value值开始，呈现递增状态
	template<typename ForwardIterator, typename T>
	void Iota(ForwardIterator first, ForwardIterator last, T value)
	{
		while(first != last)
			*first++ = value++;
	}
}


#endif