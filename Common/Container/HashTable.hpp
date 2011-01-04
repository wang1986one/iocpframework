#ifndef __HASH_TABLE_HPP
#define __HASH_TABLE_HPP


#include "HashFunction.hpp"


namespace DataStructure
{

	template<typename Value>
	struct __HashTableNode
	{
		// 用vector来做HashTable的底层
		__HashTableNode *m_pNext;
		Value m_value;
	};


	template<typename Value, typename Key, typename HashFunc,
		typename ExtraKey, typename EqualKey, typename Alloc = Allocator<__HashTableNode<Value>> >
	class HashTable;


	template<typename Value, typename Key, typename HashFunc,
		typename ExtraKey, typename EqualKey, typename Alloc = Allocator<Value> >
	struct __HashTableIterator;

	template<typename Value, typename Key, typename HashFunc,
		typename ExtraKey, typename EqualKey, typename Alloc = Allocator<Value> >
	struct __HashTableConstIterator;


	template<typename Value, typename Key, typename HashFunc,
		typename ExtraKey, typename EqualKey, typename Alloc >
	struct __HashTableIterator
	{
		typedef HashTable<Value, Key, HashFunc, ExtraKey, EqualKey, Alloc>					HashTable;
		typedef __HashTableIterator<Value, Key, HashFunc, ExtraKey, EqualKey, Alloc>		iterator;
		typedef __HashTableConstIterator<Value, Key, HashFunc, ExtraKey, EqualKey, Alloc>	const_iterator;
		typedef __HashTableNode<Value> node;


		typedef ForwardIteratorTag			iterator_category;
		typedef ptrdiff_t					difference_type;
		typedef size_t						size_type;
		typedef Value						value_type;
		typedef Value&						reference;
		typedef Value*						pointer;


		// 迭代器目前所指节点
		node *m_pCur;

		// 保持容器的连接关系
		HashTable *m_pHashTable;

		__HashTableIterator(node *pNode, HashTable *table)
			: m_pCur(pNode)
			, m_pHashTable(table)
		{}
		__HashTableIterator()
		{}
		
		reference operator*() const
		{
			return m_pCur->m_value;
		}
		pointer operator->() const
		{
			return &(operator*());
		}

		iterator &operator++()
		{
			const node *pOld = m_pCur;
			m_pCur = m_pCur->m_pNext;	// 如果存在，就是它

			if( !m_pCur )
			{
				// 根据原值，重新定位。
				size_type bucket = m_pHashTable->_BucketNum(pOld->m_value);

				while( !m_pCur && ++bucket < m_pHashTable->m_buckets.size() )	
					m_pCur = m_pHashTable->m_buckets[bucket];
			}

			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++*this;	
			return tmp;
		}

		bool operator==(const iterator &iter) const
		{
			return m_pCur == iter.m_pCur;
		}
		bool operator!=(const iterator &iter) const
		{
			return m_pCur != iter.m_pCur;
		}
	};

	template<typename Value, typename Key, typename HashFunc,
		typename ExtraKey, typename EqualKey, typename Alloc>
	struct __HashTableConstIterator
	{
		typedef HashTable<Value, Key, HashFunc, ExtraKey, EqualKey, Alloc>					HashTable;
		typedef __HashTableIterator<Value, Key, HashFunc, ExtraKey, EqualKey, Alloc>		iterator;
		typedef __HashTableConstIterator<Value, Key, HashFunc, ExtraKey, EqualKey, Alloc>	const_iterator;
		typedef __HashTableNode<Value> node;


		typedef ForwardIteratorTag	iterator_category;
		typedef ptrdiff_t			difference_type;
		typedef size_t				size_type;
		typedef Value				value_type;
		typedef const Value&		reference;
		typedef const Value*		pointer;


		// 迭代器目前所指节点
		const node *m_pCur;

		// 保持容器的连接关系
		const HashTable *m_pHashTable;

		__HashTableConstIterator(const node *pNode, const HashTable *table)
			: m_pCur(pNode)
			, m_pHashTable(table)
		{}
		__HashTableConstIterator(const iterator &iter)
			: m_pCur(iter.m_pCur)
			, m_pHashTable(iter.m_pHashTable)
		{}
		__HashTableConstIterator()
		{}

		reference operator*() const
		{
			return m_pCur->m_value;
		}
		pointer operator->() const
		{
			return &(operator*());
		}

		const_iterator &operator++()
		{
			const node *pOld = m_pCur;
			m_pCur = m_pCur->m_pNext;

			if( !m_pCur ) 
			{
				size_type bucket = m_pHashTable->_BucketNum(pOld->m_value);
				while( !m_pCur && ++bucket < m_pHashTable->m_buckets.size() )
					m_pCur = m_pHashTable->m_buckets[bucket];
			}

			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			++*this;
			return tmp;
		}

		bool operator==(const const_iterator &iter) const
		{
			return m_pCur == iter.m_pCur;
		}
		bool operator!=(const const_iterator &iter) const
		{
			return m_pCur != iter.m_pCur;
		}
	};


	
	template<typename Value, typename Key, typename HashFunc, 
		typename ExtractKey, typename EqualKey, typename Alloc>
	class HashTable
	{
	public:
		typedef Key					KeyType;
		typedef Value				value_type;
		typedef HashFunc			HashFunc;
		typedef EqualKey			EqualKey;
		typedef ExtractKey			ExtraKey;

		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;

		typedef __HashTableNode<value_type> node;
		typedef __HashTableIterator<Value, Key, HashFunc, ExtraKey, EqualKey, Alloc>		iterator;
		typedef __HashTableConstIterator<Value, Key, HashFunc, ExtraKey, EqualKey, Alloc>	const_iterator;

		
		friend __HashTableIterator<Value, Key, HashFunc, ExtraKey, EqualKey, Alloc>;
		friend __HashTableConstIterator<Value, Key, HashFunc, ExtraKey, EqualKey, Alloc>;

	private:
		HashFunc m_hash;
		EqualKey m_EqualKey;
		ExtraKey m_ExtraKey;

		Vector<node *> m_buckets;
		size_type m_numElements;


		// 没有Default Constructor
	public:
		HashTable(size_type n, const HashFunc &HashFunc, const EqualKey &equalKey, const ExtraKey &extraKey)
			: m_hash(HashFunc)
			, m_EqualKey(equalKey)
			, m_ExtraKey(extraKey)
			, m_numElements(0)
		{
			_InitializeBuckets(n);
		}
		HashTable(size_type n, const HashFunc &HashFunc, const EqualKey &equalKey)
			: m_hash(HashFunc)
			, m_EqualKey(equalKey)
			, m_ExtraKey(ExtraKey())
			, m_numElements(0)
		{
			_InitializeBuckets(n);
		}
		HashTable(const HashTable &rhs)
			: m_hash(rhs.m_hash)
			, m_EqualKey(rhs.m_EqualKey)
			, m_ExtraKey(rhs.m_ExtraKey)
			, m_numElements(0)
		{
			_CopyFrom(rhs);
		}

		HashTable &operator=(const HashTable &rhs)
		{
			if( &rhs != this )
			{
				clear();		// 先清除
				m_hash		= rhs.m_hash;
				m_EqualKey	= rhs.m_EqualKey;
				m_ExtraKey	= rhs.m_ExtraKey;

				_CopyFrom(rhs);
			}
		}
		~HashTable()
		{
			clear();
		}

	public:
		HashFunc GetHashFunction() const
		{
			return m_hash;
		}
		EqualKey GetEqualKey() const
		{
			return m_EqualKey;
		}

		size_type size() const 
		{ 
			return m_numElements; 
		}
		size_type MaxSize() const 
		{ 
			return size_type(-1); 
		}

		bool empty() const 
		{ 
			return size() == 0;
		}


		void swap(HashTable &rhs)
		{
			swap(m_hash, rhs.m_hash);
			swap(m_EqualKey, rhs.m_EqualKey);
			swap(m_ExtraKey, rhs.m_ExtraKey);

			m_buckets.swap(rhs.m_buckets);

			swap(m_numElements, rhs.m_numElements);
		}


		iterator begin()
		{ 
			for(size_type n = 0; n < m_buckets.size(); ++n)
				// 找出第一個被使用的节点，此即 begin iterator。
				if( m_buckets[n] )
					return iterator(m_buckets[n], this);

			return end();
		}

		iterator end() 
		{ 
			return iterator(0, this);
		}

		const_iterator begin() const
		{
			for(size_type n = 0; n < m_buckets.size(); ++n)
				if( m_buckets[n] )
					return const_iterator(m_buckets[n], this);

			return end();
		}

		const_iterator end() const 
		{ 
			return const_iterator(0, this); 
		}

		// bucket 的大小
		size_type BucketCount() const 
		{ 
			return m_buckets.size();
		}

		// 以目前情況（不重建表格）
		size_type MaxBucketCount() const
		{ 
			return __PrimeList[__NumPrimes - 1];
		} 

		// 得到bucket可容納多少元素。
		size_type ElementsInBuckets(size_type bucket) const
		{
			size_type result = 0;

			for(node* cur = m_buckets[bucket]; cur; cur = cur->m_pNext)
				result += 1;

			return result;
		}


		// 插入元素，不允许重复
		Pair<iterator, bool> InsertUnique(const value_type &obj)
		{
			resize(m_numElements + 1); 	// 判断是否需要重建表格，如需要就扩充

			return _InsertUniqueNoResize(obj);
		}

		// 安插元素，允許重复
		iterator InsertEqual(const value_type &obj)
		{
			resize(m_numElements + 1);	// 判断是否需要重建表格，如需要就扩充

			return _InsertEqualNoResize(obj);
		}

		template<typename InputIterator>
		void InsertUnique(InputIterator first, InputIterator last)
		{
			_InsertUnique(first, last, IteratorTraits<InputIterator>::iterator_category());
		}

		template<typename InputIterator>
		void InsertEqual(InputIterator first, InputIterator last)
		{
			_InsertEqual(first, last, IteratorTraits<InputIterator>::iterator_category());
		}

	

		reference FindOrInsert(const value_type &obj)
		{
			resize(m_numElements + 1);

			size_type n = _BucketNum(obj);
			node *first = m_buckets[n];


			for(node *pCur = first; pCur != NULL; pCur = pCur->m_pNext)
			{
				if( m_EqualKey(m_ExtraKey(pCur->m_value), m_ExtraKey(obj)) )
					return pCur->m_value;
			}

			node *tmp		= _NewNode(obj);
			tmp->m_pNext	= first;
			m_buckets[n]	= tmp;
			++m_numElements;

			return tmp->m_value;
		}

		iterator Find(const KeyType &key) 
		{
			size_type n = _BucketNumKey(key);	// 判断在哪个Bucket中

			node *first = NULL;
			// 循环遍历比较。
			for( first = m_buckets[n];	
				first && !m_EqualKey(m_ExtraKey(first->m_value), key);
				first = first->m_pNext )
				;

			return iterator(first, this);
		} 

		const_iterator Find(const KeyType &key) const
		{
			size_type n = _BucketNumKey(key);
			const node* first = NULL;

			for( first = m_buckets[n];
				first && !m_EqualKey(m_ExtraKey(first->m_value), key);
				first = first->m_pNext)
				;

			return const_iterator(first, this);
		} 

		size_type Count(const KeyType& key) const
		{
			const size_type n = _BucketNumKey(key); 
			size_type result = 0;


			for(const node* cur = m_buckets[n]; cur; cur = cur->m_pNext)
				if( m_EqualKey(m_ExtraKey(cur->m_value), key) )
					++result;

			return result;
		}


		Pair<iterator, iterator> EqualRange(const KeyType &key)
		{
			typedef Pair<iterator, iterator> pii;
			const size_type n = _BucketNumKey(key);

			for(node* first = m_buckets[n]; first; first = first->m_pNext)
			{
				if( m_EqualKey(m_ExtraKey(first->m_value), key) ) 
				{
					for(node* cur = first->m_pNext; cur; cur = cur->m_pNext)
						if( !m_EqualKey(m_ExtraKey(cur->m_value), key) )
							return pii(iterator(first, this), iterator(cur, this));
					
					for(size_type m = n + 1; m < m_buckets.size(); ++m)
						if( m_buckets[m] )
							return pii(iterator(first, this), iterator(m_buckets[m], this));
					
					
					return pii(iterator(first, this), end());
				}
			}

			return pii(end(), end());
		}
		Pair<const_iterator, const_iterator> EqualRange(const KeyType &key) const
		{
			typedef Pair<const_iterator, const_iterator> pii;
			const size_type n = _BucketNumKey(key);

			for(const node* first = m_buckets[n]; first; first = first->m_pNext)
			{
				if( m_EqualKey(m_ExtraKey(first->m_value), key) ) 
				{
					for(const node* cur = first->m_pNext; cur; cur = cur->m_pNext)
						if( !m_EqualKey(m_ExtraKey(cur->m_value), key) )
							return pii(const_iterator(first, this), const_iterator(cur, this));

					for(size_type m = n + 1; m < m_buckets.size(); ++m)
						if( m_buckets[m] )
							return pii(const_iterator(first, this), const_iterator(m_buckets[m], this));


					return pii(const_iterator(first, this), end());
				}
			}

			return pii(end(), end());

		}

		size_type erase(const KeyType &key)
		{
			const size_type n = _BucketNumKey(key);
			node* first = m_buckets[n];
			size_type erased = 0;

			if( first ) 
			{
				node* cur = first;
				node* next = cur->m_pNext;

				while(next)
				{
					if( m_EqualKey(m_ExtraKey(next->m_value), key) ) 
					{
						cur->m_pNext = next->m_pNext;
						_DeleteNode(next);

						next = cur->m_pNext;
						++erased;
						--m_numElements;
					}
					else
					{
						cur		= next;
						next	= cur->m_pNext;
					}
				}
				if( m_EqualKey(m_ExtraKey(first->m_value), key) ) 
				{
					m_buckets[n] = first->m_pNext;
					_DeleteNode(first);
					++erased;
					--m_numElements;
				}
			}

			return erased;
		}
		void erase(const iterator &it)
		{
			if( node* const p = it.m_pCur ) 
			{
				const size_type n = _BucketNum(p->m_value);
				node* cur = m_buckets[n];

				if( cur == p ) 
				{
					m_buckets[n] = cur->m_pNext;
					_DeleteNode(cur);
					--m_numElements;
				}
				else 
				{
					node* next = cur->m_pNext;
					while(next) 
					{
						if( next == p )
						{
							cur->m_pNext = next->m_pNext;
							_DeleteNode(next);
							--m_numElements;
							break;
						}
						else 
						{
							cur = next;
							next = cur->m_pNext;
						}
					}
				}
			}

		}
		void erase(iterator first, iterator last)
		{
			size_type f_bucket = first.m_pCur ? _BucketNum(first.m_pCur->m_value) : m_buckets.size();
			size_type l_bucket = last.m_pCur ? _BucketNum(last.m_pCur->m_value) : m_buckets.size();

			if( first.m_pCur == last.m_pCur )
				return;
			else if( f_bucket == l_bucket )
				_EraseBucket(f_bucket, first.m_pCur, last.m_pCur);
			else 
			{
				_EraseBucket(f_bucket, first.m_pCur, 0);

				for(size_type n = f_bucket + 1; n < l_bucket; ++n)
					_EraseBucket(n, 0);

				if( l_bucket != m_buckets.size() )
					_EraseBucket(l_bucket, last.m_pCur);
			}

		}
		void erase(const const_iterator& iter)
		{
			erase(iterator(const_cast<node *>(iter.m_pCur),
				const_cast<HashTable *>(iter.m_pHashTable)));
		}
		void erase(const_iterator first, const_iterator last)
		{
			erase(iterator(const_cast<node *>(first.m_pCur),
				const_cast<HashTable *>(first.m_pHashTable)),
				iterator(const_cast<node *>(last.m_pCur),
				const_cast<HashTable *>(last.m_pHashTable)));
		}

		void resize(size_type numElements)
		{
			const size_type old_n = m_buckets.size();
			
			if( numElements > old_n) 
			{	
				// 找出下一个质数
				const size_type n = _NextSize(numElements);	
				if( n > old_n )
				{
					// 建立新的 buckets
					Vector<node*> tmp(n, NULL);	
					
					try
					{
						// 处理每个bucket
						for(size_type bucket = 0; bucket < old_n; ++bucket)
						{
							// 指向节点对应列的起始点
							node* first = m_buckets[bucket]; 
							
							// 处理每一个节点
							while(first)
							{	
								// 以下找出节点落在哪一个新bucket 內
								size_type new_bucket = _BucketNum(first->m_value, n);
					
								// (1) 令 bucket 指向其所对应串列的下一个节点
								m_buckets[bucket] = first->m_pNext; 
								// (2)(3) 将当前节点插入buckets内，成为其对应的第一个节点
								first->m_pNext	= tmp[new_bucket];	
								tmp[new_bucket] = first;
								// (4) 回到buckets
								first = m_buckets[bucket];          
							}
						}

						m_buckets.swap(tmp);	// Vector::swap
					}
					catch(...)
					{
						for(size_type bucket = 0; bucket < tmp.size(); ++bucket)
						{
							while(tmp[bucket]) 
							{
								node* next = tmp[bucket]->m_pNext;
								_DeleteNode(tmp[bucket]);
								tmp[bucket] = next;
							}
						}
						throw;
					}
				}
			}

		}
		void clear()
		{
			// 针对每一个 bucket.
			for(size_type i = 0; i < m_buckets.size(); ++i)
			{
				node* cur = m_buckets[i];

				// 将bucket list 中的每一个刪除掉
				while(cur != 0)
				{
					node* next = cur->m_pNext;
					_DeleteNode(cur);
					cur = next;
				}

				m_buckets[i] = 0; 	// 令bucket 內容为 null
			}

			m_numElements = 0; 
		}

		friend bool operator==(const HashTable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>& lhs, const HashTable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>& rhs);

	private:

		template<typename InputIterator>
		void _InsertUnique(InputIterator first, InputIterator last, InputIteratorTag)
		{
			for ( ; first != last; ++first)
				InsertUnique(*first);
		}

		template<typename InputIterator>
		void _InsertEqual(InputIterator first, InputIterator last, InputIteratorTag)
		{
			for ( ; first != last; ++first)
				InsertEqual(*first);
		}

		template<typename ForwardIterator>
		void _InsertUnique(ForwardIterator first, ForwardIterator last, ForwardIteratorTag)
		{
			size_type n = 0;

			DistanceFunc(first, last, n);
			resize(m_numElements + n);

			for ( ; n > 0; --n, ++first)
				_InsertUniqueNoResize(*first);	// 一一安插新元素
		}

		template<typename ForwardIterator>
		void _InsertEqual(ForwardIterator first, ForwardIterator last, ForwardIteratorTag)
		{
			size_type n = 0;
			DistanceFunc(first, last, n);

			resize(m_numElements + n);		
			for ( ; n > 0; --n, ++first)
				_InsertEqualNoResize(*first);	
		}

		Pair<iterator, bool> _InsertUniqueNoResize(const value_type &obj)
		{
			const size_type n = _BucketNum(obj);
			node *first = m_buckets[n];

			// 如果buckets[n]已被占用，此时first将不为0，于是进入下回圈
			for(node *pCur = first; pCur != NULL; pCur = pCur->m_pNext)
			{
				if( m_EqualKey(m_ExtraKey(pCur->m_value), m_ExtraKey(obj)) )
					// 未发现与列中的某键值相同，则不插入
					return Pair<iterator, bool>(iterator(pCur, this), false);
			}

			// 离开回圈时，first指向buckets所指串列的头部节点
			node *tmp		= _NewNode(obj);
			tmp->m_pNext	= first;
			m_buckets[n]	= tmp;
			++m_numElements;

			return Pair<iterator, bool>(iterator(tmp, this), true);
		}
		iterator _InsertEqualNoResize(const value_type &obj)
		{
			const size_type n = _BucketNum(obj);
			node *first = m_buckets[n];

			// 如果buckets[n]被占用，此时first将不为0，于是进入下个回圈
			for(node *pCur = first; pCur != NULL; pCur = pCur->m_pNext)
			{
				if( m_EqualKey(m_ExtraKey(pCur->m_value), m_ExtraKey(obj)) )
				{
					node *tmp		= _NewNode(obj);
					tmp->m_pNext	= pCur->m_pNext;
					pCur->m_pNext	= tmp;
					++m_numElements;

					return iterator(tmp, this);
				}
			}

			// 离开回圈时，first指向buckets所指串列的头部节点
			node *tmp		= _NewNode(obj);
			tmp->m_pNext	= first;
			m_buckets[n]	= tmp;
			++m_numElements;

			return iterator(tmp, this);
		}


		//  下一个质数
		size_type _NextSize(size_type n) const
		{ 
			return __NextPrime(n); 
		}

		void _InitializeBuckets(size_type n)
		{
			const size_type nbuckets = _NextSize(n); 
			

			m_buckets.reserve(nbuckets);
			m_buckets.insert(m_buckets.end(), nbuckets, NULL);
			
			m_numElements = 0;
		}

		size_type _BucketNumKey(const KeyType &key) const
		{
			return _BucketNumKey(key, m_buckets.size());
		}

		size_type _BucketNum(const value_type &obj) const
		{
			return _BucketNumKey(m_ExtraKey(obj));
		}

		size_type _BucketNumKey(const KeyType &key, size_t n) const
		{
			return m_hash(key) % n;
		}

		size_type _BucketNum(const value_type &obj, size_t n) const
		{
			return _BucketNumKey(m_ExtraKey(obj), n);
		}

		node *_NewNode(const value_type &obj)
		{
			node *pNode = Alloc::Allocate(sizeof(node));
			pNode->m_pNext = 0;

			try 
			{
				__Construct(&pNode->m_value, obj);
				return pNode;
			}
			catch(...)
			{
				Alloc::Deallocate(pNode, sizeof(node));
			}

			return pNode;
		}

		void _DeleteNode(node *pNode)
		{
			__Destroy(&pNode->m_value);
			Alloc::Deallocate(pNode, sizeof(node));
		}

		void _EraseBucket(const size_type n, node *first, node *last)
		{
			node* cur = m_buckets[n];
			if( cur == first )
				_EraseBucket(n, last);
			else
			{
				node* next;
				for(next = cur->m_pNext; next != first; cur = next, next = cur->m_pNext)
					;

				while(next) 
				{
					cur->m_pNext = next->m_pNext;
					_DeleteNode(next);
					next = cur->m_pNext;
					--m_numElements;
				}
			}

		}
		void _EraseBucket(const size_type n, node *last)
		{
			node* cur = m_buckets[n];

			while(cur != last) 
			{
				node* next = cur->m_pNext;
				_DeleteNode(cur);
				cur = next;
				m_buckets[n] = cur;
				--m_numElements;
			}
		}

		void _CopyFrom(const HashTable &rhs)
		{
			// 先清除自己的buckets Vector.
			m_buckets.clear();	
			// 将自己的buckets vector预分配空间
			m_buckets.reserve(rhs.m_buckets.size()); 
			m_buckets.insert(m_buckets.end(), rhs.m_buckets.size(), NULL);
			
			try
			{
				// 针对buckets vector 
				for(size_type i = 0; i < rhs.m_buckets.size(); ++i)
				{
					// 复制vector 的每一个元素
					if( const node* cur = rhs.m_buckets[i] ) 
					{
						node* copy = _NewNode(cur->val);
						m_buckets[i] = copy;

						// 针对同一个 bucket list
						for(node* next = cur->m_pNext; next; cur = next, next = cur->m_pNext)
						{
							copy->m_pNext = _NewNode(next->m_value);
							copy = copy->m_pNext;
						}
					}
				}
				
				m_numElements= rhs.m_numElements;
			}
			catch(...)
			{
				clear();
			}

		}

	};


	template<typename V, typename K, typename HF, typename Ex, typename Eq, typename A>
	bool operator==(const HashTable<V, K, HF, Ex, Eq, A> &lhs, const HashTable<V, K, HF, Ex, Eq, A> &rhs)
	{
		typedef typename HashTable<V, K, HF, Ex, Eq, A>::node node;

		if( lhs.size() != rhs.size() )
			return false;

		for( int n = 0; n < lhs.size(); ++n )
		{
			node *cur1 = lhs.m_buckets[n];
			node *cur2 = rhs.m_buckets[n];

			for( ; cur1 && cur2 && cur1->m_value == cur2->m_value;
				cur1 = cur1->m_pNext, cur2 = cur2->m_pNext)
				;

			if( cur1 || cur2 )
				return false;
		}

		return true;
	}

}




#endif