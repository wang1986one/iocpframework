#ifndef __MEMORY_BLOCK_POOL_HPP
#define __MEMORY_BLOCK_POOL_HPP


#include "Config.hpp"


namespace DataStructure
{
	
	/*
		Allocator��Ҫ��֧�ֿ�������

		PolicyT--AllocType�����õĲ���,�����ṩMemBlock�����ڴ�飩�ķ���


		PolicyT::AllocType��Ҫ��֧�ֿ������죨һ��������Allocator����������һ�㣩��
		���ԣ������Allocatorͨ��ֻ����������
		1) ȫ���ڴ�Allocator(StdlibAlloc)
		2) Allocator�Ĵ���(ProxyAlloc)


		MemBlockSize--MemBlock��С������PolicyT::AllocType���ܵ������Ҫ����

	*/


	/*
		ʹ�ó�����
			��������һ���㷨�ĵ�i����Ƚϸ��ӣ����ڴ濪��Ҳ��Ϊ�ɹۣ�ϣ��Ϊ����i����һ��˽�д洢��Private GC Allocator����
			�Ա���Щ����i�ڲ������õ���ʱ�ڴ��ڸò������ʱ�ͷ�


	*/
	



	// -------------------------------------------------------------------------
	// class BlockAlloc  �ڴ��

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

		// m_nFreeLimit��������Ϊ��������m_pFreeList�е��ڴ��������Ӷ�������������ķ�����
		// ��BlockPool�е�m_pFreeList�ڴ�ﵽm_nFreeʱ��deallocate����ֱ���ͷ�MemBlock
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


		// �������ú���Ӧ��ȷ����Alloctor����������ж��󶼲�����Ҫ�������µ��á�
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