#ifndef __MEMORY_REGION_ALLOC_HPP
#define __MEMORY_REGION_ALLOC_HPP


#include "MemoryBase.hpp"





/**************************************************************************
*	������: Ϊһ�����ӵĵľֲ�����(�㷨)�ṩ�Զ��ڴ����
*	
*	�ֲ�����: ָ�㷨�����Խϸߡ�������������ռʱ���бȽ϶��ݵĹ��̡�
*
*	ʹ�÷�ʽ: �Ӿֲ����̵�һ��ʼ��ֻ��ȥ���롢ʹ���ڴ棬֪���㷨��ɣ������������Ĵ󲿷��ڴ�(��Ҫ��Ϊ�㷨�������������)��
*	�������㷨���ĸ��������룬����������������������������Զ�����(ͨ��clear����)
*
***************************************************************************/


namespace DataStructure
{

	// =========================================================================
	// class RegionAlloc

	template<typename PolicyT, int nPreAlloc = 0>
	class RegionAlloc
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

		typedef typename PolicyT::AllocType			AllocType;
		typedef RegionAlloc<PolicyT, nPreAlloc>		ClassType;


	public:
		// 64 k - 16 bit
		enum { MemBlockSize = PolicyT::MemBlockBytes - AllocType::Padding };


	private:
		struct MemBlock;

		enum { HeaderSize = sizeof(void*) };				// 4 bit
		enum { BlockSize = MemBlockSize - HeaderSize };


		// ���õ��������������ڴ��
		struct MemBlock
		{
			MemBlock* pPrev;
			char buffer[BlockSize];
		};
		struct DestroyNode
		{
			DestroyNode* pPrev;
			DestructorT fnDestroy;
		};

		char* m_pBegin;
		char* m_pEnd;
		AllocType m_alloc;
		DestroyNode* m_pDestroyChain;

	public:
		RegionAlloc() 
			: m_pBegin(NULL)
			, m_pEnd(NULL)
			, m_pDestroyChain(NULL)
		{
			_Init();
		}
		explicit RegionAlloc(const AllocType &alloc) 
			: m_pBegin(NULL)
			, m_pEnd(NULL)
			, m_alloc(alloc)
			, m_pDestroyChain(NULL)
		{
			_Init();
		}

		~RegionAlloc()
		{
			clear();
		}

	private:
		RegionAlloc(const RegionAlloc&);
		RegionAlloc &operator=(const RegionAlloc&);

	public:
		const AllocType &GetAlloc() const
		{
			return m_alloc;
		}

		void swap(ClassType &o)
		{
			SwapObject(this, &o);
		}

		void *Allocate(size_type cb)
		{
			// ����1����ǰMemBlock�����㹻�������ڴ�(free memory)��
			if( static_cast<size_type>(m_pEnd - m_pBegin) >= cb )
			{
				return m_pEnd -= cb;
			}

			// ����2���ڵ�ǰ��_MemBlock�������ڴ棨free memory�����������£����Ǿ���Ҫ����һ���µ�MemBlock�Թ�ʹ�á�
			return _DoAllocate(cb);
		}
		void *Allocate(size_type cb, DestructorT fn)
		{
			DestroyNode *pNode = reinterpret_cast<DestroyNode *>(Allocate(sizeof(DestroyNode) + cb));

			pNode->fnDestroy	= fn;
			pNode->pPrev		= m_pDestroyChain;
			m_pDestroyChain		= pNode;

			return pNode + 1;
		}

		void *Reallocate(void* p, size_type oldSize, size_type newSize)
		{
			if( oldSize >= newSize )
				return p;

			void* p2 = Allocate(newSize);
			::memcpy(p2, p, oldSize);

			return p2;
		}

		// ͳһ�ͷ�������������ڴ�
		void clear()
		{
			while( m_pDestroyChain != NULL )
			{
				DestroyNode* curr = m_pDestroyChain;
				m_pDestroyChain = m_pDestroyChain->pPrev;
				curr->fnDestroy(curr + 1);
			}

			MemBlock* pHeader = _ChainHeader();
			while( pHeader != NULL )
			{
				MemBlock* curr = pHeader;
				pHeader = pHeader->pPrev;
				m_alloc.Deallocate(curr);
			}

			m_pBegin = m_pEnd = reinterpret_cast<char *>(HeaderSize);
		}

	private:
		// ��ȡChainHeaderͷָ��
		MemBlock *_ChainHeader() const
		{
			return reinterpret_cast<MemBlock *>((m_pBegin - HeaderSize));
		}

		// ��ʼ��
		void _Init()
		{
			if( nPreAlloc != 0 )
			{
				MemBlock *pNew	= reinterpret_cast<MemBlock *>(m_alloc.Allocate(sizeof(MemBlock)));
				pNew->pPrev		= NULL;
				m_pBegin		= pNew->buffer;
				m_pEnd			= reinterpret_cast<char *>(pNew) + m_alloc.AllocSize(pNew);
			}
			else
			{
				m_pBegin = m_pEnd = reinterpret_cast<char *>(HeaderSize);
			}
		}

		// ִ�з������
		void* _DoAllocate(size_type cb)
		{
			// �ڴ�������ֽ�������cb�����ڻ����һ��Block���ֽ���ʱ��������Ҫ�����ʹ���ڴ泬���������ȣ�BlockSize����MemBlock��
			// ��������ɵ�MemBlockȫ���ڴ汻�û�����
			if( cb >= BlockSize )
			{
				MemBlock* pHeader	= _ChainHeader();
				MemBlock* pNew		= reinterpret_cast<MemBlock *>(m_alloc.Allocate(HeaderSize + cb));

				// ��������ռ��������ͷ
				if( pHeader != NULL )
				{
					pNew->pPrev = pHeader->pPrev;
					pHeader->pPrev = pNew;
				}
				else
				{
					m_pEnd = m_pBegin = pNew->buffer;
					pNew->pPrev = NULL;
				}

				return pNew->buffer;
			}
			// ������ֽ�������cb��С��һ��MemBlock���ܹ��ṩ���ڴ棨��BlockSize��
			// ��������£�����ֻ��Ҫ����MemBlock��Ϊ�µĵ�ǰMemBlock�ҵ�������
			else
			{
				MemBlock* pNew	= reinterpret_cast<MemBlock *>(m_alloc.Allocate(sizeof(MemBlock)));
				pNew->pPrev		= _ChainHeader();
				m_pBegin		= pNew->buffer;
				m_pEnd			= reinterpret_cast<char *>(pNew) + m_alloc.AllocSize(pNew);

				return m_pEnd  -= cb;
			}
		}
	};



}

#endif