#ifndef __MEMORY_BLOCK_POOL_HPP
#define __MEMORY_BLOCK_POOL_HPP


#include "Config.hpp"


namespace DataStructure
{
	
	/*
		Allocator被要求支持拷贝构造

		PolicyT--AllocType所采用的策略,负责提供MemBlock（大内存块）的分配


		PolicyT::AllocType被要求支持拷贝构造（一般真正的Allocator很难做到这一点）。
		所以，这里的Allocator通常只有两个可能
		1) 全局内存Allocator(StdlibAlloc)
		2) Allocator的代理(ProxyAlloc)


		MemBlockSize--MemBlock大小，它是PolicyT::AllocType性能调配的重要参数

	*/


	/*
		使用场景：
			我们设想一个算法的第i步骤比较复杂，其内存开销也颇为可观，希望为步骤i引入一个私有存储（Private GC Allocator），
			以便哪些步骤i内部计算用的临时内存在该步骤结束时释放


	*/
	



	// -------------------------------------------------------------------------
	// class BlockAlloc  内存池

	template<typename PolicyT>
	class BlockAlloc
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

		typedef typename PolicyT::AllocType			AllocT;
		typedef typename PolicyT::AllocType			AllocType;
		typedef BlockAlloc<PolicyT>					ClassType;

		enum { m_cbBlock = PolicyT::MemBlockBytes - AllocT::Padding };

	private:
		struct Block 
		{
			Block *pNext;
		};
		Block* m_pFreeList;

		// m_nFreeLimit的引入是为了限制了m_pFreeList中的内存总量，从而抑制这种情况的发生。
		// 在BlockPool中的m_pFreeList内存达到m_nFree时，deallocate操作直接释放MemBlock
		int m_nFree;		
		const int m_nFreeLimit;

	public:
		enum { Padding = AllocT::Padding };
		enum { BlockSize = m_cbBlock };


	public:
		BlockAlloc(int cbFreeLimit = INT_MAX)
			: m_pFreeList(NULL)
			, m_nFree(0)
			, m_nFreeLimit(cbFreeLimit / m_cbBlock)
		{
		}
		~BlockAlloc()
		{
			clear();
		}

	private:
		BlockAlloc(const BlockAlloc &);
		BlockAlloc &operator=(const BlockAlloc &);

	public:
		void *Allocate(size_type cb)
		{
			assert(cb >= m_cbBlock);

			if( cb > static_cast<size_type>(m_cbBlock) )
				return AllocT::Allocate(cb);
			else
			{
				if( m_pFreeList != NULL )
				{
					assert(AllocT::AllocSize(m_pFreeList) >= cb);

					Block* blk = m_pFreeList;
					m_pFreeList = blk->pNext;
					--m_nFree;

					return blk;
				}

				return AllocT::Allocate(m_cbBlock);
			}
		}

		void Deallocate(void* p, size_t nSize = 0)
		{
			if (m_nFree >= m_nFreeLimit) 
			{
				AllocT::Deallocate(p);
			}
			else
			{ 
				Block *blk	= reinterpret_cast<Block *>(p);
				blk->pNext	= m_pFreeList;
				m_pFreeList = blk;
				++m_nFree;
			}
		}

		size_type AllocSize(void* p)
		{
			return AllocT::AllocSize(p);
		}


		// 谨慎！该函数应在确保该Alloctor所分配的所有对象都不再需要的情形下调用。
		void clear()
		{
			while( m_pFreeList != NULL )
			{
				Block* blk = m_pFreeList;
				m_pFreeList = blk->pNext;

				AllocT::Deallocate(blk);
			}

			m_nFree = 0;
		}
	};

}
#endif