#ifndef __HASH_MAP_HPP
#define __HASH_MAP_HPP


#include "HashTable.hpp"


namespace DataStructure
{
	// HashMap
	template<
		typename Key, 
		typename T,
		typename HashFunc	= Hash<Key>, 
		typename EqualKey	= EqualTo<Key>,
		typename Alloc		= Allocator<Key>
	>
	class HashMap
	{
	private:
		typedef HashTable<Pair<const Key, T>, Key, HashFunc, Select1st<Pair<const Key, T>>, EqualKey> HashTable;
		HashTable m_hashtable;

	public:
		typedef typename HashTable::KeyType		KeyType;
		typedef T								DataType;
		typedef T								MappedType;
		typedef typename HashTable::value_type	value_type;
		typedef typename HashTable::HashFunc	HashFunc;
		typedef typename HashTable::EqualKey	EqualKey;

		typedef typename HashTable::size_type		size_type;
		typedef typename HashTable::difference_type difference_type;
		typedef typename HashTable::pointer			pointer;
		typedef typename HashTable::const_pointer	const_pointer;
		typedef typename HashTable::iterator		iterator;
		typedef typename HashTable::const_iterator	const_iterator;
		typedef typename HashTable::reference		reference;
		typedef typename HashTable::const_reference const_reference;


	public:
		HashMap()
			: m_hashtable(100, HashFunc(), EqualKey())
		{}
		explicit HashMap(size_type n)
			: m_hashtable(n, HashFunc(), EqualKey())
		{}
		HashMap(size_type n, const HashFunc &hashFunc)
			: m_hashtable(n, hashFunc, EqualKey())
		{}
		HashMap(size_type n, const HashFunc &hashFunc, const EqualKey &equalKey)
			: m_hashtable(n, hashFunc, equalKey)
		{}
		template<typename iterator>
		HashMap(iterator first, iterator last)
			: m_hashtable(100, HashFunc(), EqualKey())
		{
			m_hashtable.InsertUnique(first, last);
		}
		template<typename iterator>
		HashMap(iterator first, iterator last, size_type n)
			: m_hashtable(n, HashFunc(), EqualKey())
		{
			m_hashtable.InsertUnique(first, last);
		}
		template<typename iterator>
		HashMap(iterator first, iterator last, size_type n, const HashFunc &hashFunc)
			: m_hashtable(n, hashFunc, EqualKey())
		{
			m_hashtable.InsertUnique(first, last);
		}
		template<typename iterator>
		HashMap(iterator first, iterator last, size_type n, const HashFunc &hashFunc, const EqualKey &equalKey)
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

		void swap(HashMap &rhs)
		{
			return m_hashtable.swap(rhs.m_hashtable);
		}

		iterator begin()
		{
			return m_hashtable.begin();
		}
		iterator end()
		{
			return m_hashtable.end();
		}

		const_iterator begin() const
		{
			return m_hashtable.begin();
		}
		const_iterator end() const
		{
			return m_hashtable.end();
		}
	
		T &operator[](const KeyType &key)
		{
			return (m_hashtable.FindOrInsert(value_type(key, T()))).second;
		}

		Pair<iterator, bool> insert(const value_type &obj)
		{
			return m_hashtable.InsertUnique(obj);
		}
		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			m_hashtable.InsertUnique(first, last);
		}

		iterator Find(const KeyType &key)
		{
			return m_hashtable.Find(key);
		}
		const_iterator Find(const KeyType &key) const
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

		friend bool operator==(const HashMap &lhs, const HashMap &rhs)
		{
			return lhs.m_hashtable == rhs.m_hashtable;
		}


	};



	// HashMultiMap
	template<
		typename Key, 
		typename T,
		typename HashFunc	= Hash<Key>, 
		typename EqualKey	= EqualTo<Key>,
		typename Alloc		= Allocator<Key>
	>
	class HashMultiMap
	{
	private:
		typedef HashTable<Pair<const Key, T>, Key, HashFunc, Select1st<Pair<const Key, T>>, EqualKey> HashTable;
		HashTable m_hashtable;

	public:
		typedef typename HashTable::KeyType		KeyType;
		typedef T								DataType;
		typedef T								MappedType;
		typedef typename HashTable::value_type	value_type;
		typedef typename HashTable::HashFunc	HashFunc;
		typedef typename HashTable::EqualKey	EqualKey;

		typedef typename HashTable::size_type		size_type;
		typedef typename HashTable::difference_type difference_type;
		typedef typename HashTable::pointer			pointer;
		typedef typename HashTable::const_pointer	const_pointer;
		typedef typename HashTable::iterator		iterator;
		typedef typename HashTable::const_iterator	const_iterator;
		typedef typename HashTable::reference		reference;
		typedef typename HashTable::const_reference const_reference;


	public:
		HashMultiMap()
			: m_hashtable(100, HashFunc(), EqualKey())
		{}
		explicit HashMultiMap(size_type n)
			: m_hashtable(n, HashFunc(), EqualKey())
		{}
		HashMultiMap(size_type n, const HashFunc &hashFunc)
			: m_hashtable(n, hashFunc, EqualKey())
		{}
		HashMultiMap(size_type n, const HashFunc &hashFunc, const EqualKey &equalKey)
			: m_hashtable(n, hashFunc, equalKey)
		{}
		template<typename iterator>
		HashMultiMap(iterator first, iterator last)
			: m_hashtable(100, HashFunc(), EqualKey())
		{
			m_hashtable.InsertEqual(first, last);
		}
		template<typename iterator>
		HashMultiMap(iterator first, iterator last, size_type n)
			: m_hashtable(n, HashFunc(), EqualKey())
		{
			m_hashtable.InsertEqual(first, last);
		}
		template<typename iterator>
		HashMultiMap(iterator first, iterator last, size_type n, const HashFunc &hashFunc)
			: m_hashtable(n, hashFunc, EqualKey())
		{
			m_hashtable.InsertEqual(first, last);
		}
		template<typename iterator>
		HashMultiMap(iterator first, iterator last, size_type n, const HashFunc &hashFunc, const EqualKey &equalKey)
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

		void swap(HashMultiMap &rhs)
		{
			return m_hashtable.swap(rhs.m_hashtable);
		}

		iterator begin()
		{
			return m_hashtable.begin();
		}
		iterator end()
		{
			return m_hashtable.end();
		}

		const_iterator begin() const
		{
			return m_hashtable.begin();
		}
		const_iterator end() const
		{
			return m_hashtable.end();
		}

		Pair<iterator, bool> insert(const value_type &obj)
		{
			return m_hashtable.InsertEqual(obj);
		}
		template<typename InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			m_hashtable.InsertEqual(first, last);
		}

		iterator Find(const KeyType &key)
		{
			return m_hashtable.Find(key);
		}
		const_iterator Find(const KeyType &key) const
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

		friend bool operator==(const HashMultiMap &lhs, const HashMultiMap &rhs)
		{
			return lhs.m_hashtable == rhs.m_hashtable;
		}


	};


}

#endif