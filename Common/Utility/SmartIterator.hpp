#ifndef __SMART_ITERATOR_HPP
#define __SMART_ITERATOR_HPP

#include <type_traits>


namespace DataStructure
{

	// ------------------------------------------------
	// class SmartIterator

	// 智能迭代器，取消双引用运算符: *\->\[]。此迭代器支持原始指针和支持随机存取

	template<typename RawIteratorT>
	class SmartIterator
	{
		typedef RawIteratorT	RawIteratorType;

	public:
		typedef typename std::iterator_traits<RawIteratorType>::value_type			StoedIteratorType;
		typedef typename std::iterator_traits<RawIteratorType>::difference_type		difference_type;
		typedef typename std::iterator_traits<RawIteratorType>::iterator_category	iterator_category;

		typedef typename std::iterator_traits<StoedIteratorType>::value_type		value_type;
		typedef typename std::iterator_traits<StoedIteratorType>::pointer			pointer;
		typedef typename std::iterator_traits<StoedIteratorType>::reference			reference;
		
		//typedef typename std::tr1::add_const<pointer>::type							const_pointer;
		//typedef typename std::tr1::add_const<reference>::type						const_reference;
		
	private:
		RawIteratorType rawIterator_;

	public:
		SmartIterator()
		{}
		explicit SmartIterator(const RawIteratorType &iter)
			: rawIterator_(iter)
		{}
		SmartIterator(const SmartIterator &rhs)
			: rawIterator_(rhs.get())
		{}
		template<typename SmartIteratorU>
		SmartIterator(const SmartIteratorU &iter)
			: rawIterator_(iter.get())
		{}
		template<typename SmartIteratorU>
		SmartIterator &operator=(const SmartIteratorU &iter)
		{
			rawIterator_ = iter.get();
			return *this;
		}

	public:
		reference operator*()
		{
			// 需要双解引用
			return **rawIterator_; 
		}
		pointer operator->()
		{
			// 需要一次解引用
			return *rawIterator_;
		}

		SmartIterator &operator++()
		{
			++rawIterator_;
			return *this;
		}
		SmartIterator operator++(int)
		{
			SmartIterator tmp(*this);
			++(*this);

			return tmp;
		}
		SmartIterator &operator--()
		{
			--rawIterator_;
			return *this;
		}
		SmartIterator operator--(int)
		{
			SmartIterator tmp(*this);
			--(*this);

			return tmp;
		}
		SmartIterator &operator+=(difference_type n)
		{
			rawIterator_ += n;
			return *this;
		}
		SmartIterator &operator-=(difference_type n)
		{
			rawIterator_ -= n;
			return *this;
		}

		reference operator[](difference_type n)
		{
			// 需要一次解引用
			return *(rawIterator_[n]);
		}

		RawIteratorType &get()
		{
			return rawIterator_;
		}
		const RawIteratorType &get() const
		{
			return rawIterator_;
		}
	};

	template<typename RawIteratorT>
	inline bool operator==(const SmartIterator<RawIteratorT> &lhs, const SmartIterator<RawIteratorT> &rhs)
	{
		return lhs.get() == rhs.get();
	}
	template<typename RawIteratorT>
	inline bool operator!=(const SmartIterator<RawIteratorT> &lhs, const SmartIterator<RawIteratorT> &rhs)
	{
		return lhs.get() != rhs.get();
	}

	template<typename RawIteratorT>
	inline bool operator+(const SmartIterator<RawIteratorT> &lhs, typename SmartIterator<RawIteratorT>::difference_type n)
	{
		return SmartIterator<RawIteratorT>(lhs.get() + n);
	}
	template<typename RawIteratorT>
	inline bool operator+(typename SmartIterator<RawIteratorT>::difference_type n, const SmartIterator<RawIteratorT> &lhs)
	{
		return SmartIterator<RawIteratorT>(n + lhs.get());
	}
	template<typename RawIteratorT>
	inline bool operator-(const SmartIterator<RawIteratorT> &lhs, typename SmartIterator<RawIteratorT>::difference_type n)
	{
		return SmartIterator<RawIteratorT>(lhs.get() - n);
	}
	template<typename RawIteratorT>
	inline bool operator-(const SmartIterator<RawIteratorT> &lhs, const SmartIterator<RawIteratorT> &rhs)
	{
		return lhs.get() - rhs.get();
	}

	template<typename RawIteratorT>
	inline bool operator<(const SmartIterator<RawIteratorT> &lhs, const SmartIterator<RawIteratorT> &rhs)
	{
		return lhs.get() < rhs.get();
	}
	template<typename RawIteratorT>
	inline bool operator>(const SmartIterator<RawIteratorT> &lhs, const SmartIterator<RawIteratorT> &rhs)
	{
		return lhs.get() > rhs.get();
	}
	template<typename RawIteratorT>
	inline bool operator<=(const SmartIterator<RawIteratorT> &lhs, const SmartIterator<RawIteratorT> &rhs)
	{
		return lhs.get() <= rhs.get();
	}
	template<typename RawIteratorT>
	inline bool operator>=(const SmartIterator<RawIteratorT> &lhs, const SmartIterator<RawIteratorT> &rhs)
	{
		return lhs.get() >= rhs.get();
	}



	// -----------------------------------------------------------
	// class Dereference

	// 取消解引用，以支持标准算法
	template<typename StoredIteratorT, typename BinaryFuncT>
	class Dereference
	{
	public:
		typedef StoredIteratorT		StoredIteratorType;
		typedef BinaryFuncT			BinaryFunctionType;

		typedef StoredIteratorType	first_argument_type;
		typedef StoredIteratorType	second_argument_type;
		typedef typename BinaryFunctionType::result_type	result_type;

	private:
		BinaryFunctionType func_;

	public:
		Dereference(const BinaryFunctionType &func = BinaryFunctionType())
			: func_(func)
		{}

		result_type operator()(first_argument_type first, second_argument_type second)
		{
			return func_(*first, *second);
		}
	};


	// ----------------------------------------------------
	// class View

	template< typename StoredIteratorT, typename ContainerT = std::vector<StoredIteratorT> >
	class SmartView
	{
		typedef StoredIteratorT		StoredIteratorType;
		typedef ContainerT			ContainerType;

	public:
		// 类似Vector接口
		typedef SmartIterator<typename ContainerType::iterator>				iterator;
		typedef SmartIterator<typename ContainerType::const_iterator>		const_iterator;
		
		typedef typename iterator::reference								reference;
		typedef typename const_iterator::reference							const_reference;
		typedef typename iterator::value_type								value_type;
		typedef typename iterator::pointer									pointer;
		typedef typename const_iterator::pointer							const_pointer;

		typedef typename ContainerType::size_type							size_type;
		typedef typename ContainerType::difference_type						difference_type;
		typedef typename ContainerType::allocator_type						allocator_type;
		

		typedef std::reverse_iterator<iterator>								reverse_iterator;
		typedef std::reverse_iterator<const_iterator>						const_reverse_iterator;

		// 提供访问底层容器的类型
		typedef typename ContainerType::iterator							V_iterator;
		typedef typename ContainerType::const_iterator						V_const_iterator;
		typedef std::reverse_iterator<V_iterator>							V_reverse_iterator;
		typedef std::reverse_iterator<V_const_iterator>						V_const_reverse_iterator;


	private:
		ContainerType container_;

	public:
		SmartView()
		{}
		explicit SmartView(const ContainerType &c)
			: container_(c)
		{}
		explicit SmartView(size_type n, const StoredIteratorType &val = StoredIteratorType())
			: container_(n, val)
		{}
		template<typename InputIteratorT>
		SmartView(InputIteratorT first, InputIteratorT last)
		{
			assign(first, last);
		}
		SmartView(const SmartView &rhs)
			: container_(rhs.container_)
		{}
		SmartView &operator=(const SmartView &rhs)
		{
			container_ = rhs.container_;
		}

	public:
		template<typename InputIteratorT>
		void assign(InputIteratorT first, InputIteratorT last)
		{
			container_.clear();
			while( first != last )
				push_back(first++);
		}
		void assign(size_type n, const StoredIteratorType &val)
		{
			container_.assign(n, val);
		}

		allocator_type &get_allocator() const
		{
			return container_.get_allocator();
		}

		// iterator
		iterator begin()
		{
			return iterator(container_.begin());
		}
		const_iterator begin() const
		{
			return const_iterator(container_.begin());
		}
		iterator end()
		{
			return iterator(container_.end());
		}
		const_iterator end() const
		{
			return const_iterator(container_.end());
		}

		reverse_iterator rbegin()
		{
			return reverse_iterator(container_.end());
		}
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(container_.end());
		}
		reverse_iterator rend()
		{
			return reverse_iterator(container_.begin());
		}
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(container_.begin());
		}

		// 底层容器迭代器
		V_iterator v_begin()
		{
			return container_.begin();
		}
		V_const_iterator v_begin() const
		{
			return container_.begin();
		}
		V_iterator v_end()
		{
			return container_.end();
		}
		V_const_iterator v_end() const
		{
			return container_.end();
		}

		V_reverse_iterator v_rbegin()
		{
			return reverse_iterator(container_.end());
		}
		V_const_reverse_iterator v_rbegin() const
		{
			return const_reverse_iterator(container_.end());
		}
		V_reverse_iterator v_rend()
		{
			return reverse_iterator(container_.begin());
		}
		V_const_reverse_iterator v_rend() const
		{
			return const_reverse_iterator(container_.begin());
		}

		size_type size() const
		{
			return container_.size();
		}
		size_type max_size() const
		{
			return container_.max_size();
		}

		void resize(size_type sz, StoredIteratorType val = StoredIteratorType())
		{
			container_.resize(sz, val);
		}
		size_type capacity() const
		{
			return container_.capacity();
		}
		bool empty() const
		{
			return container_.empty();
		}
		void reserve(size_type n)
		{
			container_.reserve(n);
		}

		reference operator[](size_type n)
		{
			return *(container_[n]);
		}
		const_reference operator[](size_type n) const
		{
			return *(container_[n]);
		}
		
		reference at(size_type n)
		{
			return *(container_.at(n));
		}
		const_reference at(size_type n) const
		{
			return *(container_.at(n));
		}
		StoredIteratorType &v_at(size_type n)
		{
			return container_.at(n);
		}
		StoredIteratorType v_at(size_type n) const
		{
			return container_.at(n);
		}
		

		reference front()
		{
			return *(container_.front());
		}
		const_reference front() const
		{
			return *(container_.front());
		}
		StoredIteratorType &v_front()
		{
			return container_.front();
		}
		StoredIteratorType v_front() const
		{
			return container_.front();
		}

		reference back()
		{
			return *(container_.back());
		}
		const_reference back() const
		{
			return *(container_.back());
		}
		StoredIteratorType &v_back()
		{
			return container_.back();
		}
		StoredIteratorType v_back() const
		{
			return container_.back();
		}

		void push_front(const StoredIteratorType &val)
		{
			container_.push_front(val);
		}
		void pop_front()
		{
			container_.pop_front();
		}
		void push_back(const StoredIteratorType &val)
		{
			container_.push_back(val);
		}
		void pop_back()
		{
			container_.pop_back();
		}

		iterator insert(iterator pos, const StoredIteratorType &val)
		{
			return iterator(container_.insert(pos.get(), val));
		}
		iterator insert(iterator pos, size_type n, const StoredIteratorType &val)
		{
			return iterator(container_.insert(pos.get(), n, val));
		}
		template<typename InputIteratorT>
		void insert(iterator pos, InputIteratorT first, InputIteratorT last)
		{
			V_iterator tmp(pos.get());

			while(first != last)
			{
				tmp = container_.insert(tmp, first++);
				++tmp;
			}
		}

		iterator erase(iterator pos)
		{
			return iterator(container_.erase(pos.get()));
		}
		iterator erase(iterator first, iterator last)
		{
			return iterator(container_.erase(first.get(), last.get()));
		}

		void swap(SmartView &rhs)
		{
			container_.swap(rhs.container_);
		}
		void clear()
		{
			container_.clear();
		}

		void splice(iterator pos, SmartView &rhs)
		{
			container_.splice(pos.get(), rhs.container_);
		}
		void splice(iterator pos, SmartView &rhs, iterator iter)
		{
			container_.splice(pos.get(), rhs.container_, iter.get());
		}
		void splice(iterator pos, SmartView &rhs, iterator first, iterator last)
		{
			container_.splice(pos.get(), rhs.container_, first.get(), last.get());
		}

		void remove(const value_type &val)
		{
			V_iterator iter(v_begin());

			while(iter != v_end())
			{
				if( **iter == val )
					iter = container_.erase(iter);
				else
					++iter;
			}
		}
		void v_remove(const StoredIteratorType &val)
		{
			container_.remove(val.get());
		}

		template<typename PredT>
		void remove_if(PredT pred)
		{
			V_iterator iter(v_begin());

			while(iter != v_end())
			{
				if( pred(**iter) )
					iter = container_.erase(iter);
				else
					++iter;
			}
		}
		template<typename PredT>
		void v_remove_if(PredT pred)
		{
			container_.remove_if(pred);
		}

		void unique()
		{
			V_iterator first(v_begin());
			if( first == v_end() )
				return;

			V_iterator next = first;
			while(++next != v_end())
			{
				if( **first == **next )
				{
					container_.erase(next);
					next = first;
				}
				else
					first = next;
			}
		}
		void v_unique()
		{
			container_.unique();
		}
		template<typename BinaryPredT>
		void unique(BinaryPredT pred)
		{
			container_.unique(Dereference<StoredIteratorType, BinaryPredT>(pred));
		}
		template<typename BinaryPredT>
		void v_unique(BinaryPredT pred)
		{
			container_.unique(pred);
		}

		void merge(SmartView &rhs)
		{
			container_.merge(rhs.container_, Dereference<StoredIteratorType, std::less<value_type>>());
		}
		void v_merge(SmartView &rhs)
		{
			container_.merge(rhs.container_);
		}
		template<typename CompareT>
		void merge(SmartView &rhs, CompareT comp)
		{
			container_.merge(rhs.container_, Dereference<StoredIteratorType, CompareT>(comp));
		}
		template<typename BinaryPredT>
		void v_merge(SmartView &rhs, BinaryPredT comp)
		{
			container_.merge(rhs.container_, comp);
		}

		void sort(SmartView &rhs)
		{
			container_.sort(rhs.container_, Dereference<StoredIteratorType, std::less<value_type>>());
		}
		void v_sort(SmartView &rhs)
		{
			container_.sort(rhs.container_);
		}
		template<typename CompareT>
		void sort(SmartView &rhs, CompareT comp)
		{
			container_.sort(rhs.container_, Dereference<StoredIteratorType, CompareT>(comp));
		}
		template<typename BinaryPredT>
		void v_sort(SmartView &rhs, BinaryPredT comp)
		{
			container_.sort(rhs.container_, comp);
		}

		void reverse()
		{
			container_.reverse();
		}
	};

	template<typename StoredIteratorT, typename ContainerT>
	inline bool operator==(const SmartView<StoredIteratorT, ContainerT> &lhs, const SmartView<StoredIteratorT, ContainerT> &rhs)
	{
		return lhs.size() == rhs.size() && std::equal(lhs.begin(), rhs.end(), rhs.begin()); 
	}
	template<typename StoredIteratorT, typename ContainerT>
	inline bool operator!=(const SmartView<StoredIteratorT, ContainerT> &lhs, const SmartView<StoredIteratorT, ContainerT> &rhs)
	{
		return !(lhs == rhs);
	}

	template<typename StoredIteratorT, typename ContainerT>
	inline bool operator<(const SmartView<StoredIteratorT, ContainerT> &lhs, const SmartView<StoredIteratorT, ContainerT> &rhs)
	{
		return std::lexicographical_compare(lhs.begin(), rhs.end(), rhs.begin(), rhs.end()); 
	}
	template<typename StoredIteratorT, typename ContainerT>
	inline bool operator>(const SmartView<StoredIteratorT, ContainerT> &lhs, const SmartView<StoredIteratorT, ContainerT> &rhs)
	{
		return rhs < lhs; 
	}
	template<typename StoredIteratorT, typename ContainerT>
	inline bool operator<=(const SmartView<StoredIteratorT, ContainerT> &lhs, const SmartView<StoredIteratorT, ContainerT> &rhs)
	{
		return !(rhs < lhs);
	}
	template<typename StoredIteratorT, typename ContainerT>
	inline bool operator>=(const SmartView<StoredIteratorT, ContainerT> &lhs, const SmartView<StoredIteratorT, ContainerT> &rhs)
	{
		return !(lhs < rhs); 
	}

	template<typename StoredIteratorT, typename ContainerT>
	inline bool swap(const SmartView<StoredIteratorT, ContainerT> &lhs, const SmartView<StoredIteratorT, ContainerT> &rhs)
	{
		return lhs.swap(rhs); 
	}
}





#endif