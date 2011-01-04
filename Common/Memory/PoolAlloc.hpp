#ifndef __MEMORY_POOL_ALLOC_HPP
#define __MEMORY_POOL_ALLOC_HPP


#include "MemoryBase.hpp"

namespace DataStructure
{


	// -------------------------------------------------------------------------
	// class PoolAlloc

	template<typename PolicyT>
	class PoolAlloc
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

		typedef PoolAlloc<PolicyT>					ClassType;
		typedef typename PolicyT::AllocType			AllocType;

	public:
		enum { MemBlockSize = PolicyT::MemBlockBytes - AllocType::Padding };

	protected:
		struct MemBlock;
		friend struct MemBlock;

		enum { HeaderSize = sizeof(void*) };
		enum { BlockSize = MemBlockSize - HeaderSize };

		struct MemBlock
		{
			MemBlock *pPrev;
			char buffer[BlockSize];
		};

		struct FreeChunk
		{
			FreeChunk *pPrev;
		};

		enum { MinElemBytes = sizeof(FreeChunk) };

		FreeChunk *m_pFreelist;
		MemBlock *m_pBlocks;
		size_type m_cbChunk;
		size_type m_nChunkPerBlock;
		AllocType m_alloc;


	public:
		explicit PoolAlloc(size_type cbElem)
		{
			_Init(cbElem);
		}

		PoolAlloc(const AllocType &alloc, size_type cbElem) 
			: m_alloc(alloc)
		{
			_Init(cbElem);
		}

		~PoolAlloc()
		{
			clear();
		}

	private:
		PoolAlloc(const PoolAlloc &);
		PoolAlloc &operator=(const PoolAlloc &);

	public:
		size_type AllocSize() const
		{
			return m_cbChunk;
		}

		AllocType GetAlloc() const
		{
			return m_alloc;
		}

		void clear()
		{
			MemBlock *nextBlock = NULL;
			for(MemBlock *blk = m_pBlocks; blk != NULL; blk = nextBlock)
			{
				nextBlock = blk->pPrev;
				m_alloc.Deallocate(blk);
			}

			m_pBlocks	= NULL;
			m_pFreelist = NULL;
		}

		void* Allocate()
		{
			if( m_pFreelist != NULL )
			{
				FreeChunk *p = m_pFreelist;
				m_pFreelist = p->pPrev;

				Internal::DebugFill(p, AllocSize());
				return p;
			}

			return _DoAllocate();
		}

		void* Allocate(size_type cb)
		{
			assert(cb <= AllocSize());

			return Allocate();
		}

		void Deallocate(void* const p)
		{
			Internal::DebugFill(p, AllocSize());

			static_cast<FreeChunk *>(p)->pPrev = m_pFreelist;
			m_pFreelist = static_cast<FreeChunk *>(p);
		}

		void Deallocate(void* const p, size_type cb)
		{
			assert(cb == AllocSize());
			Internal::DebugFill(p, AllocSize());

			static_cast<FreeChunk *>(p)->pPrev = m_pFreelist;
			m_pFreelist = static_cast<FreeChunk *>(p);
		}

	private:
		void _Init(size_type cbElem)
		{
			cbElem				= Internal::Round(cbElem, sizeof(void*));
			m_cbChunk			= Internal::Max(cbElem, static_cast<size_type>(MinElemBytes));
			m_nChunkPerBlock	= BlockSize / m_cbChunk;
			m_pFreelist			= NULL;
			m_pBlocks			= NULL;

			assert(m_nChunkPerBlock > 0);
		}

		void *_DoAllocate()
		{
			MemBlock* const blk = reinterpret_cast<MemBlock *>(m_alloc.Allocate(sizeof(MemBlock)));
			blk->pPrev = m_pBlocks;
			m_pBlocks = blk;

			FreeChunk *p = reinterpret_cast<FreeChunk *>(blk->buffer);
			p->pPrev = m_pFreelist;

			for(size_type i = m_nChunkPerBlock; --i;)
			{
				FreeChunk* const n = reinterpret_cast<FreeChunk *>(reinterpret_cast<char *>(p) + m_cbChunk);
				n->pPrev = p;
				p = n;
			}

			m_pFreelist = p->pPrev;

			Internal::DebugFill(p, AllocSize());
			return p;
		}
	};

	// -------------------------------------------------------------------------

}

#endif