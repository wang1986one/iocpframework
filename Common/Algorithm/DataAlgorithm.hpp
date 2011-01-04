#ifndef __DATA_ALGORITHM_HPP
#define __DATA_ALGORITHM_HPP



namespace DataStructure
{
	// AdjacentFind--找出第一组满足条件的相邻元素
	template<typename ForwardIterator>
	inline ForwardIterator AdjacentFind(ForwardIterator first, ForwardIterator last)
	{
		if( first == last )
			return last;

		ForwardIterator next = first;
		while( ++next != last )
		{
			// 如果找到相邻元素值相等，就结束
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
			// 如果找到相邻元素值相等，就结束
			if( op(*first, *next) )	
				return first;

			first = next;
		}

		return last;
	}


	// Count--运用Equality操作符，将[first,last)区间内的每个元素拿来和指定值比较，并返回与value相等元素的个数
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
	// CountIf--将制定操作Pred实施与[first, last)区间内的每一个元素
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

	// Find--根据Equality操作符，循环查找[first, last)内的所有元素，找出第一个匹配"等同条件"者
	template<typename InIter, typename T>
	inline InIter Find(InIter first, InIter last, const T &value)
	{
		while( first != last && *first != value )
			++first;

		return first;
	}
	// FindIf--根据指定的Pred运算条件，循环查找
	template<typename InIter, typename Predicate>
	inline InIter FindIf(InIter first, InIter last, Predicate pred)
	{
		while( first != last && !pred(*first) )
			++first;

		return first;
	}
	// FindEnd--在序列[first1, last1)中，查找序列[first2,last2)的最后一次出现点。
	// 如果序列1内不存在完全匹配序列2的子序列，返回last1
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
		// 查找目标为空
		if( first2 == last2 )
			return last1;
		else
		{
			ForwardIterator1 result = last1;
			while(1)
			{
				// 利用Search查找某个子序列的首次出现点
				ForwardIterator1 newRes = search(first1, last1, first2, last2);
				
				// 没找到
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
		// 利用反向查找
		typedef ReverseIterator<BidirectionalIterator1> reverse1;
		typedef ReverseIterator<BidirectionalIterator2> reverse2; 

		reverse1 rlast1(first1);
		reverse2 rlast2(first2);

		reverse1 rresult = Search(reverse1(last1), rlast1, reverse2(last2), rlast2);

		// 没找到
		if( rresult == rlast1 )
			return last1;
		else
		{
			// 转会正常迭代器
			BidirectionalIterator1 result = rresult.Base();
			// 调整回子序列的起头处
			Advance(result, -DistanceFunc(first2, last2));

			return result;
		}
	}

	// FindFirstOf--以[first2,last2)区间内的某些元素作为查找目标，寻找在[first1,last1)区间内第一次出现地点
	template<typename InIter, typename ForwardIterator>
	inline InIter FindFirstOf(InIter first1, InIter last1, ForwardIterator first2, ForwardIterator last2)
	{
		for(; first1 != last1; ++first1)
		{
			for(ForwardIterator iter = first2; iter != last2; ++iter)
			{
				// 如果序列1的元素与序列2的元素相等
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
				// 如果序列1的元素与序列2的元素相等
				if( comp(*first1, *iter) )
					return first1;
			}
		}

		return last1;
	}


	// ForEach--将仿函数施行于[first,last)区间内每一个元素上,该仿函数不可改变元素内容,因为first和last都是InputIterator。
	// 如果需要修改元素内容，可使用Transform
	template<typename InIter, typename Function>
	inline Function ForEach(InIter first, InIter last, Function func)
	{
		for(; first != last; ++first)
			func(*first);

		return func;
	}


	// Generate--将仿函数的运算结果填写在[first,last)区间的所有元素中
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


	// Remove--移除[first,last)中所有与value相等的元素。但并不从容器中删除元素(容器大小并未改变),
	// 而是将每一个不与value相等的元素一次赋值为first之后的空间
	template<typename ForwardIterator, typename T>
	inline ForwardIterator Remove(ForwardIterator first, ForwardIterator last, const T &value)
	{
		// 利用循环查找第一个相等的元素
		first = Find(first, last, value);
		ForwardIterator next = first;

		// 利用RemoveCopy允许新旧容器重叠
		return first == last ? first : RemoveCopy(++next, last, first, value);
	}

	// RemoveCopy--移除[first,last)区间内所有与value相等的元素,并将结果复制到一个以result标志起始位置的容器中
	// 新容器可以和原容器重叠
	template<typename InIter, typename OutIter, typename T>
	inline OutIter RemoveCopy(InIter first, InIter last, OutIter result, const T &value)
	{
		for(; first != last; ++first)
		{
			// 如果不相等则赋值给新容器
			if( *first != value )
			{
				*result = *first;
				++result;
			}
		}

		return result;
	}

	// Removeif--移除[first,last)区间内所有被仿函数pred运算为true的元素
	template<typename ForwardIterator, typename Predicate>
	inline ForwardIterator RemoveIf(ForwardIterator first, ForwardIterator last, Predicate pred)
	{
		// 利用循环查找第一个匹配者
		first = FindIf(first, last, pred);

		ForwardIterator next = first;

		return first == last ? first : RemoveCopyIf(++next, last, first, pred);
	}
	template<typename InIter, typename OutIter, typename Predicate>
	inline OutIter RemoveCopyIf(InIter first, InIter last, OutIter result,Predicate pred)
	{
		for(; first != last; ++first)
		{
			// 如果不相等则赋值给新容器
			if( !pred(*first) )
			{
				*result = *first;
				++result;
			}
		}

		return result;
	}


	// Replace--将[first,last)区间内所有的oldValue都以newValue代替
	template<typename ForwardIterator, typename T>
	inline void Replace(ForwardIterator first, ForwardIterator last, const T &oldValue, const T &newValue)
	{
		for(; first != last; ++first)
		{
			if( *first == oldValue )
				*first = newValue;
		}
	}

	// ReplaceCopy--类似Replace，不同处是新序列会被复制到result所指容器中
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


	// Reverse--将[first,last)的元素在原容器中颠倒重排
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

	// ReplaceCopy--类似Replace，并将新序列放于result容器中
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


	// Rotate--将[first,last)内的元素和[middle,last)内的元素互换.
	// middle所指的元素会成为容器的第一个元素
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
			// 前段和后段的元素一一交换
			IterSwap(first, iter);
			++first;
			++iter;

			// 判断前段[first,middle)与后段[middle, last)哪个更先结束
			if( first == middle )
			{
				// 如果前段与后段一同结束,就整个一起结束
				if( iter == last )
					return;

				// 否则调整，再做交换
				middle = iter;
			}
			else if( iter == last )
			{
				// 后段先结束
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
		// 取全长和前段长度的最大公因子
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


	// Search--在序列[first1, last1)中查找序列[first2, last2)的首次出现点。
	// 如果序列1不在序列2中，则返回last1
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

		// 遍历第二个序列
		while( cur2 != last2 )
		{
			// 如果元素相等，调整
			if( *cur1 == *cur2 )
			{
				++cur1;
				++cur2;
			}
			else
			{
				// 如果序列一样长
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
			// 查找value第一次出现点
			first = Find(first, last, value);

			// 继续查找余下元素,value还应该出现n次
			while(first != last)
			{
				// value还应该查找n次
				Integer n = count - 1;
				ForwardIterator iter = first;
				++iter;

				while(iter != last && n != 0 && *iter == value)
				{
					++iter;
					--n;
				}

				// 表示已经找到了n次出现的元素
				if( n == 0 )
					return first;
				else
					// 找到value的下一个出现点
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
			// 查找value第一次出现点
			while(first != last)
			{
				if( pred(*first, value) )
					break;

				++first;
			}

			// 继续查找余下元素,value还应该出现n次
			while(first != last)
			{
				// value还应该查找n次
				Integer n = count - 1;
				ForwardIterator iter = first;
				++iter;

				while(iter != last && n != 0 && pred(*iter, value))
				{
					++iter;
					--n;
				}

				// 表示已经找到了n次出现的元素
				if( n == 0 )
					return first;
				else
				{
					// 找到value的下一个出现点
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


	// SwapRanges--将[first1, last1)元素与从first2开始，个数相同的元素交换.
	// 可以再不同容器中进行交换
	template<typename ForwardIterator1, typename ForwardIterator2>
	inline ForwardIterator2 SwapRanges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
	{
		for(; first1 != last1; ++first1, ++first2)
			IterSwap(first1, first2);

		return first2;
	}



	// Transform--以仿函数作用于[first,last)中的每个元素,并产生一个新序列
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

	// MaxElement--指向序列中数值最大的元素
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

	// MinElement--指向序列中数值最小的元素
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


	// Inlcudes--在有序区间里判断序列S2是否涵盖于S1.
	// S1和S2都必须是有序集合
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


	// Merge--将两个经过排序的集合S1和S2合并起来放置于另外一段空间
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

		// 最后剩余元素copy到目的端，两个序列中，至少有一个为空
		return Copy(first2, last2, Copy(first1, last1, result));
	}


	// Partition--将区间[first,last)中的元素重排，所有被一元条件运算pred判定为true的元素，
	// 都放在区间前段,false的元素放在后段.
	// 这个算法不是稳定的，若需要稳定排序则可使用StablePartition
	template<typename BidirectionalIterator, typename Predicate>
	inline BidirectionalIterator Partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred)
	{
		for( ; ; )
		{
			for( ; ; )
			{
				if( first == last )			// 头指针等于尾指针
					return first;
				else if( pred(*first) )		// 头指针所指元素符合条件
					++first;
				else
					break;
			}

			--last;							// 尾指针回朔

			for( ; ; )
			{
				if( first == last )			// 头指针等于尾指针
					return first;
				else if( !pred(*last) )		// 尾指针所指的元素不符合条件
					--last;
				else						
					break;
			}

			IterSwap(first,last);			// 头尾指针彼此交换
			++first;
		}
	}



	// Unique--移除重复的元素，每当在[first,last)内遇到有重复的元素群，便移除该元素群中的
	// 第一个以后的所有元素。如果需要移除所有(包括不相邻)重复元素，必须现将序列排序，使重复的元素都相邻
	template<typename ForwardIterator>
	inline ForwardIterator Unique(ForwardIterator first, ForwardIterator last)
	{
		// 首先找到相邻重复元素的起点
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
	// 由于OutputIterator不具有ForwardIterator可以读取的能力，故不能进行*result != *first的操作，才需要这版本
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