#ifndef __STD_ALLOCATOR_HPP
#define __STD_ALLOCATOR_HPP


// BUG!!

#ifdef max
#undef max
#endif

namespace DataStructure
{
	
	//-----------------------------------------------------------
	// class ObjectTraitsT

	template<typename T>
	class ObjectTraitsT
	{
	public:
		// convert an ObjectTraits<T> to ObjectTraits<U>
		template<typename U>
		struct Rebind 
		{
			typedef ObjectTraitsT<U> Other;
		};

	public:
		ObjectTraitsT()
		{}
		explicit ObjectTraitsT(const ObjectTraitsT &)
		{}
		template<typename U>
		explicit ObjectTraitsT(const ObjectTraitsT<U> &)
		{}
		~ObjectTraitsT()
		{}

	public:
		// Address 
		T *Address(T &val)
		{
			return &val;
		}
		const T* Address(const T &val) const
		{
			return &val;
		}

		 // Inlitial & Unintilial
		static void Construct(T *p)
		{
			new(p) T();
		}
		template<typename ArgT>
		static void Construct(T *p, const ArgT &arg)
		{
			new(p) T(arg);
		}

		static void Destroy(T *p)
		{
			p->~T();
		}
	};
	

	
	//--------------------------------------------------------------
	// StdAllocPolicyT

	template<typename T>
	class StdAllocPolicyT
	{
	public:
		// type definitions
		typedef T				value_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;

	public:
		// rebind allocator to type U
		template<typename U>
		struct Rebind 
		{
			typedef Allocator<U> Other;
		};

	public:
		StdAllocPolicyT()
		{}
		explicit StdAllocPolicyT(const StdAllocPolicyT &)
		{}
		template<typename U>
		explicit StdAllocPolicyT(const StdAllocPolicyT<U> &)
		{}
		~StdAllocPolicyT()
		{}

	public:
		// 内存申请释放
		pointer Allocate(size_type size, const void* = 0)
		{
			return reinterpret_cast<pointer>(::operator new(size * sizeof(value_type)));
		}
		void Deallocate(pointer p, size_type)
		{
			::operator delete(p);
		}

		// 大小
		size_type MaxSize() const
		{
			return std::numeric_limits<size_type>::max() / sizeof(value_type);
		}
	};

	//	确定不同内存分配器的分配与释放内存一致
	template<typename T, typename T2>
	inline bool operator==(StdAllocPolicyT<T> const&, StdAllocPolicyT<T2> const&) 
	{ 
		return false;
	}
	template<typename T, typename OtherAllocator>
	inline bool operator==(StdAllocPolicyT<T> const&, OtherAllocator const&) 
	{ 
		return false; 
	}



	//--------------------------------------------------------------
	// ArrayAllocPolicyT

	template<typename T, size_t Num>
	class ArrayAllocPolicyT
	{
	public:
		// type definitions
		typedef T				value_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;

	public:
		// rebind allocator to type U
		template<typename U>
		struct Rebind 
		{
			typedef ArrayAllocPolicyT<U, Num> Other;
		};

	private:
		T m_data[Num];

	public:
		ArrayAllocPolicyT()
		{}
		explicit ArrayAllocPolicyT(const ArrayAllocPolicyT &)
		{}
		template<typename U, size_t Num2>
		explicit ArrayAllocPolicyT(const ArrayAllocPolicyT<U, Num2> &)
		{}
		~ArrayAllocPolicyT()
		{}

	public:
		// 内存申请释放
		pointer Allocate(size_type size, const void* = 0)
		{
			return reinterpret_cast<pointer>(m_data);
		}
		void Deallocate(pointer p, size_type)
		{
		}

		// 大小
		size_type MaxSize() const
		{
			return Num;
		}
	};

	//	确定不同内存分配器的分配与释放内存一致
	template<typename T, size_t N1, typename T2, size_t N2>
	inline bool operator==(ArrayAllocPolicyT<T, N1> const&, ArrayAllocPolicyT<T2, N2> const&) 
	{ 
		return false;
	}
	template<typename T, size_t N1, typename OtherAllocator>
	inline bool operator==(ArrayAllocPolicyT<T, N1> const&, OtherAllocator const&) 
	{ 
		return false; 
	}


	//--------------------------------------------------------------
	// class OtherAllocPolicyT
	



	//--------------------------------------------------------------
	// class TrackAllocPolicy

	template< typename T, typename PolicyT = StdAllocPolicyT<T> >
	class TrackAllocPolicyT
		: public PolicyT
	{
	public:
		typedef PolicyT									AllocatePolicyType;
		typedef typename AllocatePolicyType::size_type	size_type;
		typedef typename AllocatePolicyType::pointer	pointer;
		typedef typename AllocatePolicyType::const_pointer const_pointer;

	public:
		template<typename U>
		struct Rebind
		{
			typedef TrackAllocPolicyT<U, typename AllocatePolicyType::Rebind<U>::Other> Other;
		};


	private:
		size_type	m_nTotal;
		size_type	m_nCurrent;
		size_type	m_nPeak;

	public:
		TrackAllocPolicyT()
			: m_nTotal(0)
			, m_nCurrent(0)
			, m_nPeak(0)
		{}
		TrackAllocPolicyT(const TrackAllocPolicyT &rhs)
			: AllocatePolicyType(rhs)
			, m_nTotal(rhs.m_nTotal)
			, m_nCurrent(rhs.m_nCurrent)
			, m_nPeak(rhs.m_nPeak)
		{}
		template<typename U>
		explicit TrackAllocPolicyT(const TrackAllocPolicyT<U> &rhs)
			: AllocatePolicyType(rhs)
			, m_nTotal(0)
			, m_nCurrent(0)
			, m_nPeak(0)
		{}
		~TrackAllocPolicyT()
		{}


	public:
		pointer Allocate(size_type size, const void*ptr = 0)
		{
			pointer p = AllocatePolicyType::Allocate(size, ptr);

			m_nTotal += size;
			m_nCurrent += size;

			if( m_nCurrent > m_nPeak )
			{
				m_nPeak = m_nCurrent;
			}

			return p;
		}

		void Deallocate(pointer p, size_type size)
		{
			AllocatePolicyType::Deallocate(p, size);
			m_nCurrent -= size;
		}


	public:
		size_type GetTotalAllocations() const
		{
			return m_nTotal;
		}
		size_type GetCurrentAllocation() const
		{
			return m_nCurrent;
		}
		size_type GetPeakAllocation() const
		{
			return m_nPeak;
		}
	};

	//	确定不同内存分配器的分配与释放内存一致
	template<typename T, typename PolicyT1, typename U, typename PolicyT2>
	inline bool operator==(const TrackAllocPolicyT<T, PolicyT1> &lhs, const TrackAllocPolicyT<U, PolicyT2> &rhs) 
	{ 
		return operator==(static_cast<PolicyT1&>(lhs), static_cast<PolicyT1 &>(rhs));
	}
	template<typename T, typename PolicyT1, typename OtherAllocator>
	inline bool operator==(const TrackAllocPolicyT<T, PolicyT1> &lhs, const OtherAllocator &rhs) 
	{ 
		return operator==(static_cast<PolicyT1&>(lhs), rhs);
	}



	//--------------------------------------------------------------
	// class ExtAllocator

	template< typename T, typename PolicyT = StdAllocPolicyT<T>, typename TraitsT = ObjectTraitsT<T> >
	class ExtAllocator
		: public PolicyT
		, public TraitsT
	{
	private:
		typedef PolicyT AllocatePolicyType;
		typedef TraitsT	AllocateTraitsType;

	public:
		typedef typename AllocatePolicyType::value_type				value_type;
		typedef typename AllocatePolicyType::pointer				pointer;
		typedef typename AllocatePolicyType::const_pointer			const_pointer;
		typedef typename AllocatePolicyType::reference				reference;
		typedef typename AllocatePolicyType::const_reference			const_reference;
		typedef typename AllocatePolicyType::size_type				size_type;
		typedef typename AllocatePolicyType::difference_type			difference_type;
	
	
	public:
		template<typename U>
		struct Rebind
		{
			typedef ExtAllocator<U, typename AllocatePolicyType::Rebind<U>::Other> Other;
		};

	public:
		ExtAllocator()
		{}
		ExtAllocator(const ExtAllocator &rhs)
			: AllocatePolicyType(rhs)
			, AllocateTraitsType(rhs)
		{}
		template<typename U, typename P, typename T>
		ExtAllocator(const ExtAllocator<U, P, T> &rhs)
			: AllocatePolicyType(rhs)
			, AllocateTraitsType(rhs)
		{}
		~ExtAllocator()
		{}

	public:
		pointer Allocate(size_type size, const void *p = 0)
		{
			return AllocatePolicyType::Allocate(size, p);
		}
		void Deallocate(pointer p, size_type size)
		{
			return AllocatePolicyType::Deallocate(p, size);
		}
	};


	template<typename T, typename P, typename Tr>
	inline bool operator==(ExtAllocator<T, P, Tr> const& lhs, ExtAllocator<T, P, Tr> const& rhs)
	{ 
		return operator==(static_cast<P&>(lhs), static_cast<P&>(rhs)); 
	}
	template<typename T, typename P, typename Tr, typename T2, typename P2, typename Tr2>
	inline bool operator==(ExtAllocator<T, P, Tr> const& lhs, ExtAllocator<T2, P2, Tr2> const& rhs) 
	{ 
		return operator==(static_cast<P&>(lhs), static_cast<P2&>(rhs)); 
	}
	template<typename T, typename P, typename Tr, typename OtherAllocator>
	inline bool operator==(ExtAllocator<T, P, Tr> const& lhs, OtherAllocator const& rhs) 
	{ 
		return operator==(static_cast<P&>(lhs), rhs); 
	}
	template<typename T, typename P, typename Tr>
	inline bool operator!=(ExtAllocator<T, P, Tr> const& lhs, ExtAllocator<T, P, Tr> const& rhs)
	{ 
		return !operator==(lhs, rhs); 
	}
	template<typename T, typename P, typename Tr, typename T2, typename P2, typename Tr2>
	inline bool operator!=(ExtAllocator<T, P, Tr> const& lhs, ExtAllocator<T2, P2, Tr2> const& rhs) 
	{ 
		return !operator==(lhs, rhs); 
	}
	template<typename T, typename P, typename Tr, typename OtherAllocator>
	inline bool operator!=(ExtAllocator<T, P, Tr> const& lhs, OtherAllocator const& rhs) 
	{ 
		return !operator==(lhs, rhs); 
	}


}







#endif