#ifndef __MEMORY_REGION_ALLOC_HPP
#define __MEMORY_REGION_ALLOC_HPP


#include "MemoryBase.hpp"





/**************************************************************************
*	适用性: 为一个复杂的的局部过程(算法)提供自动内存回收
*	
*	局部过程: 指算法复杂性较高、程序运行期所占时间有比较短暂的过程。
*
*	使用方式: 从局部过程的一开始，只管去申请、使用内存，知道算法完成，这个过程申请的大部分内存(需要作为算法结果保留的列外)，
*	无论在算法的哪个步骤申请，均在这个结束点上由垃圾回收器自动销毁(通过clear方法)
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


		// 利用单向链表来管理内存块
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
			// 情形1，当前MemBlock还有足够的自由内存(free memory)，
			if( static_cast<size_type>(m_pEnd - m_pBegin) >= cb )
			{
				return m_pEnd -= cb;
			}

			// 情形2，在当前的_MemBlock的自由内存（free memory）不足的情况下，我们就需要申请一个新的MemBlock以供使用。
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

		// 统一释放所申请的所有内存
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
		// 获取ChainHeader头指针
		MemBlock *_ChainHeader() const
		{
			return reinterpret_cast<MemBlock *>((m_pBegin - HeaderSize));
		}

		// 初始化
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

		// 执行分配操作
		void* _DoAllocate(size_type cb)
		{
			// 内存申请的字节数（即cb）大于或等于一个Block的字节数时，我们需要申请可使用内存超过正常长度（BlockSize）的MemBlock。
			// 这个新生成的MemBlock全部内存被用户申请
			if( cb >= BlockSize )
			{
				MemBlock* pHeader	= _ChainHeader();
				MemBlock* pNew		= reinterpret_cast<MemBlock *>(m_alloc.Allocate(HeaderSize + cb));

				// 把新申请空间放在链表头
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
			// 申请的字节数（即cb）小于一个MemBlock所能够提供的内存（即BlockSize）
			// 这种情况下，我们只需要将该MemBlock作为新的当前MemBlock挂到链表中
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