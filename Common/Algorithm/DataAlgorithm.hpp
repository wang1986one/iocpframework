#ifndef __DATA_ALGORITHM_HPP
#define __DATA_ALGORITHM_HPP



namespace DataStructure
{
	// AdjacentFind--�ҳ���һ����������������Ԫ��
	template<typename ForwardIterator>
	inline ForwardIterator AdjacentFind(ForwardIterator first, ForwardIterator last)
	{
		if( first == last )
			return last;

		ForwardIterator next = first;
		while( ++next != last )
		{
			// ����ҵ�����Ԫ��ֵ��ȣ��ͽ���
			if( *first == *next )	
				return first;

			first = next;
		}

		return last;
	}
	template<typename ForwardIterator, typename BinaryPredicate>
	inline ForwardIterator AdjacentFind(ForwardIterator first, ForwardIterator last, BinaryPredicate op)
	{
		if( first == last )
			return last;

		ForwardIterator next = first;
		while( ++next != last )
		{
			// ����ҵ�����Ԫ��ֵ��ȣ��ͽ���
			if( op(*first, *next) )	
				return first;

			first = next;
		}

		return last;
	}


	// Count--����Equality����������[first,last)�����ڵ�ÿ��Ԫ��������ָ��ֵ�Ƚϣ���������value���Ԫ�صĸ���
	template<typename InIter, typename T>
	inline typename IteratorTraits<InIter>::difference_type Count(InIter first, InIter last, const T &value)
	{
		typename IteratorTraits<InIter>::difference_type n = 0;

		for(; first != last; ++first)
		{
			if( *first == value )
				++n;
		}

		return n;
	}
	// CountIf--���ƶ�����Predʵʩ��[first, last)�����ڵ�ÿһ��Ԫ��
	template<typename InIter, typename Predicate>
	inline typename IteratorTraits<InIter>::difference_type CountIf(InIter first, InIter last, Predicate pred)
	{
		typename IteratorTraits<InIter>::difference_type n = 0;

		for(; first != last; ++first)
		{
			if( pred(*first) )
				++n;
		}

		return n;
	}

	// Find--����Equality��������ѭ������[first, last)�ڵ�����Ԫ�أ��ҳ���һ��ƥ��"��ͬ����"��
	template<typename InIter, typename T>
	inline InIter Find(InIter first, InIter last, const T &value)
	{
		while( first != last && *first != value )
			++first;

		return first;
	}
	// FindIf--����ָ����Pred����������ѭ������
	template<typename InIter, typename Predicate>
	inline InIter FindIf(InIter first, InIter last, Predicate pred)
	{
		while( first != last && !pred(*first) )
			++first;

		return first;
	}
	// FindEnd--������[first1, last1)�У���������[first2,last2)�����һ�γ��ֵ㡣
	// �������1�ڲ�������ȫƥ������2�������У�����last1
	template<typename ForwardIterator1, typename ForwardIterator2>
	inline ForwardIterator1 FindEnd(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
	{
		typedef typename IteratorTraits<ForwardIterator1>::iterator_category category1;
		typedef typename IteratorTraits<ForwardIterator2>::iterator_category category2;

		return __FindEnd(first1, last1, first2, last2, category1(), category2());
	}
	template<typename ForwardIterator1, typename ForwardIterator2>
	inline ForwardIterator1 __FindEnd(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, ForwardIteratorTag, ForwardIteratorTag)
	{
		// ����Ŀ��Ϊ��
		if( first2 == last2 )
			return last1;
		else
		{
			ForwardIterator1 result = last1;
			while(1)
			{
				// ����Search����ĳ�������е��״γ��ֵ�
				ForwardIterator1 newRes = search(first1, last1, first2, last2);
				
				// û�ҵ�
				if( newRes == last1 )
					return result;
				else
				{
					result = newRes;
					first1 = newRes;

					++first1;
				}
			}
		}
	}
	template<typename BidirectionalIterator1, typename BidirectionalIterator2>
	inline BidirectionalIterator1 __FindEnd(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2, BidirectionalIteratorTag, BidirectionalIteratorTag)
	{
		// ���÷������
		typedef ReverseIterator<BidirectionalIterator1> reverse1;
		typedef ReverseIterator<BidirectionalIterator2> reverse2; 

		reverse1 rlast1(first1);
		reverse2 rlast2(first2);

		reverse1 rresult = Search(reverse1(last1), rlast1, reverse2(last2), rlast2);

		// û�ҵ�
		if( rresult == rlast1 )
			return last1;
		else
		{
			// ת������������
			BidirectionalIterator1 result = rresult.Base();
			// �����������е���ͷ��
			Advance(result, -DistanceFunc(first2, last2));

			return result;
		}
	}

	// FindFirstOf--��[first2,last2)�����ڵ�ĳЩԪ����Ϊ����Ŀ�꣬Ѱ����[first1,last1)�����ڵ�һ�γ��ֵص�
	template<typename InIter, typename ForwardIterator>
	inline InIter FindFirstOf(InIter first1, InIter last1, ForwardIterator first2, ForwardIterator last2)
	{
		for(; first1 != last1; ++first1)
		{
			for(ForwardIterator iter = first2; iter != last2; ++iter)
			{
				// �������1��Ԫ��������2��Ԫ�����
				if( *first1 == *iter )
					return first1;
			}
		}

		return last1;
	}
	template<typename InIter, typename ForwardIterator, typename BinaryPredicate>
	inline InIter FindFirstOf(InIter first1, InIter last1, ForwardIterator first2, ForwardIterator last2, BinaryPredicate comp)
	{
		for(; first1 != last1; ++first1)
		{
			for(ForwardIterator iter = first2; iter != last2; ++iter)
			{
				// �������1��Ԫ��������2��Ԫ�����
				if( comp(*first1, *iter) )
					return first1;
			}
		}

		return last1;
	}


	// ForEach--���º���ʩ����[first,last)������ÿһ��Ԫ����,�÷º������ɸı�Ԫ������,��Ϊfirst��last����InputIterator��
	// �����Ҫ�޸�Ԫ�����ݣ���ʹ��Transform
	template<typename InIter, typename Function>
	inline Function ForEach(InIter first, InIter last, Function func)
	{
		for(; first != last; ++first)
			func(*first);

		return func;
	}


	// Generate--���º�������������д��[first,last)���������Ԫ����
	template<typename ForwardIterator, typename Generator>
	inline void Generate(ForwardIterator first, ForwardIterator last, Generator gen)
	{
		for(; first != last; ++first)
			*first = gen();
	}
	template<typename OutputIterator, typename size, typename Generator>
	inline OutputIterator GenerateN(OutputIterator first, size n, Generator gen)
	{
		for(; n > 0; --n, ++first)
			*first = gen();

		return first;
	}


	// Remove--�Ƴ�[first,last)��������value��ȵ�Ԫ�ء���������������ɾ��Ԫ��(������С��δ�ı�),
	// ���ǽ�ÿһ������value��ȵ�Ԫ��һ�θ�ֵΪfirst֮��Ŀռ�
	template<typename ForwardIterator, typename T>
	inline ForwardIterator Remove(ForwardIterator first, ForwardIterator last, const T &value)
	{
		// ����ѭ�����ҵ�һ����ȵ�Ԫ��
		first = Find(first, last, value);
		ForwardIterator next = first;

		// ����RemoveCopy�����¾������ص�
		return first == last ? first : RemoveCopy(++next, last, first, value);
	}

	// RemoveCopy--�Ƴ�[first,last)������������value��ȵ�Ԫ��,����������Ƶ�һ����result��־��ʼλ�õ�������
	// ���������Ժ�ԭ�����ص�
	template<typename InIter, typename OutIter, typename T>
	inline OutIter RemoveCopy(InIter first, InIter last, OutIter result, const T &value)
	{
		for(; first != last; ++first)
		{
			// ����������ֵ��������
			if( *first != value )
			{
				*result = *first;
				++result;
			}
		}

		return result;
	}

	// Removeif--�Ƴ�[first,last)���������б��º���pred����Ϊtrue��Ԫ��
	template<typename ForwardIterator, typename Predicate>
	inline ForwardIterator RemoveIf(ForwardIterator first, ForwardIterator last, Predicate pred)
	{
		// ����ѭ�����ҵ�һ��ƥ����
		first = FindIf(first, last, pred);

		ForwardIterator next = first;

		return first == last ? first : RemoveCopyIf(++next, last, first, pred);
	}
	template<typename InIter, typename OutIter, typename Predicate>
	inline OutIter RemoveCopyIf(InIter first, InIter last, OutIter result,Predicate pred)
	{
		for(; first != last; ++first)
		{
			// ����������ֵ��������
			if( !pred(*first) )
			{
				*result = *first;
				++result;
			}
		}

		return result;
	}


	// Replace--��[first,last)���������е�oldValue����newValue����
	template<typename ForwardIterator, typename T>
	inline void Replace(ForwardIterator first, ForwardIterator last, const T &oldValue, const T &newValue)
	{
		for(; first != last; ++first)
		{
			if( *first == oldValue )
				*first = newValue;
		}
	}

	// ReplaceCopy--����Replace����ͬ���������лᱻ���Ƶ�result��ָ������
	template<typename InIter, typename OutIter, typename T>
	inline OutIter ReplaceCopy(InIter first, InIter last, OutIter result, const T &oldValue, const T &newValue)
	{
		for(; first != last; ++first, ++result)
		{
			*result = (*first == oldValue ? newValue : *first);
		}

		return result;
	}

	template<typename ForwardIterator, typename Predicate, typename T>
	inline void ReplaceIf(ForwardIterator first, ForwardIterator last, Predicate pred, const T &newValue)
	{
		for(; first != last; ++first)
		{
			if( pred(*first) )
				*first = newValue;
		}
	}

	template<typename InIter, typename OutIter, typename Predicate, typename T>
	inline OutIter ReplaceCopyIf(InIter first, InIter last, OutIter result, Predicate pred, const T &newValue)
	{
		for(; first != last; ++first, ++result)
		{
			*result = (pred(*first) ? newValue : *first);
		}

		return result;
	}


	// Reverse--��[first,last)��Ԫ����ԭ�����еߵ�����
	template<typename BidirectionalIterator>
	inline void Reverse(BidirectionalIterator first, BidirectionalIterator last)
	{
		__Reverse(first, last, IteratorTraits<BidirectionalIterator>::iterator_category());
	}
	template<typename BidirectionalIterator>
	inline void __Reverse(BidirectionalIterator first, BidirectionalIterator last, BidirectionalIteratorTag)
	{
		while(true)
		{
			if( first == last || first == --last )
				return;
			else
				IterSwap(first++, last);
		}
	}
	template<typename RandomAccessIterator>
	inline void __Reverse(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIteratorTag)
	{
		while(first < last)
			IterSwap(first++, --last);
	}

	// ReplaceCopy--����Replace�����������з���result������
	template<typename BidirectionalIterator, typename OutIter>
	inline OutIter ReverseCopy(BidirectionalIterator first, BidirectionalIterator last, OutIter result)
	{
		while(first != last)
		{
			--last;
			*result = *last;
			++result;
		}

		return result;
	}


	// Rotate--��[first,last)�ڵ�Ԫ�غ�[middle,last)�ڵ�Ԫ�ػ���.
	// middle��ָ��Ԫ�ػ��Ϊ�����ĵ�һ��Ԫ��
	template<typename ForwardIterator>
	inline void Rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last)
	{
		if( first == middle || middle == last )
			return;

		__Rotate(first, middle, last, IteratorTraits<ForwardIterator>::difference_type(), IteratorTraits<ForwardIterator>::iterator_category());
	}
	template<typename ForwardIterator, typename Distance>
	inline void __Rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, Distance, ForwardIteratorTag)
	{
		for(ForwardIterator iter = middle; ;)
		{
			// ǰ�κͺ�ε�Ԫ��һһ����
			IterSwap(first, iter);
			++first;
			++iter;

			// �ж�ǰ��[first,middle)����[middle, last)�ĸ����Ƚ���
			if( first == middle )
			{
				// ���ǰ������һͬ����,������һ�����
				if( iter == last )
					return;

				// �����������������
				middle = iter;
			}
			else if( iter == last )
			{
				// ����Ƚ���
				iter = middle;
			}
		}
	}
	template<typename BidirectionalIterator, typename Distance>
	inline void __Rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance, BidirectionalIteratorTag)
	{
		Reverse(first, middle);
		Reverse(middle, last);
		Reverse(first, last);
	}
	template<typename RandomAccessIterator, typename Distance>
	inline void __Rotate(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Distance, RandomAccessIteratorTag)
	{
		// ȡȫ����ǰ�γ��ȵ��������
		Distance n = __Gcd(last - first, middle - first);

		while(n--)
		{
			__RotateCycle(first, last, first + n, middle - first, IteratorTraits<RandomAccessIterator>::value_type());
		}
	}
	template<typename T>
	inline T __Gcd(T m, T n)
	{
		while( n != 0 )
		{
			T tmp = m % n;
			m = n;
			n = tmp;
		}

		return m;
	}
	template<typename RandomAccessIterator, typename Distance, typename T>
	inline void __RotateCycle(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator initial, Distance shift, T)
	{
		T value = *first;
		RandomAccessIterator ptr1 = initial;
		RandomAccessIterator ptr2 = ptr1 + shift;

		while( ptr2 != initial )
		{
			*ptr1	= *ptr2;
			ptr1	= ptr2;

			if( last - ptr2 > shift )
				ptr2 += shift;
			else
				ptr2 = first + (shift - (last - ptr2));
		}

		*ptr1 = value;
	}

	// RotateCopy
	template<typename ForwardIterator, typename OutIter>
	inline OutIter RotateCopy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutIter result)
	{
		return Copy(first, middle, Copy(middle, last, result));
	}


	// Search--������[first1, last1)�в�������[first2, last2)���״γ��ֵ㡣
	// �������1��������2�У��򷵻�last1
	template<typename ForwardIterator1, typename ForwardIterator2>
	inline ForwardIterator1 Search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
	{
		return __Search(first1, last1, first2, last2, 
			IteratorTraits<ForwardIterator1>::difference_type(), 
			IteratorTraits<ForwardIterator2>::difference_type());
	}
	template<typename ForwardIterator1, typename ForwardIterator2, typename Distance1, typename Distance2>
	inline ForwardIterator1 __Search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, Distance1, Distance2)
	{
		Distance1 d1 = DistanceFunc(first1, last1);

		Distance2 d2 = DistanceFunc(first2, last2);

		if( d1 < d2 )
			return last1;

		ForwardIterator1 cur1 = first1;
		ForwardIterator2 cur2 = first2;

		// �����ڶ�������
		while( cur2 != last2 )
		{
			// ���Ԫ����ȣ�����
			if( *cur1 == *cur2 )
			{
				++cur1;
				++cur2;
			}
			else
			{
				// �������һ����
				if( d1 == d2 )
					return last1;
				else
				{
					cur1 = ++first1;
					cur2 = first2;
					--d1;
				}
			}
		}

		return first1;
	}

	// SearchN
	template<typename ForwardIterator, typename Integer, typename T>
	inline ForwardIterator SearchN(ForwardIterator first, ForwardIterator last, Integer count, const T &value)
	{
		if( count <= 0 )
			return last;
		else
		{
			// ����value��һ�γ��ֵ�
			first = Find(first, last, value);

			// ������������Ԫ��,value��Ӧ�ó���n��
			while(first != last)
			{
				// value��Ӧ�ò���n��
				Integer n = count - 1;
				ForwardIterator iter = first;
				++iter;

				while(iter != last && n != 0 && *iter == value)
				{
					++iter;
					--n;
				}

				// ��ʾ�Ѿ��ҵ���n�γ��ֵ�Ԫ��
				if( n == 0 )
					return first;
				else
					// �ҵ�value����һ�����ֵ�
					first = Find(iter, last, value);
			}

			return last;
		}
	}
	template<typename ForwardIterator, typename Integer, typename T, typename BinaryPredicate>
	inline ForwardIterator SearchN(ForwardIterator first, ForwardIterator last, Integer count, const T &value, BinaryPredicate pred)
	{
		if( count <= 0 )
			return last;
		else
		{
			// ����value��һ�γ��ֵ�
			while(first != last)
			{
				if( pred(*first, value) )
					break;

				++first;
			}

			// ������������Ԫ��,value��Ӧ�ó���n��
			while(first != last)
			{
				// value��Ӧ�ò���n��
				Integer n = count - 1;
				ForwardIterator iter = first;
				++iter;

				while(iter != last && n != 0 && pred(*iter, value))
				{
					++iter;
					--n;
				}

				// ��ʾ�Ѿ��ҵ���n�γ��ֵ�Ԫ��
				if( n == 0 )
					return first;
				else
				{
					// �ҵ�value����һ�����ֵ�
					while(iter != last)
					{
						if( pred(*iter, value) )
							break;

						++iter;
					}

					first = iter;
				}
			}

			return last;
		}
	}


	// SwapRanges--��[first1, last1)Ԫ�����first2��ʼ��������ͬ��Ԫ�ؽ���.
	// �����ٲ�ͬ�����н��н���
	template<typename ForwardIterator1, typename ForwardIterator2>
	inline ForwardIterator2 SwapRanges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
	{
		for(; first1 != last1; ++first1, ++first2)
			IterSwap(first1, first2);

		return first2;
	}



	// Transform--�Էº���������[first,last)�е�ÿ��Ԫ��,������һ��������
	template<typename InIter, typename OutIter, typename UnaryOperation>
	inline OutIter Transform(InIter first, InIter last, OutIter result, UnaryOperation op)
	{
		for(; first != last; ++first, ++result)
		{
			*result = op(*first);
		}

		return result;
	}
	template<typename InIter1, typename InIter2, typename OutIter, typename BinaryOperation>
	inline OutIter Transform(InIter1 first1, InIter1 last1, InIter2 first2, OutIter result, BinaryOperation op)
	{
		for(; first1 != last1; ++first1, ++first2, ++result)
			*result = op(*first1, *first2);

		return result;
	}

	// MaxElement--ָ����������ֵ����Ԫ��
	template<typename ForwardIterator>
	inline ForwardIterator MaxElement(ForwardIterator first, ForwardIterator last)
	{
		return MaxElement(first, last, Less<typename IteratorTraits<ForwardIterator>::value_type>());
	}
	template<typename ForwardIterator, typename Compare>
	inline ForwardIterator MaxElement(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		if( first == last )
			return first;

		ForwardIterator iter = first;
		while(++first != last)
		{
			if( comp(*iter, *first) )
				iter = first;
		}

		return iter;
	}

	// MinElement--ָ����������ֵ��С��Ԫ��
	template<typename ForwardIterator>
	inline ForwardIterator MinElement(ForwardIterator first, ForwardIterator last)
	{
		return MinElement(first, last, Greater<typename IteratorTraits<ForwardIterator>::value_type>());
	}
	template<typename ForwardIterator, typename Compare>
	inline ForwardIterator MinElement(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		if( first == last )
			return first;

		ForwardIterator iter = first;
		while(++first != last)
		{
			if( comp(*iter, *first) )
				iter = first;
		}

		return iter;
	}


	// Inlcudes--�������������ж�����S2�Ƿ񺭸���S1.
	// S1��S2�����������򼯺�
	template<typename InIter1, typename InIter2>
	bool Includes(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2)
	{
		return Includes(first1, last1, first2, last2, Less<typename IteratorTraits<InIter1>::value_type>());
	}
	template<typename InIter1, typename InIter2, typename Compare>
	bool Includes(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2, Compare comp)
	{
		while( first1 != last1 && first2 != last2 )
		{
			if( comp(*first2, *first1) )
				return false;
			else if( comp(*first1, *first2) )
				++first1;
			else
				+first1, ++first2;
		}

		return first2 == last2;
	}


	// Merge--��������������ļ���S1��S2�ϲ���������������һ�οռ�
	template<typename InIter1, typename InIter2, typename OutIter>
	inline OutIter Merge(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2, OutIter result)
	{
		return Merge(first1, last1, first2, last2,result, Less<typename IteratorTraits<InIter1>::value_type>());
	}
	template<typename InIter1, typename InIter2, typename OutIter, typename Compare>
	inline OutIter Merge(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2, OutIter result, Compare comp)
	{
		while(first1 != last1 && first2 != last2)
		{
			if( comp(*first1, *first2) )
			{
				*result = *first2;
				++first2;
			}
			else
			{
				*result = *first1;
				++first1;
			}

			++result;
		}

		// ���ʣ��Ԫ��copy��Ŀ�Ķˣ����������У�������һ��Ϊ��
		return Copy(first2, last2, Copy(first1, last1, result));
	}


	// Partition--������[first,last)�е�Ԫ�����ţ����б�һԪ��������pred�ж�Ϊtrue��Ԫ�أ�
	// ����������ǰ��,false��Ԫ�ط��ں��.
	// ����㷨�����ȶ��ģ�����Ҫ�ȶ��������ʹ��StablePartition
	template<typename BidirectionalIterator, typename Predicate>
	inline BidirectionalIterator Partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred)
	{
		for( ; ; )
		{
			for( ; ; )
			{
				if( first == last )			// ͷָ�����βָ��
					return first;
				else if( pred(*first) )		// ͷָ����ָԪ�ط�������
					++first;
				else
					break;
			}

			--last;							// βָ���˷

			for( ; ; )
			{
				if( first == last )			// ͷָ�����βָ��
					return first;
				else if( !pred(*last) )		// βָ����ָ��Ԫ�ز���������
					--last;
				else						
					break;
			}

			IterSwap(first,last);			// ͷβָ��˴˽���
			++first;
		}
	}



	// Unique--�Ƴ��ظ���Ԫ�أ�ÿ����[first,last)���������ظ���Ԫ��Ⱥ�����Ƴ���Ԫ��Ⱥ�е�
	// ��һ���Ժ������Ԫ�ء������Ҫ�Ƴ�����(����������)�ظ�Ԫ�أ������ֽ���������ʹ�ظ���Ԫ�ض�����
	template<typename ForwardIterator>
	inline ForwardIterator Unique(ForwardIterator first, ForwardIterator last)
	{
		// �����ҵ������ظ�Ԫ�ص����
		first = AdjacentFind(first, last);
		return UniqueCopy(first, last, first);
	}

	template<typename InIter, typename OutIter>
	inline OutIter UniqueCopy(InIter first, InIter last, OutIter result)
	{
		if( first == last )
			return result;

		return __UniqueCopy(first, last, result, IteratorTraits<OutIter>::iterator_category());
	}
	template<typename InIter, typename ForwardIterator>
	inline ForwardIterator __UniqueCopy(InIter first, InIter last, ForwardIterator result, ForwardIteratorTag)
	{
		*result = *first;

		while(++first != last)
		{
			if( *result != *first )
				*++result = *first;
		}

		return ++result;
	}
	template<typename InIter, typename OutIter>
	inline OutIter __UniqueCopy(InIter first, InIter last, OutIter result, OutputIteratorTag)
	{
		return __UniqueCopy(first, last, result, IteratorTraits<InIter>::value_type());
	}
	// ����OutputIterator������ForwardIterator���Զ�ȡ���������ʲ��ܽ���*result != *first�Ĳ���������Ҫ��汾
	template<typename InIter, typename OutIter, typename T>
	OutIter __UniqueCopy(InIter first, InIter last, OutIter result, T)
	{
		T value = *first;
		*result = value;

		while(++first != last)
		{
			if( value != *first )
			{
				value = *first;
				*++result = value;
			}
		}

		return ++result;
	}

}



#endif