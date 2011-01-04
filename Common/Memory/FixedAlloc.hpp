#ifndef __MEMORY_FIEXED_ALLOC_HPP
#define __MEMORY_FIEXED_ALLOC_HPP


#include "MemoryBase.hpp"
#include "../Container/DclList.hpp"


namespace DataStructure
{

	// -------------------------------------------------------------------------
	// class FixedAlloc
	
	template<typename PolicyT>
	class FixedAlloc
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

		enum { MemBlockSize = PolicyT::MemBlockBytes - AllocType::Padding };

	protected:
		struct MemBlock;
		friend struct MemBlock;

		struct Header:
			public DclListNode<MemBlock>
		{
			size_type m_nUsed;
		};

		enum { HeaderSize = sizeof(Header) };
		enum { BlockSize = MemBlockSize - HeaderSize };

		struct MemBlock:
			public Header
		{
			char m_buffer[BlockSize];
		};

		struct ChunckHeader
		{
			MemBlock *m_pBlock;
		};

		struct FreeChunk:
			DclListNode<FreeChunk>
		{

		};

		enum { ChunckHeaderSize = sizeof(ChunckHeader) };
		enum { MinElementBytes  = sizeof(FreeChunk)  };

		DclList<FreeChunk>		m_freeList;
		DclList<MemBlock>		m_blocks;
		size_type				m_nChunk;
		size_type				m_nChunkPerBlock;

		MemBlock				*m_pLastBlock;

		AllocType				m_alloc;
		

	public:
		explicit FixedAlloc(size_type nElement)
			: m_nChunk(0)
			, m_nChunkPerBlock(0)
			, m_pLastBlock(NULL)
		{
			_Init(nElement);
		}
		FixedAlloc(const AllocType &alloc, size_type nElement)
			: m_nChunk(0)
			, m_nChunkPerBlock(0)
			, m_pLastBlock(NULL)
			, m_alloc(alloc)
		{
			_Init(nElement);
		}

		~FixedAlloc()
		{
			clear();
		}


	public:
		size_type AllocSize() const
		{
			return m_nChunk - ChunckHeaderSize;
		}

		const AllocType &GetAlloc() const
		{
			return m_alloc;
		}

		void clear()
		{
			MemBlock *pNextBlock = NULL;

			for(MemBlock *blk = m_blocks.First(); !m_blocks.Done(blk); blk = pNextBlock)
			{
				pNextBlock = blk->Next();
				m_alloc.Deallocate(blk);
			}

			m_blocks.clear();
			m_freeList.clear();
		}


	private:
		FixedAlloc(const FixedAlloc &);
		FixedAlloc &operator=(const FixedAlloc &);

	public:
		void *Allocate()
		{
			if( m_freeList.empty() )
				_DoAllocateBlock();

			void *p = &m_freeList.front();
			++(_ChunkHeader(p)->m_nUsed);
			m_freeList.PopFront();

			Internal::DebugFill(p, AllocSize());
			return p;
		}

		void *Allocate(size_type size)
		{
			assert( size <= AllocSize() );
			return Allocate();
		}

		void Deallocate(void * const p)
		{
			MemBlock * const blk = _ChunkHeader(p);

			assert(blk->m_nUsed > 0 && blk->m_nUsed <= m_nChunkPerBlock);
			Internal::DebugFill(p, AllocSize());

			m_freeList.push_back(reinterpret_cast<FreeChunk *>(p));

			if( --blk->m_nUsed == 0 && blk != m_pLastBlock )
				_DoDeallocateBlock(blk);
		}

		void Deallocate(void * const p, size_type size)
		{
			assert(cb == AllocSize());
			Deallocate(p);
		}

	private:
		void _Init(size_type nElement)
		{
			nElement			= Internal::Round(nElement, sizeof(void *));
			m_nChunk			= Internal::Max(nElement, static_cast<size_type>(MinElementBytes)) + ChunckHeaderSize;
			m_pLastBlock		= NULL;
			m_nChunkPerBlock	= BlockSize / m_nChunk;

			assert(m_nChunkPerBlock > 0);
		}

		MemBlock *&_ChunkHeader(void * const p)
		{
			return (reinterpret_cast<ChunckHeader *>(p) - 1)->m_pBlock;
		}

		void _DoAllocateBlock()
		{
			MemBlock * const blk = reinterpret_cast<MemBlock *>(m_alloc.Allocate(sizeof(MemBlock)));
			
			m_blocks.PushFront(blk);
			m_pLastBlock = blk;

			blk->m_nUsed = 0;
			char *p = blk->m_buffer + ChunckHeaderSize;

			for(size_type i = m_nChunkPerBlock; --i; p += m_nChunk)
			{
				_ChunkHeader(p) = blk;
				m_freeList.PushFront(reinterpret_cast<FreeChunk *>(p));
			}

			assert(!m_freeList.empty());
		}

		void _DoDeallocateBlock(MemBlock * const blk)
		{
			char *p = blk->m_buffer + ChunckHeaderSize;
			for(size_type i = m_nChunkPerBlock; --i; p += m_nChunk)
			{
				reinterpret_cast<FreeChunk *>(p)->erase();
			}

			blk->erase();
			m_alloc.Deallocate(blk);
		}


	private:
		void swap(FixedAlloc &);		// No swap
	};
}

#endif