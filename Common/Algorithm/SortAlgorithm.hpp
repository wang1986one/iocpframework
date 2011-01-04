#ifndef __SORT_ALGORITHM_HPP
#define __SORT_ALGORITHM_HPP



namespace DataStructure
{
	// LowerBound--用于有序区间，二分查找的一个版本，试图在已排序[first,last)中寻找元素value
	// 如果[first,last)具有与value相等的元素，则返回一个迭代器，指向其中第一个元素
	// 如果没有相等的元素，则返回一个不小于value值元素的迭代器
	// 如果大于[first,last)内的任何元素，则返回last
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
		// 取整个区间长度
		Distance len		= DistanceFunc(first, last);
		Distance half		= 0;
		ForwardIterator mid = 0;

		while(len > 0)
		{
			// 除以2
			half = len >> 1;
			mid = first;

			// 令mid指向中间位置
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
		// 取整个区间长度
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

	// UpperBound--二分查找，在一个已排序区间[first,last)中寻找value
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
		// 取整个区间长度
		Distance len		= DistanceFunc(first, last);
		Distance half		= 0;
		ForwardIterator mid = 0;

		while(len > 0)
		{
			// 除以2
			half = len >> 1;
			mid = first;

			// 令mid指向中间位置
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
		// 取整个区间长度
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


	// BinarySearch--二分查找，在已排序的[first,last)中寻找元素value
	// 如果区间内有相同元素，则返回true,否则返回false
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


	// InterpolateSearch--插值查找,估量插值
	template<typename RandomAccessIterator, typename T>
	inline RandomAccessIterator InterpolateSearch(RandomAccessIterator first, RandomAccessIterator last, const T &value)
	{
		return InterpolateSearch(first, last, value, Less<typename IteratorTraits<RandomAccessIterator>::value_type>());
	}
	template<typename RandomAccessIterator, typename T, typename Compare>
	inline RandomAccessIterator InterpolateSearch(RandomAccessIterator first, RandomAccessIterator last, const T &value, Compare comp)
	{
		//// 取整个区间长度
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


	// NextPermutation--取得[first,last)区间内下一个排列组合
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

		// Iter指向尾端
		iter = last;
		--iter;

		for(;;)
		{
			BidirectionalIterator ii = iter;
			--iter;

			// 所定一组相邻元素
			if( comp(*iter, *ii) )
			{
				// 由尾端往前查找,知道比*iter大
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

		// Iter指向尾端
		iter = last;
		--iter;

		for(;;)
		{
			BidirectionalIterator ii = iter;
			--iter;

			// 所定一组相邻元素
			if( comp(*ii, *iter) )
			{
				// 由尾端往前查找,知道比*iter大
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

	
	// RandomShuffle--将[first,last)序列元素次序随机重排，有N!种次序
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


	// PartialSort--找出middle-first个最小元素。首先利用MakeHeap组织成一个MaxHeap。然后
	// 将[middle,last)中的每个元素与max-heap的最大值比较。如果小于该最大值，就互换位置并重新保持max-heap状态
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


	// EqualRange--是二分查找的一个版本，在已排序的[first,last)中寻找value
	// 返回一对迭代器i和j，i是可插入的第一个位置，j是可插入的最后一个位置
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
			// 找出中央位置
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
				// 在前半段查找LowerBound
				left = LowerBound(first, middle, value, comp);
				// 在后半段查找LowerBound
				right = LowerBound(++middle, first + len, value, comp);

				return Pair<RandomAccessIterator, RandomAccessIterator>(left, right);
			}
		}

		// 整个区间内都没有匹配的值，迭代器指向一个大于value的元素
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
			// 找出中央位置
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
				// 在前半段查找LowerBound
				left = LowerBound(first, middle, value, comp);
				// 在后半段查找LowerBound
				right = LowerBound(++middle, first + len, value, comp);

				return Pair<RandomAccessIterator, RandomAccessIterator>(left, right);
			}
		}

		// 整个区间内都没有匹配的值，迭代器指向一个大于value的元素
		return Pair<RandomAccessIterator, RandomAccessIterator>(first, first);
	}

	// NthElement--重新排列[first,last),使迭代器nth所指的元素与整个排序后的同一位置的元素同值
	// 该算法的做法是，不断地以median-of-3partition(首、尾、中央)为枢轴分割，将整个序列分割为更小的左右子序列
	template<typename RandomAccessIterator>
	inline void NthElement(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last)
	{
		IteratorTraits<RandomAccessIterator>::value_type type;
		__NthElement(first, nth, last, type);
	}
	template<typename RandomAccessIterator, typename T>
	inline void __NthElement(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, T &)
	{
		//// 长度超过3
		//while( last - first > 3 )
		//{
		//	RandomAccessIterator iter = __Un
		//}
	}


	// PushHeap--为了满足Complete Binary Tree的条件，新加入的元素一定要放在最下一层作为叶节点
	// 并填补在由左至右的第一个空格
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

		// 此函数被调用是，新元素都以至于底部容器最低端
		__PushHeap(first, last - first - 1, Distance(0), Type(*(last - 1)), comp);
	}
	//template<typename RandomAccessIterator, typename Distance, typename T>
	//inline void __PushHeapAux(RandomAccessIterator first, RandomAccessIterator last, Distance &, T &)
	//{
	//	// 新值必须置于底部容器最尾端
	//	__PushHeap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));
	//}
	template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
	inline void __PushHeap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, const Compare &comp)
	{
		// 找到父节点
		Distance parent = (holeIndex - 1) >> 1;

		// 没有达到顶端，且父节点小于新值
		while(holeIndex > topIndex && comp(*(first + parent), value))
		{
			// 令洞值为父值
			*(first + holeIndex) = *(first + parent);
			// 调整洞号，向上提升父节点
			holeIndex = parent;
			parent = (holeIndex - 1) >> 1;
		}

		// 令洞值为新值，完成插入操作
		*(first + holeIndex) = value;		
	}

	// PopHeap--将空间节点和其较大节点对调，并持续下方，知道叶节点为止
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

	//	// Pop 操作的结果因为底部容器的第一个元素，因首先调整值为尾值，然后将首值调制尾节点
	//	// 然后调整[first, last - 1)使之车各位一个合格的heap
	//	__PopHeap(first, last - 1, last - 1, T(*(last - 1)), distance);
	//}
	template<typename RandomAccessIterator, typename T, typename Distance, typename Compare>
	inline void __PopHeap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance, const Compare &comp)
	{
		// 设定尾值为首值
		*result = *first;

		// 调整heap
		__AdjustHeap(first, Distance(0), Distance(last - first), value, comp);
	}
	template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
	inline void __AdjustHeap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, const Compare &comp)
	{
		Distance topIndex = holeIndex;

		// 洞节点之右节点
		Distance secondIndex = 2 * holeIndex + 2;

		while(secondIndex < len)
		{
			// 比较洞节点之左右两个值，然后以secondChild代表大子节点
			if( comp(*(first + secondIndex), *(first + (secondIndex - 1))) )
				--secondIndex;

			// 令较大值为洞值,再令洞号下移至较大子节点处
			*(first + holeIndex) = *(first + secondIndex);
			holeIndex = secondIndex;
			
			// 找出新洞节点的右节点
			secondIndex = 2 * secondIndex + 2;
		}

		// 没有右子节点，只有左子节点
		if( secondIndex == len )
		{
			*(first + holeIndex) = *(first + (secondIndex - 1));
			holeIndex = secondIndex - 1;
		}

		__PushHeap(first, holeIndex, topIndex, value, comp);
	}


	// SortHeap--持续对整个heap做PopHeap操作，每次将操作范围从后向前所建议一个元素
	// PopHeap会吧键值最大的元素放在底部容器最尾端
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

	// MakeHeap--将一段数据转化为一个heap
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
		// 如果长度为0或1则不重新排列
		if( last - first < 2 )
			return;

		Distance len = last - first;

		// 找出第一个需要重排的子树头部，以parent标识出
		Distance parent = (len - 2) >> 1;

		for( ; ; )
		{
			// 重新以parent为首的子树，len是为了让__AdjustHeap判断操作范围
			__AdjustHeap(first, parent, len, T(*(first + parent)), comp);
			if( parent == 0 )
				return;

			// 头部向前一个节点
			--parent;
		}
	}

	
	



	// 冒泡排序 : 比较两个相邻记录，逆序则交换
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


	// 鸡尾酒排序(双向冒泡)
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

			// 正序改变最大值位置
			for(BidirectionalIterator iter = first + bottom; iter < first + top; ++iter)
			{
				if( op(*(iter + 1), *iter) )
				{
					Swap(*iter, *(iter + 1));
					bSwap = true;
				}
			}

			// 自减顶部位置，因为最大值在未排序部分的最顶部
			--top;

			// 后续改变最小值位置
			for(BidirectionalIterator j = first + top; j > first + bottom; --j)
			{
				if( op(*j, *(j - 1)) )
				{
					Swap(*j, *(j - 1));
					bSwap = true;
				}
			}

			// 自增底部位置，因为最小值在未排序部分的最底部
			++bottom; 
		}
	}


	// QuickSort : 人去待排序列中的某个记录作为基准，按照关键吗大小将整个序列分成两个子序列
	// --左子序列的所有记录的关键码都比基准小(或等)
	// --右子序列的都比基准大
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

	// SelectionSort : 每次选出第i小的记录，放在第i位置(i的起点为0)
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
			// 最小值
			RandomAccessIterator min = iter;
			for(RandomAccessIterator jiter = iter; jiter != last; ++jiter)
			{
				if( op(*jiter, *min) )
					min = jiter;
			}

			// 交换最小值
			if( min != iter )
				IterSwap(min, iter);
		}
	}


	// InsertSort : 每步讲一个待排序的记录按其关键码的大小，插入列之前已经排序好的记录的适当位置，从头到尾即可
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
			// 临时保存当前值
			IteratorTraits<BidirectionalIterator>::value_type tmp = *iter;

			BidirectionalIterator jter = iter;
			// 比较当前值与前端值
			for(; jter != first && comp(tmp, *(jter - 1)) ; --jter)
			{
				*jter = *(jter - 1);
			}
			
			*jter = tmp;
		}
	}


	// BinaryInsertSort : 将插入排序中的搜索策略由顺序搜索变为折半搜索
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
			// 临时保存当前值
			tmp = *iter;

			// 折半查找
			RandomAccessIterator low = first, high = iter - 1;
			while( low <= high )
			{
				RandomAccessIterator mid = first + ((low - first) + (high - first)) / 2;
				if( comp(tmp, *mid) )
					high = mid - 1;
				else
					low = mid + 1;
			}

			// 记录后移
			for(RandomAccessIterator jter = iter - 1; jter >= low; --jter)
				*(jter + 1) = *jter;
			
			*low = tmp;
		}
	}


	// ShellSort : 取一个间隔(gap)，将序列分成若干的子序列。对每个子序列进行插入排序。
	// 然后逐渐缩小间隔重复直到间隔为1
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