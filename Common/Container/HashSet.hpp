#ifndef __HASH_SET_HPP
#define __HASH_SET_HPP


#include "HashTable.hpp"


namespace DataStructure
{
	
	template<
		typename Value, 
		typename HashFunc	= Hash<Value>, 
		typename EqualKey	= EqualTo<Value>,
		typename Alloc		= Allocator<Value>
	>
	class HashSet
	{
	private:
		typedef HashTable<Value, Value, HashFunc, Identity<Value>, EqualKey> HashTable;
		HashTable m_hashtable;

	public:
		typedef typename HashTable::KeyType		KeyType;
		typedef typename HashTable::value_type	value_type;
		typedef typename HashTable::HashFunc	HashFunc;
		typedef typename HashTable::EqualKey	EqualKey;

		typedef typename HashTable::size_type		size_type;
		typedef typename HashTable::difference_type difference_type;
		typedef typename HashTable::const_pointer	pointer;
		typedef typename HashTable::const_pointer	const_pointer;
		typedef typename HashTable::const_iterator	iterator;
		typedef typename HashTable::const_iterator	const_iterator;


	public:
		HashSet()
			: m_hashtable(100, HashFunc(), EqualKey())
		{}
		explicit HashSet(size_type n)
			: m_hashtable(n, HashFunc(), EqualKey())
		{}
		HashSet(size_type n, const HashFunc &hashFunc)
			: m_hashtable(n, hashFunc, EqualKey())
		{}
		HashSet(size_type n, const HashFunc &hashFunc, const EqualKey &equalKey)
			: m_hashtable(n, hashFunc, equalKey)
		{}
		template<typename iterator>
		HashSet(iterator first, iterator last)
			: m_hashtable(100, HashFunc(), EqualKey())
		{
			m_hashtable.InsertUnique(first, last);
		}
		template<typename iterator>
		HashSet(iterator first, iterator last, size_type n)
			: m_hashtable(n, HashFunc(), EqualKey())
		{
			m_hashtable.InsertUnique(first, last);
		}
		template<typename iterator>
		HashSet(iterator first, iterator last, size_type n, const HashFunc &hashFunc)
			: m_hashtable(n, hashFunc, EqualKey())
		{
			m_hashtable.InsertUnique(first, last);
		}
		template<typename iterator>
		HashSet(iterator first, iterator last, size_type n, const HashFunc &hashFunc, const EqualKey &equalKey)
			: m_hashtable(n, hashFunc, equalKey)
		{
			m_hashtable.InsertUnique(first, last);
		}

	public:
		HashFunc GetHashFunction() const
		{
			return m_hashtable.GetHashFunction();
		}
		KeyType GetEqualKey() const
		{
			return m_hashtable.GetEqualKey();
		}

		size_type size() const
		{
			return m_hashtable.size();
		}
		size_type MaxSize() const
		{
			return m_hashtable.MaxSize();
		}

		bool empty() const
		{
			return m_hashtable.empty();
		}

		void swap(HashSet &rhs)
		{
			return m_hashtable.swap(rhs.m_hashtable);
		}

		iterator begin() const
		{
			return m_hashtable.begin();
		}
		iterator end() const
		{
			return m_hashtable.end();
		}

		Pair<iterator, bool> insert(const value_type &obj)
		{
			Pair<typename HashTable::iterator, bool> p = m_hashtable.InsertUnique(obj);
			
			return Pair<iterator, bool>(p.first, p.second);
		}
		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			m_hashtable.InsertUnique(first, last);
		}

		iterator Find(const KeyType &key) const
		{
			return m_hashtable.Find(key);
		}

		size_type Count(const KeyType &key) const
		{
			return m_hashtable.Count(key);
		}

		Pair<iterator, iterator> EqualRange(const KeyType &key) const
		{
			return m_hashtable.EqualRange(key);
		}

		size_type erase(const KeyType &key)
		{
			return m_hashtable.erase(key);
		}

		void erase(iterator iter)
		{
			return m_hashtable.erase(iter);
		}

		void erase(iterator first, iterator last) 
		{
			return m_hashtable.erase(first, last);
		}

		size_type ElementsInBuckets(size_type n) const
		{
			return m_hashtable.ElementsInBuckets(n);
		}

		void resize(size_type n)
		{
			m_hashtable.resize(n);
		}

		size_type BucketCount() const
		{
			return m_hashtable.BucketCount();
		}

		size_type MaxBucketCount() const
		{
			return m_hashtable.MaxBucketCount();
		}

		void clear()
		{
			return m_hashtable.clear();
		}

		friend bool operator==(const HashSet &lhs, const HashSet &rhs)
		{
			return lhs.m_hashtable == rhs.m_hashtable;
		}


	};



	// HashMultiSet
	template<
		typename Value, 
		typename HashFunc	= Hash<Value>, 
		typename EqualKey	= EqualTo<Value>,
		typename Alloc		= Allocator<Value>
	>
	class HashMultiSet
	{
	private:
		typedef HashTable<Value, Value, HashFunc, Identity<Value>, EqualKey> HashTable;
		HashTable m_hashtable;

	public:
		typedef typename HashTable::KeyType		KeyType;
		typedef typename HashTable::value_type	value_type;
		typedef typename HashTable::HashFunc	HashFunc;
		typedef typename HashTable::EqualKey	EqualKey;

		typedef typename HashTable::size_type		size_type;
		typedef typename HashTable::difference_type difference_type;
		typedef typename HashTable::const_pointer	pointer;
		typedef typename HashTable::const_pointer	const_pointer;
		typedef typename HashTable::const_iterator	iterator;
		typedef typename HashTable::const_iterator	const_iterator;


	public:
		HashMultiSet()
			: m_hashtable(100, HashFunc(), EqualKey())
		{}
		explicit HashMultiSet(size_type n)
			: m_hashtable(n, HashFunc(), EqualKey())
		{}
		HashMultiSet(size_type n, const HashFunc &hashFunc)
			: m_hashtable(n, hashFunc, EqualKey())
		{}
		HashMultiSet(size_type n, const HashFunc &hashFunc, const EqualKey &equalKey)
			: m_hashtable(n, hashFunc, equalKey)
		{}
		template<typename iterator>
		HashMultiSet(iterator first, iterator last)
			: m_hashtable(100, HashFunc(), EqualKey())
		{
			m_hashtable.InsertEqual(first, last);
		}
		template<typename iterator>
		HashMultiSet(iterator first, iterator last, size_type n)
			: m_hashtable(n, HashFunc(), EqualKey())
		{
			m_hashtable.InsertEqual(first, last);
		}
		template<typename iterator>
		HashMultiSet(iterator first, iterator last, size_type n, const HashFunc &hashFunc)
			: m_hashtable(n, hashFunc, EqualKey())
		{
			m_hashtable.InsertEqual(first, last);
		}
		template<typename iterator>
		HashMultiSet(iterator first, iterator last, size_type n, const HashFunc &hashFunc, const EqualKey &equalKey)
			: m_hashtable(n, hashFunc, equalKey)
		{
			m_hashtable.InsertEqual(first, last);
		}

	public:
		HashFunc GetHashFunction() const
		{
			return m_hashtable.GetHashFunction();
		}
		KeyType GetEqualKey() const
		{
			return m_hashtable.GetEqualKey();
		}

		size_type size() const
		{
			return m_hashtable.size();
		}
		size_type MaxSize() const
		{
			return m_hashtable.MaxSize();
		}

		bool empty() const
		{
			return m_hashtable.empty();
		}

		void swap(HashMultiSet &rhs)
		{
			return m_hashtable.swap(rhs.m_hashtable);
		}

		iterator begin() const
		{
			return m_hashtable.begin();
		}
		iterator end() const
		{
			return m_hashtable.end();
		}

		iterator insert(const value_type &obj)
		{
			return m_hashtable.InsertEqual(obj);
		}
		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			m_hashtable.InsertEqual(first, last);
		}

		iterator Find(const KeyType &key) const
		{
			return m_hashtable.Find(key);
		}

		size_type Count(const KeyType &key) const
		{
			return m_hashtable.Count(key);
		}

		Pair<iterator, iterator> EqualRange(const KeyType &key) const
		{
			return m_hashtable.EqualRange(key);
		}

		size_type erase(const KeyType &key)
		{
			return m_hashtable.erase(key);
		}

		void erase(iterator iter)
		{
			return m_hashtable.erase(iter);
		}

		void erase(iterator first, iterator last) 
		{
			return m_hashtable.erase(first, last);
		}

		size_type ElementsInBuckets(size_type n) const
		{
			return m_hashtable.ElementsInBuckets(n);
		}

		void resize(size_type n)
		{
			m_hashtable.resize(n);
		}

		size_type BucketCount() const
		{
			return m_hashtable.BucketCount();
		}

		size_type MaxBucketCount() const
		{
			return m_hashtable.MaxBucketCount();
		}

		void clear()
		{
			return m_hashtable.clear();
		}

		friend bool operator==(const HashMultiSet &lhs, const HashMultiSet &rhs)
		{
			return lhs.m_hashtable == rhs.m_hashtable;
		}


	};

}

#endif