#ifndef __CACHE_CACHE_HPP
#define __CACHE_CACHE_HPP



#include <list>
#include <utility>
#include <functional>
#include <algorithm>


namespace DataStructure
{

	// KeyT		Cache中关键字的类型
	// ValueT	Cache中值的类型
	// DisposeFuc 当元素移除时会回调的函数指针


	//---------------------------------------------------------
	// class Cache

	template<typename KeyT, typename ValueT>
	class Cache
	{
	public:
		typedef KeyT			KeyType;
		typedef ValueT			ValueType;

		typedef std::tr1::function<void(const KeyType &, const ValueType &)>	CallbackType;


	private:
		// helper types
		typedef std::pair<KeyType, typename std::list<ValueType>::iterator>		KeyListType;
		typedef typename std::list<KeyListType>::iterator						KeyListIterator;
		typedef typename std::list<KeyListType>::const_iterator					KeyListConstIterator;

	
	private:
		std::list<KeyListType>		keyList_;
		std::list<ValueType>		valueList_;
		size_t						maxSize_;
		CallbackType				dispFunc_;


	public:
		// functor，比较Cache中的Key
		struct KeyEqual
			: public std::unary_function<KeyListType, bool>
		{
		private:
			const KeyType &k_;

		public:
			KeyEqual(const KeyType &k)
				: k_(k)
			{}

			result_type operator()(const argument_type &val) const
			{
				return val.first == k_;
			}
		};

		// 标志Cache中元素的位置
		struct cache_iterator
		{
			KeyListIterator i_;

			cache_iterator(KeyListIterator i)
				: i_(i)
			{}

			bool operator==(const cache_iterator &rhs)
			{
				return this->i_ == rhs.i_;
			}
		};


	public:
		Cache(size_t maxSize, CallbackType disFunc)
			: maxSize_(maxSize)
			, dispFunc_(disFunc)
		{}
		~Cache()
		{
			clear();
		}

	private:
		Cache(const Cache &);
		Cache &operator=(const Cache &);


	public:
		// 插入元素到Cache中
		void insert(const KeyType &key, const ValueType &val)
		{
			valueList_.push_front(val);
			keyList_.push_front(std::make_pair(key, valueList_.begin()));

			if( valueList_.size() > maxSize_ )
			{
				KeyListIterator keyIter = keyList_.end();
				--keyIter;
				
				_CallDisposeFunc(keyIter);
				valueList_.erase(keyIter->second);

				keyList_.pop_back();
			}
		}

		// 移除元素
		void remove(const KeyType &key)
		{
			_RemoveImpl(_FindKey(key));
		}

		void remove(const cache_iterator &iter)
		{
			_RemoveImpl(iter.i_);
		}

		// 清空
		void clear()
		{
			for(KeyListIterator iter = keyList_.begin(); iter != keyList_.end(); ++iter)
				_CallDisposeFunc(iter);

			keyList_.clear();
			valueList_.clear();
		}

		cache_iterator find(const KeyT &key)
		{
			return cache_iterator(_FindKey(key));
		}

		cache_iterator end()
		{
			return cache_iterator(keyList_.end());
		}

		const ValueType &get(cache_iterator &iter) const
		{
			// 获取最新的元素
			return *(_GetTop(iter.i_)->second);
		}

		ValueType &get(cache_iterator &iter)
		{
			return *(_GetTop(iter.i_)->second);
		}


	private:
		KeyListIterator _FindKey(const KeyType &key)
		{
			return std::find_if(keyList_.begin(), keyList_.end(), KeyEqual(key));
		}

		KeyListIterator _GetTop(KeyListIterator &iter)
		{
			KeyListType keyList(*iter);

			keyList_.erase(iter);
			keyList_.push_back(keyList);

			iter = keyList_.begin();
			return iter;
		}

		void _RemoveImpl(const KeyListIterator &iter)
		{
			if( iter != keyList_.end() )
			{
				_CallDisposeFunc(iter);

				valueList_.erase(iter->second);
				keyList_.erase(iter);
			}
		}

		void _CallDisposeFunc(const KeyListIterator &iter)
		{
			dispFunc_(iter->first, *(iter->second));
		}
		
	};



	using std::tr1::placeholders::_1;
	using std::tr1::placeholders::_2;
}


#endif