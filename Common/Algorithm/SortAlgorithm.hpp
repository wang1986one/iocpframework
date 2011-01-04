#ifndef __SORT_ALGORITHM_HPP
#define __SORT_ALGORITHM_HPP



namespace DataStructure
{
	// LowerBound--�����������䣬���ֲ��ҵ�һ���汾����ͼ��������[first,last)��Ѱ��Ԫ��value
	// ���[first,last)������value��ȵ�Ԫ�أ��򷵻�һ����������ָ�����е�һ��Ԫ��
	// ���û����ȵ�Ԫ�أ��򷵻�һ����С��valueֵԪ�صĵ�����
	// �������[first,last)�ڵ��κ�Ԫ�أ��򷵻�last
	template<typename ForwardIterator, typename T>
	inline ForwardIterator LowerBound(ForwardIterator first, ForwardIterator last, const T &value)
	{
		return __LowerBound(first, last, value, Less<int>(),
			IteratorTraits<ForwardIterator>::difference_type(),
			IteratorTraits<ForwardIterator>::iterator_category());
	}
	template<typename ForwardIterator, typename T, typename Compare>
	inline ForwardIterator LowerBound(ForwardIterator first, ForwardIterator last, const T &value, Compare comp)
	{
		return __LowerBound(first, last, value, comp,
			IteratorTraits<ForwardIterator>::difference_type(),
			IteratorTraits<ForwardIterator>::iterator_category());
	}
	template<typename ForwardIterator, typename T, typename Compare, typename Distance>
	inline ForwardIterator __LowerBound(ForwardIterator first, ForwardIterator last, const T &value, Compare comp, Distance, ForwardIteratorTag)
	{
		// ȡ�������䳤��
		Distance len		= DistanceFunc(first, last);
		Distance half		= 0;
		ForwardIterator mid = 0;

		while(len > 0)
		{
			// ����2
			half = len >> 1;
			mid = first;

			// ��midָ���м�λ��
			Advance(mid, half);

			if( comp(*mid, value) )
			{
				first = mid;
				++first;
				len = len - half - 1;
			}
			else
				len = half;
		}

		return first;
	}
	template<typename RandomAccessIterator, typename T, typename Compare, typename Distance>
	inline RandomAccessIterator __LowerBound(RandomAccessIterator first, RandomAccessIterator last, const T &value, Compare comp, Distance, RandomAccessIteratorTag)
	{
		// ȡ�������䳤��
		Distance len		= last - first;
		Distance half		= 0;
		RandomAccessIterator mid = 0;

		while(len > 0)
		{
			half = len >> 1;
			mid = first + half;

			if( comp(*mid, value) )
			{
				first = mid + 1;
				len = len - half - 1;
			}
			else 
				len = half;
		}

		return first;
	}

	// UpperBound--���ֲ��ң���һ������������[first,last)��Ѱ��value
	template<typename ForwardIterator, typename T>
	inline ForwardIterator UpperBound(ForwardIterator first, ForwardIterator last, const T &value)
	{
		return __UpperBound(first, last, value, Greater<int>(),
			IteratorTraits<ForwardIterator>::difference_type(),
			IteratorTraits<ForwardIterator>::iterator_category());
	}
	template<typename ForwardIterator, typename T, typename Compare>
	inline ForwardIterator UpperBound(ForwardIterator first, ForwardIterator last, const T &value, Compare comp)
	{
		return __UpperBound(first, last, value, comp,
			IteratorTraits<ForwardIterator>::difference_type(),
			IteratorTraits<ForwardIterator>::iterator_category());
	}
	template<typename ForwardIterator, typename T, typename Compare, typename Distance>
	inline ForwardIterator __UpperBound(ForwardIterator first, ForwardIterator last, const T &value, Compare comp, Distance, ForwardIteratorTag)
	{
		// ȡ�������䳤��
		Distance len		= DistanceFunc(first, last);
		Distance half		= 0;
		ForwardIterator mid = 0;

		while(len > 0)
		{
			// ����2
			half = len >> 1;
			mid = first;

			// ��midָ���м�λ��
			Advance(mid, half);

			if( comp(*mid, value) )
			{
				len = half;
			}
			else
			{
				first = mid;
				++first;
				len = len - half - 1;
			}
				
		}

		return first;
	}
	template<typename RandomAccessIterator, typename T, typename Compare, typename Distance>
	inline RandomAccessIterator __UpperBound(RandomAccessIterator first, RandomAccessIterator last, const T &value, Compare comp, Distance, RandomAccessIteratorTag)
	{
		// ȡ�������䳤��
		Distance len		= last - first;
		Distance half		= 0;
		RandomAccessIterator mid = 0;

		while(len > 0)
		{
			half = len >> 1;
			mid = first + half;

			if( comp(*mid, value) )
			{
				len = half;
			}
			else 
			{
				first = mid + 1;
				len = len - half - 1;
			}		
		}

		return first;
	}


	// BinarySearch--���ֲ��ң����������[first,last)��Ѱ��Ԫ��value
	// �������������ͬԪ�أ��򷵻�true,���򷵻�false
	template<typename ForwardIterator, typename T>
	inline bool BinarySearch(ForwardIterator first, ForwardIterator last, const T &value)
	{
		return BinarySearch(first, last, value, Less<typename IteratorTraits<ForwardIterator>::value_type>());
	}
	template<typename ForwardIterator, typename T, typename Compare>
	inline bool BinarySearch(ForwardIterator first, ForwardIterator last, const T &value, Compare comp)
	{
		ForwardIterator iter = LowerBound(first, last, value, comp);
		
		return iter != last && !(comp(value, *iter));
	}


	// InterpolateSearch--��ֵ����,������ֵ
	template<typename RandomAccessIterator, typename T>
	inline RandomAccessIterator InterpolateSearch(RandomAccessIterator first, RandomAccessIterator last, const T &value)
	{
		return InterpolateSearch(first, last, value, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename T, typename Compare>
	inline RandomAccessIterator InterpolateSearch(RandomAccessIterator first, RandomAccessIterator last, const T &value, Compare comp)
	{
		//// ȡ�������䳤��
		//IteratorTraits<RandomAccessIterator>::difference_type len		= last - first;
		//RandomAccessIterator mid;
		//last = last - 1;

		//if( value > *(last - 1) )
		//	return last;

		//while( first != last )
		//{
		//	mid = first + (value - *first) * (last - first) / (*last - *first);

		//	if( *mid == value ) 
		//		return mid;

		//	if( *mid < value ) 
		//		first = mid + 1;
		//	else 
		//		last = mid - 1;
		//}

		return last;
	}


	// NextPermutation--ȡ��[first,last)��������һ���������
	template<typename BidirectionalIterator>
	inline bool NextPermutation(BidirectionalIterator first, BidirectionalIterator last)
	{
		return NextPermutation(first, last, Less<typename IteratorTraits<BidirectionalIterator>::value_type>());
	}
	template<typename BidirectionalIterator, typename Compare>
	inline bool NextPermutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp)
	{
		if( first == last )
			return false;

		BidirectionalIterator iter = first;
		++iter;

		if( iter == last )
			return false;

		// Iterָ��β��
		iter = last;
		--iter;

		for(;;)
		{
			BidirectionalIterator ii = iter;
			--iter;

			// ����һ������Ԫ��
			if( comp(*iter, *ii) )
			{
				// ��β����ǰ����,֪����*iter��
				BidirectionalIterator j = last;
				while( !(comp(*iter, *--j)) )
					;
				IterSwap(iter, j);
				Reverse(ii, last);

				return true;
			}

			if( iter == first )
			{
				Reverse(first, last);
				return false;
			}
		}

		return false;
	}


	// PrevPermutation
	template<typename BidirectionalIterator>
	inline bool PrevPermutation(BidirectionalIterator first, BidirectionalIterator last)
	{
		return PrevPermutation(first, last, Less<typename IteratorTraits<BidirectionalIterator>::value_type>());
	}
	template<typename BidirectionalIterator, typename Compare>
	inline bool PrevPermutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp)
	{
		if( first == last )
			return false;

		BidirectionalIterator iter = first;
		++iter;

		if( iter == last )
			return false;

		// Iterָ��β��
		iter = last;
		--iter;

		for(;;)
		{
			BidirectionalIterator ii = iter;
			--iter;

			// ����һ������Ԫ��
			if( comp(*ii, *iter) )
			{
				// ��β����ǰ����,֪����*iter��
				BidirectionalIterator j = last;
				while( !(comp(*--j, *iter)) )
					;
				IterSwap(iter, j);
				Reverse(ii, last);

				return true;
			}

			if( iter == first )
			{
				Reverse(first, last);
				return false;
			}
		}

		return false;
	}

	
	// RandomShuffle--��[first,last)����Ԫ�ش���������ţ���N!�ִ���
	template<typename RandomAccessIterator>
	inline void RandomShuffle(RandomAccessIterator first, RandomAccessIterator last)
	{
		IteratorTraits<RandomAccessIterator>::difference_type type;
		return __RandomShuffle(first, last, type);
	}
	template<typename RandomAccessIterator, typename RandomGenerator>
	inline void RandomShuffle(RandomAccessIterator first, RandomAccessIterator last, RandomGenerator &gen)
	{
		if( first == last )
			return;
		
		for(RandomAccessIterator iter = first + 1; iter != last; ++iter)
		{
			IterSwap(iter, first +gen(iter - first + 1));
		}
	}
	template<typename RandomAccessIterator, typename Distance>
	inline void __RandomShuffle(RandomAccessIterator first, RandomAccessIterator last, Distance &)
	{
		if( first == last )
			return;
		
		for(RandomAccessIterator iter = first + 1; iter != last; ++iter)
		{
			IterSwap(iter, first + Distance(rand() % (iter - first + 1)));
		}
	}


	// PartialSort--�ҳ�middle-first����СԪ�ء���������MakeHeap��֯��һ��MaxHeap��Ȼ��
	// ��[middle,last)�е�ÿ��Ԫ����max-heap�����ֵ�Ƚϡ����С�ڸ����ֵ���ͻ���λ�ò����±���max-heap״̬
	template<typename RandomAccessIterator>
	inline void PartialSort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last)
	{
		IteratorTraits<RandomAccessIterator>::value_type type;
		return __PartialSort(first, middle, last, Less<int>(), type);
	}
	template<typename RandomAccessIterator, typename Compare>
	inline void PartialSort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp)
	{
		IteratorTraits<RandomAccessIterator>::value_type type;
		return __PartialSort(first, middle, last, comp, type);
	}
	template<typename RandomAccessIterator, typename Compare, typename T>
	inline void __PartialSort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp, T &)
	{
		MakeHeap(first, middle);

		/*for(RandomAccessIterator iter = middle; iter != last; ++iter)
		{
			if( comp(*iter, *first) )
				pop_heap(first, )
		}*/

		HeapSort(first, middle);
	}


	// EqualRange--�Ƕ��ֲ��ҵ�һ���汾�����������[first,last)��Ѱ��value
	// ����һ�Ե�����i��j��i�ǿɲ���ĵ�һ��λ�ã�j�ǿɲ�������һ��λ��
	template<typename ForwardIterator, typename T>
	inline Pair<ForwardIterator, ForwardIterator> EqualRange(ForwardIterator first, ForwardIterator last, const T &value)
	{
		IteratorTraits<ForwardIterator>::value_type type;
		IteratorTraits<ForwardIterator>::iterator_category category;

		return __EqualRange(first, last, value, Less<int>(), type, category);
	}
	template<typename ForwardIterator, typename T, typename Compare>
	inline Pair<ForwardIterator, ForwardIterator> EqualRange(ForwardIterator first, ForwardIterator last, const T &value, Compare comp)
	{
		IteratorTraits<ForwardIterator>::value_type type;
		IteratorTraits<ForwardIterator>::iterator_category category;

		return __EqualRange(first, last, value, comp, type, category);
	}
	template<typename RandomAccessIterator, typename T, typename Compare, typename Distance>
	inline Pair<RandomAccessIterator, RandomAccessIterator> __EqualRange(
		RandomAccessIterator first, RandomAccessIterator last, const T &value, Compare comp, Distance &, RandomAccessIteratorTag)
	{
		Distance len = last - first;
		Distance half = 0;
		RandomAccessIterator middle = 0, left = 0, right = 0;

		while( len > 0 )
		{
			// �ҳ�����λ��
			half = len >> 1;
			middle = first + half;

			if( comp(*middle, value) )
			{
				first = middle + 1;
				len = len - half + 1;
			}
			else if( comp(value, *middle) )
			{
				len = half;
			}
			else
			{
				// ��ǰ��β���LowerBound
				left = LowerBound(first, middle, value, comp);
				// �ں��β���LowerBound
				right = LowerBound(++middle, first + len, value, comp);

				return Pair<RandomAccessIterator, RandomAccessIterator>(left, right);
			}
		}

		// ���������ڶ�û��ƥ���ֵ��������ָ��һ������value��Ԫ��
		return Pair<RandomAccessIterator, RandomAccessIterator>(first, first);
	}
	template<typename ForwardIterator, typename T, typename Compare, typename Distance>
	inline Pair<ForwardIterator, ForwardIterator> __EqualRange(
		ForwardIterator first, ForwardIterator last, const T &value, Compare comp, Distance &, ForwardIteratorTag)
	{
		Distance len = last - first;
		Distance half = 0;
		ForwardIterator middle = 0, left = 0, right = 0;

		while( len > 0 )
		{
			// �ҳ�����λ��
			half = len >> 1;
			middle = first;
			Advance(middle, half);

			if( comp(*middle, value) )
			{
				first = middle;
				++first;
				len = len - half + 1;
			}
			else if( comp(value, *middle) )
			{
				len = half;
			}
			else
			{
				// ��ǰ��β���LowerBound
				left = LowerBound(first, middle, value, comp);
				// �ں��β���LowerBound
				right = LowerBound(++middle, first + len, value, comp);

				return Pair<RandomAccessIterator, RandomAccessIterator>(left, right);
			}
		}

		// ���������ڶ�û��ƥ���ֵ��������ָ��һ������value��Ԫ��
		return Pair<RandomAccessIterator, RandomAccessIterator>(first, first);
	}

	// NthElement--��������[first,last),ʹ������nth��ָ��Ԫ��������������ͬһλ�õ�Ԫ��ֵͬ
	// ���㷨�������ǣ����ϵ���median-of-3partition(�ס�β������)Ϊ����ָ���������зָ�Ϊ��С������������
	template<typename RandomAccessIterator>
	inline void NthElement(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last)
	{
		IteratorTraits<RandomAccessIterator>::value_type type;
		__NthElement(first, nth, last, type);
	}
	template<typename RandomAccessIterator, typename T>
	inline void __NthElement(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, T &)
	{
		//// ���ȳ���3
		//while( last - first > 3 )
		//{
		//	RandomAccessIterator iter = __Un
		//}
	}


	// PushHeap--Ϊ������Complete Binary Tree���������¼����Ԫ��һ��Ҫ��������һ����ΪҶ�ڵ�
	// ������������ҵĵ�һ���ո�
	template<typename RandomAccessIterator>
	inline void PushHeap(RandomAccessIterator first, RandomAccessIterator last)
	{
		PushHeap(first, last, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename Compare>
	inline void PushHeap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		typedef typename IteratorTraits<RandomAccessIterator>::difference_type Distance;
		typedef typename IteratorTraits<RandomAccessIterator>::value_type Type;

		Distance distance;
		Type type;

		// �˺����������ǣ���Ԫ�ض������ڵײ�������Ͷ�
		__PushHeap(first, last - first - 1, Distance(0), Type(*(last - 1)), comp);
	}
	//template<typename RandomAccessIterator, typename Distance, typename T>
	//inline void __PushHeapAux(RandomAccessIterator first, RandomAccessIterator last, Distance &, T &)
	//{
	//	// ��ֵ�������ڵײ�������β��
	//	__PushHeap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));
	//}
	template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
	inline void __PushHeap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, const Compare &comp)
	{
		// �ҵ����ڵ�
		Distance parent = (holeIndex - 1) >> 1;

		// û�дﵽ���ˣ��Ҹ��ڵ�С����ֵ
		while(holeIndex > topIndex && comp(*(first + parent), value))
		{
			// �ֵΪ��ֵ
			*(first + holeIndex) = *(first + parent);
			// �������ţ������������ڵ�
			holeIndex = parent;
			parent = (holeIndex - 1) >> 1;
		}

		// �ֵΪ��ֵ����ɲ������
		*(first + holeIndex) = value;		
	}

	// PopHeap--���ռ�ڵ����ϴ�ڵ�Ե����������·���֪��Ҷ�ڵ�Ϊֹ
	template<typename RandomAccessIterator>
	inline void PopHeap(RandomAccessIterator first, RandomAccessIterator last)
	{
		PopHeap(first, last, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename Compare>
	inline void PopHeap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		typedef typename IteratorTraits<RandomAccessIterator>::value_type Type;
		typedef typename IteratorTraits<RandomAccessIterator>::difference_type DistenceType;

		__PopHeap(first, last - 1, last - 1, Type(*(last - 1)), DistenceType(0), comp);
	}
	//template<typename RandomAccessIterator, typename T>
	//inline void __PopHeapAux(RandomAccessIterator first, RandomAccessIterator last, T &)
	//{
	//	typedef typename IteratorTraits<RandomAccessIterator>::difference_type Distance;
	//	Distance distance;

	//	// Pop �����Ľ����Ϊ�ײ������ĵ�һ��Ԫ�أ������ȵ���ֵΪβֵ��Ȼ����ֵ����β�ڵ�
	//	// Ȼ�����[first, last - 1)ʹ֮����λһ���ϸ��heap
	//	__PopHeap(first, last - 1, last - 1, T(*(last - 1)), distance);
	//}
	template<typename RandomAccessIterator, typename T, typename Distance, typename Compare>
	inline void __PopHeap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance, const Compare &comp)
	{
		// �趨βֵΪ��ֵ
		*result = *first;

		// ����heap
		__AdjustHeap(first, Distance(0), Distance(last - first), value, comp);
	}
	template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
	inline void __AdjustHeap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, const Compare &comp)
	{
		Distance topIndex = holeIndex;

		// ���ڵ�֮�ҽڵ�
		Distance secondIndex = 2 * holeIndex + 2;

		while(secondIndex < len)
		{
			// �Ƚ϶��ڵ�֮��������ֵ��Ȼ����secondChild������ӽڵ�
			if( comp(*(first + secondIndex), *(first + (secondIndex - 1))) )
				--secondIndex;

			// ��ϴ�ֵΪ��ֵ,������������ϴ��ӽڵ㴦
			*(first + holeIndex) = *(first + secondIndex);
			holeIndex = secondIndex;
			
			// �ҳ��¶��ڵ���ҽڵ�
			secondIndex = 2 * secondIndex + 2;
		}

		// û�����ӽڵ㣬ֻ�����ӽڵ�
		if( secondIndex == len )
		{
			*(first + holeIndex) = *(first + (secondIndex - 1));
			holeIndex = secondIndex - 1;
		}

		__PushHeap(first, holeIndex, topIndex, value, comp);
	}


	// SortHeap--����������heap��PopHeap������ÿ�ν�������Χ�Ӻ���ǰ������һ��Ԫ��
	// PopHeap��ɼ�ֵ����Ԫ�ط��ڵײ�������β��
	template<typename RandomAccessIterator>
	inline void HeapSort(RandomAccessIterator first, RandomAccessIterator last)
	{
		HeapSort(first, last, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename Compare>
	inline void HeapSort(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		while(last - first > 1)
			PopHeap(first, last--, comp);
	}

	// MakeHeap--��һ������ת��Ϊһ��heap
	template<typename RandomAccessIterator>
	inline void MakeHeap(RandomAccessIterator first, RandomAccessIterator last)
	{
		MakeHeap(first, last, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename Compare>
	inline void MakeHeap(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		typedef typename IteratorTraits<RandomAccessIterator>::value_type Type;
		typedef typename IteratorTraits<RandomAccessIterator>::difference_type Distance;

		Type type;
		Distance distance;
		__MakeHeap(first, last, type, distance, comp);
	}
	template<typename RandomAccessIterator, typename T, typename Distance, typename Compare>
	void __MakeHeap(RandomAccessIterator first, RandomAccessIterator last, T &, Distance &, const Compare &comp)
	{
		// �������Ϊ0��1����������
		if( last - first < 2 )
			return;

		Distance len = last - first;

		// �ҳ���һ����Ҫ���ŵ�����ͷ������parent��ʶ��
		Distance parent = (len - 2) >> 1;

		for( ; ; )
		{
			// ������parentΪ�׵�������len��Ϊ����__AdjustHeap�жϲ�����Χ
			__AdjustHeap(first, parent, len, T(*(first + parent)), comp);
			if( parent == 0 )
				return;

			// ͷ����ǰһ���ڵ�
			--parent;
		}
	}

	
	



	// ð������ : �Ƚ��������ڼ�¼�������򽻻�
	template<typename T, size_t nSize>
	inline void BublleSort(T (&arr)[nSize])
	{
		return BublleSort(arr, arr + nSize, Less<T>());
	}
	template<typename T, size_t nSize, typename Operation>
	inline void BublleSort(T (&arr)[nSize], const Operation &op)
	{
		return BublleSort(arr, arr + nSize, op);
	}
	template<typename RandomAccessIterator>
	inline void BublleSort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return BublleSort(first, last, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIteratorT, typename Operation>
	inline void BublleSort(RandomAccessIteratorT first, RandomAccessIteratorT last, const Operation &op)
	{
		if( first == last )
			return;

		for(RandomAccessIteratorT i = first; i != last; ++i)
		{
			for(RandomAccessIteratorT j = first + (last - first - 1); j > i; --j)
			{
				if( op(*j, *(j - 1)) )
					Swap(*j, *(j - 1));
			}
		}
	}


	// ��β������(˫��ð��)
	template<typename T, size_t nSize>
	inline void CocktailSort(T (&arr)[nSize])
	{
		return CocktailSort(arr, Less<T>());
	}
	template<typename T, size_t nSize, typename Operation>
	inline void CocktailSort(T (&arr)[nSize], const Operation &op)
	{
		return CocktailSort(arr, arr + _countof(arr), op);
	}
	template<typename RandomAccessIterator>
	inline void CocktailSort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return CocktailSort(first, last, Less<T>());
	}
	template<typename BidirectionalIterator, typename Operation>
	inline void CocktailSort(BidirectionalIterator first, BidirectionalIterator last, const Operation &op)
	{
		if( first == last )
			return;

		size_t bottom = 0;
		size_t top = DistanceFunc(first, last) - 1;
		bool bSwap = true;

		while( bSwap == true )
		{
			bSwap = false;

			// ����ı����ֵλ��
			for(BidirectionalIterator iter = first + bottom; iter < first + top; ++iter)
			{
				if( op(*(iter + 1), *iter) )
				{
					Swap(*iter, *(iter + 1));
					bSwap = true;
				}
			}

			// �Լ�����λ�ã���Ϊ���ֵ��δ���򲿷ֵ����
			--top;

			// �����ı���Сֵλ��
			for(BidirectionalIterator j = first + top; j > first + bottom; --j)
			{
				if( op(*j, *(j - 1)) )
				{
					Swap(*j, *(j - 1));
					bSwap = true;
				}
			}

			// �����ײ�λ�ã���Ϊ��Сֵ��δ���򲿷ֵ���ײ�
			++bottom; 
		}
	}


	// QuickSort : ��ȥ���������е�ĳ����¼��Ϊ��׼�����չؼ����С���������зֳ�����������
	// --�������е����м�¼�Ĺؼ��붼�Ȼ�׼С(���)
	// --�������еĶ��Ȼ�׼��
	template<typename T, size_t nSize>
	inline void QuickSort(T (&arr)[nSize])
	{
		return QuickSort(arr, Less<T>());
	}
	template<typename T, size_t nSize, typename Operation>
	inline void QuickSort(T (&arr)[nSize], const Operation &op)
	{
		return QuickSort(arr, arr + _countof(arr) - 1, op);
	}
	template<typename BidirectionalIterator>
	inline void QuickSort(BidirectionalIterator first, BidirectionalIterator last)
	{
		return QuickSort(first, last - 1, Less<typename IteratorTraits<BidirectionalIterator>::value_type>());
	}
	template<typename BidirectionalIterator>
	typename IteratorTraits<BidirectionalIterator>::value_type __Median(BidirectionalIterator first, BidirectionalIterator last)
	{
		IteratorTraits<BidirectionalIterator>::difference_type center = (last - first) / 2;

		if( *(first + center) < *first )
			IterSwap(first, first + center);
		if( *(last - 1) < *first )
			IterSwap(first, last - 1);
		if( *(last - 1) < *(first + center) )
			IterSwap(first + center, last - 1);

		IterSwap(first + center, last - 2);

		return *(last - 2);
	}
	template<typename BidirectionalIterator, typename Compare>
	inline void QuickSort(BidirectionalIterator first, BidirectionalIterator last,const  Compare &comp)
	{
		if( first == last )
			return;


		BidirectionalIterator left	= first;
		BidirectionalIterator right	= last;
		//typename IteratorTraits<BidirectionalIterator>::value_type pivot = __Median(first, last);
		BidirectionalIterator pivot = left++;

		while( left != right )
		{
			if( comp( *left, *pivot ) ) 
			{
				++left;
			} 
			else 
			{
				while( (left != right) && comp( *pivot, *right ) )
					right--;

				IterSwap( left, right );
			}

		}

		if( comp(*pivot, *left) )
			--left;

		IterSwap(first, left);

		
		QuickSort(first, left, comp);
		QuickSort(right, last, comp);

		return;
	}

	// SelectionSort : ÿ��ѡ����iС�ļ�¼�����ڵ�iλ��(i�����Ϊ0)
	template<typename T, size_t nSize>
	inline void SelectionSort(T (&arr)[nSize])
	{
		return SelectionSort(arr, Less<T>());
	}
	template<typename T, size_t nSize, typename Operation>
	inline void SelectionSort(T (&arr)[nSize], const Operation &op)
	{
		return SelectionSort(arr, arr + _countof(arr), op);
	}
	template<typename RandomAccessIterator>
	inline void SelectionSort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return SelectionSort(first, last, Less<T>());
	}
	template<typename RandomAccessIterator, typename Operation>
	inline void SelectionSort(RandomAccessIterator first, RandomAccessIterator last, const Operation &op)
	{
		if( first == last )
			return;

		for(RandomAccessIterator iter = first; iter != last; ++iter)
		{
			// ��Сֵ
			RandomAccessIterator min = iter;
			for(RandomAccessIterator jiter = iter; jiter != last; ++jiter)
			{
				if( op(*jiter, *min) )
					min = jiter;
			}

			// ������Сֵ
			if( min != iter )
				IterSwap(min, iter);
		}
	}


	// InsertSort : ÿ����һ��������ļ�¼����ؼ���Ĵ�С��������֮ǰ�Ѿ�����õļ�¼���ʵ�λ�ã���ͷ��β����
	template<typename T, size_t nSize>
	inline void InsertSort(T (&arr)[nSize])
	{
		return InsertSort(arr, Less<T>());
	}
	template<typename T, size_t nSize, typename Operation>
	inline void InsertSort(T (&arr)[nSize], const Operation &op)
	{
		return InsertSort(arr, arr + _countof(arr), op);
	}
	template<typename RandomAccessIterator>
	inline void InsertSort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return InsertSort(first, last, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename BidirectionalIterator, typename Compare>
	inline void InsertSort(BidirectionalIterator first, BidirectionalIterator last, const Compare &comp)
	{
		if( first == last )
			return;

		for(BidirectionalIterator iter = first + 1; iter != last; ++iter)
		{
			// ��ʱ���浱ǰֵ
			IteratorTraits<BidirectionalIterator>::value_type tmp = *iter;

			BidirectionalIterator jter = iter;
			// �Ƚϵ�ǰֵ��ǰ��ֵ
			for(; jter != first && comp(tmp, *(jter - 1)) ; --jter)
			{
				*jter = *(jter - 1);
			}
			
			*jter = tmp;
		}
	}


	// BinaryInsertSort : �����������е�����������˳��������Ϊ�۰�����
	template<typename T, size_t nSize>
	inline void BinaryInsertSort(T (&arr)[nSize])
	{
		return BinaryInsertSort(arr, Less<T>());
	}
	template<typename T, size_t nSize, typename Operation>
	inline void BinaryInsertSort(T (&arr)[nSize], const Operation &op)
	{
		return BinaryInsertSort(arr, arr + _countof(arr), op);
	}
	template<typename RandomAccessIterator>
	inline void BinaryInsertSort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return BinaryInsertSort(first, last, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename Compare>
	inline void BinaryInsertSort(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		if( first == last )
			return;

		IteratorTraits<RandomAccessIterator>::value_type tmp = *first;

		for(RandomAccessIterator iter = first + 1; iter != last; ++iter)
		{
			// ��ʱ���浱ǰֵ
			tmp = *iter;

			// �۰����
			RandomAccessIterator low = first, high = iter - 1;
			while( low <= high )
			{
				RandomAccessIterator mid = first + ((low - first) + (high - first)) / 2;
				if( comp(tmp, *mid) )
					high = mid - 1;
				else
					low = mid + 1;
			}

			// ��¼����
			for(RandomAccessIterator jter = iter - 1; jter >= low; --jter)
				*(jter + 1) = *jter;
			
			*low = tmp;
		}
	}


	// ShellSort : ȡһ�����(gap)�������зֳ����ɵ������С���ÿ�������н��в�������
	// Ȼ������С����ظ�ֱ�����Ϊ1
	template<typename T, size_t nSize>
	inline void ShellSort(T (&arr)[nSize])
	{
		return ShellSort(arr, Less<T>());
	}
	template<typename T, size_t nSize, typename Operation>
	inline void ShellSort(T (&arr)[nSize], const Operation &op)
	{
		return ShellSort(arr, arr + _countof(arr), op);
	}
	template<typename RandomAccessIterator>
	inline void ShellSort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return ShellSort(first, last, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename Compare>
	inline void ShellSort(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		if( first == last )
			return;
		
		typedef typename IteratorTraits<RandomAccessIterator>::difference_type	difference_type;
		typedef typename IteratorTraits<RandomAccessIterator>::value_type		value_type;
		
		difference_type gap = last - first;
		gap >>= 1;	// gap / 2

		for(; gap > 0; gap >>= 1)
		{
			for(RandomAccessIterator i = first + gap; i < last; ++i)
			{
				value_type tmp = *i;
				RandomAccessIterator j = i;

				for(; (j - first) >= gap && comp(tmp, *(j - gap)); j -= gap)
				{
					*j = *(j - gap);
				}

				*j = tmp;
			}

		}


		/*if( first == last )
			return false;

		typedef typename IteratorTraits<BidirectionalIterator>::difference_type	difference_type;
		typedef typename IteratorTraits<BidirectionalIterator>::value_type		value_type;

		difference_type gap = (last - first);
		gap >>= 2;

		for(; gap > 0; gap >>= 2)
		{
			for(BidirectionalIterator i = first + gap; i < last; ++i)
			{
				value_type tmp = *i;
				BidirectionalIterator j = i;

				for(; j - first >= gap && comp(tmp, *(j - gap)); j -= gap)
				{
					*j = *(j - gap);
				}

				*j = tmp;
			}

		}*/
	}

	// MergeSort
	template<typename T, size_t nSize>
	inline void MergeSort(T (&arr)[nSize])
	{
		return MergeSort(arr, Less<T>());
	}
	template<typename T, size_t nSize, typename Operation>
	inline void MergeSort(T (&arr)[nSize], const Operation &op)
	{
		return MergeSort(arr, arr + _countof(arr), op);
	}
	template<typename RandomAccessIterator>
	inline void MergeSort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return MergeSort(first, last, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename Compare>
	inline void MergeSort(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		if( last - first > 1 )
		{
			RandomAccessIterator middle = first + ((last - first) >> 1);

			MergeSort(first, middle, comp);
			MergeSort(middle, last, comp);

			std::inplace_merge(first, middle, last, comp);
		}
		
	}
	template<typename RandomAccessIterator, typename Compare>
	inline void __Merge(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, const Compare comp)
	{
		typedef typename IteratorTraits<RandomAccessIterator>::value_type Type;
		

		
	}



	// RadixSort
	template<typename T, size_t nSize>
	inline void RadixSort(T (&arr)[nSize])
	{
		return RadixSort(arr, Less<T>());
	}
	template<typename T, size_t nSize, typename Operation>
	inline void RadixSort(T (&arr)[nSize], const Operation &op)
	{
		return RadixSort(arr, arr + _countof(arr), op);
	}
	template<typename RandomAccessIterator>
	inline void RadixSort(RandomAccessIterator first, RandomAccessIterator last)
	{
		return RadixSort(first, last, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename Compare>
	inline void RadixSort(RandomAccessIterator first, RandomAccessIterator last, const Compare &comp)
	{
		/*static const unsigned int r = 8;
		static const unsigned int R = 1 << r;
		static const unsigned int P = (sizeof(unsigned int) + r - 1U) / r;*/

		enum {r = 8, R = 1 << r, P = (sizeof(unsigned int) + r - 1U) / r};
		
		unsigned int nCount = last - first;
		Array<unsigned int, R> arr;
		Vector<unsigned int> tmp(nCount);

		for(unsigned int i = 0; i != P; ++i)
		{
			for(unsigned int j = 0; j != R; ++j)
				arr[j] = 0;

			for(unsigned int k = 0; k != nCount; ++k)
			{
				++arr[(*(first + k) >> (r * i)) % (R - 1U)];
				tmp[k] = *(first + k);
			}
	
			unsigned int pos = 0;
			for(unsigned int j = 0; j != R; ++j)
			{
				const unsigned int val = arr[j];
				arr[j] = pos;
				pos += val;
			}

			for(unsigned int k = 0; k != nCount; ++k)
			{
				unsigned int j = ((tmp[k] >> (r * i)) % (R - 1U));

				*(first + arr[j]++) = tmp[k];
			}

		}
	}
	
}





#endif