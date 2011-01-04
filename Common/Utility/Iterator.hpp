#ifndef __ITERATOR_HPP
#define __ITERATOR_HPP



namespace DataStructure
{
	// iterator

	struct InputIteratorTag {};
	struct OutputIteratorTag {};
	struct ForwardIteratorTag : public InputIteratorTag {};
	struct BidirectionalIteratorTag : public ForwardIteratorTag {};
	struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};


	template<typename T, typename DistanceT> 
	struct InputIterator 
	{
		typedef InputIteratorTag	iterator_category;
		typedef T					value_type;
		typedef DistanceT			difference_type;
		typedef T*					pointer;
		typedef T&					reference;
	};

	struct OutputIterator 
	{
		typedef OutputIteratorTag	iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;
	};

	template<typename T, typename DistanceT> 
	struct ForwardIterator 
	{
		typedef ForwardIteratorTag	iterator_category;
		typedef T                   value_type;
		typedef DistanceT           difference_type;
		typedef T*                  pointer;
		typedef T&                  reference;
	};


	template<typename T, typename DistanceT> 
	struct BidirectionalIterator 
	{
		typedef BidirectionalIteratorTag	iterator_category;
		typedef T							value_type;
		typedef DistanceT					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template<typename T, typename DistanceT> 
	struct RandomAccessIterator 
	{
		typedef RandomAccessIteratorTag		iterator_category;
		typedef T							value_type;
		typedef DistanceT					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};



	template<typename CategoryT, typename T, typename DistanceT = ptrdiff_t, typename PointerT = T*, typename ReferenceT = T&>
	struct Iterator 
	{
		typedef CategoryT  iterator_category;
		typedef T          value_type;
		typedef DistanceT  difference_type;
		typedef PointerT   pointer;
		typedef ReferenceT reference;
	};


	template<typename iterator>
	struct IteratorTraits 
	{
		typedef typename iterator::iterator_category		iterator_category;
		typedef typename iterator::value_type			value_type;
		typedef typename iterator::difference_type		difference_type;
		typedef typename iterator::pointer				pointer;
		typedef typename iterator::reference			reference;
	};

	template<typename T>
	struct IteratorTraits<T *> 
	{
		typedef RandomAccessIteratorTag		iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template<typename T>
	struct IteratorTraits<const T *> 
	{
		typedef RandomAccessIteratorTag		iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef const T*					pointer;
		typedef const T&					reference;
	};



	// iterator Helper Function
	template<typename IteratorT>
	inline typename IteratorTraits<IteratorT>::iterator_category IteratorCategory(const IteratorT&) 
	{
		typedef typename IteratorTraits<IteratorT>::iterator_category category;
		return category();
	}

	template<typename IteratorT>
	inline typename IteratorTraits<IteratorT>::difference_type* DistanceType(const IteratorT&) 
	{
		return static_cast<typename IteratorTraits<iterator>::difference_type*>(0);
	}

	template<typename IteratorT>
	inline typename IteratorTraits<IteratorT>::value_type* ValueType(const IteratorT&) 
	{
		return static_cast<typename IteratorTraits<iterator>::value_type*>(0);
	}

	template<typename InputIteratorT, typename DistanceT>
	inline void __Distance(InputIteratorT first, InputIteratorT last, DistanceT& n, InputIteratorTag) 
	{
		while(first != last) 
		{ 
			++first; 
			++n; 
		}
	}

	template<typename RandomAccessIteratorT, typename DistanceT>
	inline void __Distance(RandomAccessIteratorT first, RandomAccessIteratorT last, DistanceT& n, RandomAccessIteratorTag)
	{
		n += last - first;
	}

	template<typename InputIteratorT, typename DistanceT>
	inline void DistanceFunc(InputIteratorT first, InputIteratorT last, DistanceT& n)
	{
		__Distance(first, last, n, iterator_category(first));
	}

	template<typename InputIteratorT>
	inline typename IteratorTraits<InputIteratorT>::difference_type __Distance(InputIteratorT first, InputIteratorT last, InputIteratorTag)
	{
		IteratorTraits<InputIteratorT>::difference_type n = 0;
			
		while(first != last) 
		{
			++first; 
			++n;
		}
			
		return n;
	}

	template<typename RandomAccessIteratorT>
	inline typename IteratorTraits<RandomAccessIteratorT>::difference_type __Distance(RandomAccessIteratorT first, RandomAccessIteratorT last, RandomAccessIteratorTag) 
	{
		return last - first;
	}

	template<typename InputIteratorT>
	inline typename IteratorTraits<InputIteratorT>::difference_type DistanceFunc(InputIteratorT first, InputIteratorT last) 
	{
		typedef typename IteratorTraits<InputIteratorT>::iterator_category category;
		
		return __Distance(first, last, category());
	}


	template<typename BidirectionalIteratorT, typename DistanceT>
	inline void __Advance(BidirectionalIteratorT& i, DistanceT n, BidirectionalIteratorTag) 
	{
		if( n >= 0 )
			while(n--) ++i;
		else
			while(n++) --i;
	}

	template<typename RandomAccessIteratorT, typename DistanceT>
	inline void __Advance(RandomAccessIteratorT& i, DistanceT n, RandomAccessIteratorTag) 
	{
			i += n;
	}

	template<typename InputIteratorT, typename DistanceT>
	inline void Advance(InputIteratorT& i, DistanceT n) 
	{
		__Advance(i, n, IteratorCategory(i));
	}



	template<typename ContainerT>
	class BackInsertIterator
	{
	protected:
		ContainerT* container;

	public:
		typedef OutputIteratorTag	iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;

	public:
		explicit BackInsertIterator(ContainerT& x) 
			: container(&x) 
		{}
		BackInsertIterator<ContainerT>& operator=(const typename ContainerT::value_type& value) 
		{ 
			container->push_back(value);
				
			return *this;
		}

		BackInsertIterator<ContainerT>& operator*()		{ return *this; }
		BackInsertIterator<ContainerT>& operator++()		{ return *this; }
		BackInsertIterator<ContainerT>& operator++(int)	{ return *this; }
	};


	template<typename ContainerT>
	inline BackInsertIterator<ContainerT> BackInserter(ContainerT& x)
	{
		return BackInsertIterator<ContainerT>(x);
	}


	//---------------------------------------------
	// class FrontInsertIterator

	template<typename ContainerT>
	class FrontInsertIterator 
	{
	protected:
		ContainerT* container;

	public:
		typedef OutputIteratorTag	iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;

	public:
		explicit FrontInsertIterator(ContainerT& x) 
			: container(&x) {}
		FrontInsertIterator<ContainerT>& operator=(const typename ContainerT::value_type& value) 
		{ 
			container->PushFront(value);
				
			return *this;
		}
		FrontInsertIterator<ContainerT>& operator*()		{ return *this; }
		FrontInsertIterator<ContainerT>& operator++()	{ return *this; }
		FrontInsertIterator<ContainerT>& operator++(int) { return *this; }
	};


	template<typename ContainerT>
	inline FrontInsertIterator<ContainerT> FrontInserter(ContainerT& x) 
	{
		return FrontInsertIterator<ContainerT>(x);
	}


	//------------------------------------------------------------
	// class InsertIterator

	template<typename ContainerT>
	class InsertIterator
	{
	protected:
		ContainerT* container;
		typename ContainerT::iterator iter;

	public:
		typedef OutputIteratorTag	iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;

	public:
		InsertIterator(ContainerT& x, typename ContainerT::iterator i) 
			: container(&x), iter(i) {}
		InsertIterator<ContainerT>& operator=(const typename ContainerT::value_type& value) 
		{ 
			iter = container->insert(iter, value);
			++iter;

			return *this;
		}
		InsertIterator<ContainerT>& operator*()		{ return *this; }
		InsertIterator<ContainerT>& operator++()		{ return *this; }
		InsertIterator<ContainerT>& operator++(int)	{ return *this; }
	};


	template<typename ContainerT, typename IteratorT>
	inline InsertIterator<ContainerT> Inserter(ContainerT& x, IteratorT i) 
	{
		typedef typename ContainerT::iterator iter;

		return InsertIterator<ContainerT>(x, iter(i));
	}


	//------------------------------------------------------------
	// class ReverseBidirectionalIterator

	template<typename BidirectionalIteratorT, typename T, typename ReferenceT, typename DistanceT> 
	class ReverseBidirectionalIterator 
	{
		typedef ReverseBidirectionalIterator<BidirectionalIteratorT, T, ReferenceT, DistanceT> Self;

	protected:
		BidirectionalIteratorT current;

	public:
		typedef BidirectionalIteratorTag	iterator_category;
		typedef T							value_type;
		typedef DistanceT					difference_type;
		typedef T*							pointer;
		typedef ReferenceT					reference;

	public:
		ReverseBidirectionalIterator() 
		{}
		explicit ReverseBidirectionalIterator(BidirectionalIteratorT x)
			: current(x)
		{}
		BidirectionalIteratorT Base() const 
		{ return current; }

		reference operator*() const
		{
			BidirectionalIterator tmp = current;
			return *--tmp;
		}


		pointer operator->() const 
		{ 
			return &(operator*());
		}

		Self& operator++() 
		{
			--current;

			return *this;
		}
		Self operator++(int) 
		{
			Self tmp = *this;
			--current;

			return tmp;
		}
		Self& operator--() 
		{
			++current;

			return *this;
		}
		Self operator--(int) 
		{
			Self tmp = *this;
			++current;

			return tmp;
		}
	};


	template<typename BidirectionalIteratorT, typename T, typename ReferenceT, typename DistanceT>
	inline BidirectionalIteratorTag IteratorCategory(const ReverseBidirectionalIterator<BidirectionalIteratorT, T, ReferenceT, DistanceT>&) 
	{
		return BidirectionalIteratorTag();
	}

	template<typename BidirectionalIteratorT, typename T, typename ReferenceT, typename DistanceT>
	inline T* ValueType(const ReverseBidirectionalIterator<BidirectionalIteratorT, T, ReferenceT, DistanceT>&) 
	{
		return (T*) 0;
	}

	template<typename BidirectionalIteratorT, typename T, typename ReferenceT, typename DistanceT>
	inline DistanceT* DistanceType(const ReverseBidirectionalIterator<BidirectionalIteratorT, T, ReferenceT, DistanceT>&)
	{
		return reinterpret_cast<DistanceT *>(0);
	}


	template<typename BidirectionalIteratorT, typename T, typename ReferenceT, typename DistanceT>
	inline bool operator==(const ReverseBidirectionalIterator<BidirectionalIteratorT, T, ReferenceT, DistanceT>& x, 
		const ReverseBidirectionalIterator<BidirectionalIteratorT, T, ReferenceT,
		DistanceT>& y) 
	{
			return x.Base() == y.Base();
	}


	//----------------------------------------------------------------
	// class ReverseIterator

	template<typename IteratorT>
	class ReverseIterator 
	{
	protected:
		IteratorT current;

	public:
		typedef typename IteratorTraits<IteratorT>::iterator_category	iterator_category;
		typedef typename IteratorTraits<IteratorT>::value_type			value_type;
		typedef typename IteratorTraits<IteratorT>::difference_type		difference_type;
		typedef typename IteratorTraits<IteratorT>::pointer				pointer;
		typedef typename IteratorTraits<IteratorT>::reference			reference;
		typedef IteratorT												iterator_type;
		typedef ReverseIterator<IteratorT>								Self;

	public:
		ReverseIterator() 
		{}
		explicit ReverseIterator(iterator_type x)
			: current(x) 
		{}

		ReverseIterator(const Self& x) 
			: current(x.current) 
		{}

		template<typename Iter>
		ReverseIterator(const ReverseIterator<Iter>& x) 
			: current(x.current) 
		{}

	public:
		iterator_type Base() const 
		{ 
			return current; 
		}

		reference operator*() const 
		{
			iterator tmp = current;
			return *--tmp;
		}


		pointer operator->() const 
		{ 
			return &(operator*()); 
		}


		Self& operator++() 
		{
			--current;
			return *this;
		}
		Self operator++(int) 
		{
			Self tmp = *this;
			--current;
			return tmp;
		}
		Self& operator--() 
		{
			++current;
			return *this;
		}
		Self operator--(int)
		{
			Self tmp = *this;
			++current;
			return tmp;
		}
		Self operator+(difference_type n) const 
		{
			return Self(current - n);
		}
		Self& operator+=(difference_type n) 
		{
			current -= n;
			return *this;
		}
		Self operator-(difference_type n) const
		{
			return Self(current + n);
		}
		Self& operator-=(difference_type n) 
		{
			current += n;
			return *this;
		}

		reference operator[](difference_type n) const 
		{ 
			return *(*this + n); 
		}  
	}; 


	template<typename IteratorT>
	inline bool operator==(const ReverseIterator<IteratorT>& x, const ReverseIterator<IteratorT>& y) 
	{
		return x.Base() == y.Base();
	}

	template<typename IteratorT>
	inline bool operator!=(const ReverseIterator<IteratorT>& x, const ReverseIterator<IteratorT>& y) 
	{
		return !(x.Base() == y.Base());
	}

	template<typename IteratorT>
	inline bool operator<(const ReverseIterator<IteratorT>& x, const ReverseIterator<IteratorT>& y)
	{
		return y.Base() < x.Base();
	}

	template<typename IteratorT>
	inline typename ReverseIterator<IteratorT>::difference_type operator-(const ReverseIterator<IteratorT>& x, const ReverseIterator<IteratorT>& y) 
	{
		return y.Base() - x.Base();
	}

	template<typename IteratorT>
	inline ReverseIterator<IteratorT> operator+(typename ReverseIterator<IteratorT>::difference_type n, const ReverseIterator<IteratorT>& x) 
	{
		return ReverseIterator<IteratorT>(x.Base() - n);
	}


	//--------------------------------------------------------------
	// class IStreamIterator

	template<typename T, typename CharT = char, typename DistanceT = ptrdiff_t> 
	class IStreamIterator 
	{
		friend bool operator==(const IStreamIterator<T, CharT, DistanceT>& x, 
			const IStreamIterator<T, CharT, DistanceT>& y);
	
		typedef CharT						char_type;
		typedef const CharT					const_char_type;
		typedef std::basic_istream<CharT>	IStreamType;

	protected:
		IStreamType* stream;
		T value;
		bool end_marker;


	public:
		typedef InputIteratorTag	iterator_category;
		typedef T					value_type;
		typedef DistanceT			difference_type;
		typedef const T*			pointer;
		typedef const T&			reference;

	public:
		IStreamIterator() 
			: stream(&std::cin)
			, end_marker(false) 
		{}
		IStreamIterator(IStreamType& s) 
			: stream(&s) 
		{ Read(); }

	public:
		reference operator*() const 
		{ 
			return value; 
		}

		pointer operator->() const 
		{ 
			return &(operator*()); 
		}

		IStreamIterator<T, CharT, DistanceT>& operator++()
		{ 
			Read(); 

			return *this;
		}
		IStreamIterator<T, CharT, DistanceT> operator++(int) 
		{
			IStreamIterator<T, CharT, DistanceT> tmp = *this;
			Read();

			return tmp;
		}

	protected:
		void Read() 
		{
			end_marker = (*stream) ? true : false;

			if(end_marker) 
				*stream >> value;

			end_marker = (*stream) ? true : false;
		}
	};


	template<typename T, typename CharT, typename DistanceT>
	inline bool operator==(const IStreamIterator<T, CharT, DistanceT>& x, const IStreamIterator<T, CharT, DistanceT>& y) 
	{
		return x.stream == y.stream && x.end_marker == y.end_marker ||
			x.end_marker == false && y.end_marker == false;
	}


	//---------------------------------------------------------
	// class OStreamIterator

	template<typename T, typename CharT = char>
	class OStreamIterator 
	{
		typedef CharT						char_type;
		typedef const CharT					const_char_type;
		typedef std::basic_ostream<CharT>	OStreamType;
		
	protected:
		OStreamType* stream;
		const_char_type* string;

	public:
		typedef OutputIteratorTag	iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;

	public:
		OStreamIterator(OStreamType& s) 
			: stream(&s)
			, string(0) 
		{}
		OStreamIterator(OStreamType& s, const_char_type* c)
			: stream(&s)
			, string(c)  
		{}
		OStreamIterator<T, CharT>& operator=(const T& value) 
		{ 
			*stream << value;

			if (string) 
				*stream << string;

			return *this;
		}

		OStreamIterator<T, CharT>& operator*()		{ return *this; }
		OStreamIterator<T, CharT>& operator++()		{ return *this; } 
		OStreamIterator<T, CharT>& operator++(int)	{ return *this; } 
	};


}



#endif