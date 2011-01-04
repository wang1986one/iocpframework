#ifndef __MEMORY_POOLS_HPP
#define __MEMORY_POOLS_HPP


#include "Pool.hpp"
#include "RegionAlloc.hpp"



namespace DataStructure
{
	// -------------------------------------------------------------------------
	// class PoolsAlloc

	template<typename PolicyT, int m_nPreAlloc = 0>
	class PoolsAlloc
	{
	public:
		// type definitions
		typedef typename PolicyT::value_type			value_type;
		typedef typename PolicyT::pointer			pointer;
		typedef typename PolicyT::const_pointer		const_pointer;
		typedef typename PolicyT::reference			reference;
		typedef typename PolicyT::const_reference	const_reference;
		typedef typename PolicyT::size_type			size_type;
		typedef typename PolicyT::difference_type	difference_type;

		
		typedef PoolsAlloc<PolicyT, m_nPreAlloc>	ClassType;
		typedef PoolAlloc<PolicyT>					PoolType;
		typedef RegionAlloc<PolicyT, m_nPreAlloc>	RegionAllocType;
		typedef typename PoolType::AllocType		BlockPoolType;


	private:
		// 8, 16, 24, 32, 40, ..., 128
		enum { NPOOL1 = 16 };
		enum { ALIGN_BITS1 = 3 };
		enum { ALIGN1 = 1 << ALIGN_BITS1 };
		enum { MIN_BYTES1 = ALIGN1 };
		enum { MAX_BYTES1 = ALIGN1 * NPOOL1 };

		// 2^8-16, 2^9-16, 2^10-16, 2^11-16, 2^12-16, 2^13-16, 2^14-16, 2^15-16
		enum { PADDING2 = 16 };
		enum { MIN_BITS2 = 8 };
		enum { MAX_BITS2 = 15 };
		enum { NPOOL2 = MAX_BITS2 - MIN_BITS2 + 1 };
		enum { MIN_BYTES2 = (1 << MIN_BITS2) - PADDING2 };
		enum { MAX_BYTES2 = (1 << MAX_BITS2) - PADDING2 };

		enum { NPOOL = NPOOL1 + NPOOL2 };

	public:
		enum { MAX_BYTES = MAX_BYTES2 + 1 };

	private:
		PoolType *m_pools[NPOOL];
		BlockPoolType m_recycle;
		RegionAllocType m_alloc;

	public:
		PoolsAlloc()
		{
			::memset(m_pools, 0, sizeof(PoolType *) * NPOOL);
		}

		PoolsAlloc(const BlockPoolType &recycle)
			: m_recycle(recycle)
			, m_alloc(recycle)
		{
			::memset(m_pools, 0, sizeof(PoolType *) * NPOOL);
		}

	private:
		PoolsAlloc(const PoolsAlloc&);
		const PoolsAlloc& operator=(const PoolsAlloc&);

	public:
		BlockPoolType &GetBlockPool() const
		{
			return m_recycle;
		}

		RegionAllocType &GetRegionAlloc() const 
		{
			return m_alloc;
		}

		PoolType &GetPool(size_type cb)
		{
			assert(HasPool(cb));

			if( cb - 1 < static_cast<size_type>(MAX_BYTES1) )
				return _GetPool1(cb);
			else
				return _GetPool2(cb);
		}

		bool HasPool(size_type cb) const
		{
			return cb - 1 < static_cast<size_type>(MAX_BYTES2);
		}

		void *Allocate(size_type cb)
		{
			typedef typename StdlibAlloc::AllocType SysAlloc;

			if( cb - 1 < static_cast<size_type>(MAX_BYTES1) )
				return _GetPool1(cb).Allocate();
			else if( cb - 1 < static_cast<size_type>(MAX_BYTES2) )
				return _GetPool2(cb).Allocate();
			else
				return SysAlloc::Allocate(cb);
		}

		void Deallocate(void* p, size_type cb)
		{
			typedef typename StdlibAlloc::AllocType SysAlloc;

			if( cb - 1 < static_cast<size_type>(MAX_BYTES1) )
				_GetPool1(cb).Deallocate(p);
			else if( cb - 1 < static_cast<size_type>(MAX_BYTES2) )
				_GetPool2(cb).Deallocate(p);
			else
				SysAlloc::Deallocate(p, cb);
		}

	private:
		PoolType &_GetPool2(size_type cb)
		{
			assert(!_HasPool1(cb));
			assert(HasPool(cb));

			/*
				BSR从高位开始扫描源操作数，若所有位都是0，则ZF=0，否则ZF=1。
				并且将第一个出现1的位号存入目的操作数。
	
				逆向扫描这个 val，看哪个位最先被置了 1，将 zf 置 1，并将这个位数存到 eax 中。
			*/
			const size_type index = Internal::log2(cb + (PADDING2 - 1)) + (NPOOL1 - (MIN_BITS2 - 1));
			PoolType* p = m_pools[index];

			if( p == NULL )
			{
				const size_type cbElem = (1 << (index - (NPOOL1 - MIN_BITS2))) - PADDING2;
				m_pools[index] = p = New<PoolType>(m_alloc, m_recycle, cbElem);
			}

			return *p;
		}

		PoolType &_GetPool1(size_type cb)
		{
			assert(_HasPool1(cb));

			const size_type index = (cb - 1) >> ALIGN_BITS1;
			PoolType* p = m_pools[index];

			if( p == NULL )
			{
				const size_type cbElem = (index + 1) << ALIGN_BITS1;
				m_pools[index] = p = New<PoolType>(m_alloc,m_recycle, cbElem);
			}

			return *p;
		}

		bool _HasPool1(size_type cb) const
		{
			return cb - 1 < static_cast<size_type>(MAX_BYTES1);
		}
	};


	// -------------------------------------------------------------------------
	typedef PoolsAlloc<Config::Stdlib> Pools;

	// -------------------------------------------------------------------------
	typedef PoolsAlloc<Config::Scoped> ScopedPools;
}

#endif