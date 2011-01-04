#ifndef __MEMORY_OBJ_POOL_HPP
#define __MEMORY_OBJ_POOL_HPP


#include "FixedAlloc.hpp"



namespace DataStructure
{
	// -------------------------------------------------------------------------
	// class NormalObjectPool
	
	#pragma pack(1)
	template<typename T, typename PolicyT>
	class NormalObjectPool
		: private FixedAlloc<PolicyT>
	{
	public:
		typedef FixedAlloc<PolicyT>			PoolT;
		typedef typename PoolT::MemBlock	MemBlock;


		typedef typename PoolT::AllocType	AllocType;
		typedef typename PoolT::size_type	size_type;

		using PoolT::AllocSize;
		using PoolT::GetAlloc;


	private:
		struct ChunckHeader
		{
			union
			{
				MemBlock *m_pBlock;
				size_type m_tag;
			};
		};

	private:
		NormalObjectPool(const NormalObjectPool &);
		NormalObjectPool &operator=(const NormalObjectPool &);

	public:
		NormalObjectPool()
			: PoolT(sizeof(T))
		{
			STATIC_ASSERT(sizeof(ChunckHeader) == sizeof(typename PoolT::ChunckHeader), NormalObjectPool);
		}
		explicit NormalObjectPool(const AllocType &alloc)
			: PoolT(alloc, sizeof(T))
		{
			STATIC_ASSERT(sizeof(ChunckHeader) == sizeof(typename PoolT::ChunckHeader), NormalObjectPool);
		}
		~NormalObjectPool()
		{
			clear();
		}

	public:
		void clear()
		{
			MemBlock *blk = m_blocks.First();

			m_blocks.clear();
			m_freeList.clear();
	
			for(MemBlock *pNextBlock = NULL; !m_blocks.Done(blk); blk = pNextBlock)
			{
				pNextBlock = blk->Next();
				if( blk->m_nUsed )
					_DoClearBlock(blk, m_nChunk);

				m_alloc.Deallocate(blk);
			}

		}


		void *Allocate(size_type size, DestructorT /*fn*/)
		{
			assert(size == sizeof(T) );

			void *p = PoolT::Allocate();
			MarkAllocated(p);

			return p;
		}

		void *UnManagedAlloc(size_type size, DestructorT fn)
		{
			assert( size == sizeof(T) );

			return PoolT::Allocate();
		}

		void Manage(void *p, DestructorT fn)
		{
			MarkAllocated(p);
		}

		T *Construct()
		{
			//T *p = New<T>(PoolT::GetAlloc());
			T *p = new(PoolT::Allocate()) T;

			MarkAllocated(p);

			return p;
		}

		void Destroy(T *obj)
		{
			MarkDeallocated(obj);

			obj->~T();

			PoolT::Deallocate(obj);

		}

	protected:
		static size_type IsAllocated(void *p)
		{
			return 1 & (reinterpret_cast<ChunckHeader *>(p) - 1)->m_tag;
		}
		
		static void MarkAllocated(void *p)
		{
			assert(!IsAllocated(p));

			++(reinterpret_cast<ChunckHeader *>(p) - 1)->m_tag;
		}

		static void MarkDeallocated(void *p)
		{
			assert(IsAllocated(p));

			--(reinterpret_cast<ChunckHeader *>(p) - 1)->m_tag;
		}

	private:
		static void _DoClearBlock(MemBlock * const blk, size_type sizeChunk)
		{
			size_type nUsed = blk->m_nUsed;
			char *p = blk->m_buffer + sizeof(PoolT::ChunckHeader);

			for(; ; p += sizeChunk)
			{
				if( IsAllocated(p) )
				{
					reinterpret_cast<T *>(p)->~T();
					if( --nUsed == 0 )
						break;
				}
			}
		}

	private:
		void swap(NormalObjectPool &);
	};
	#pragma pack()


	// -------------------------------------------------------------------------
	// class PodObjectPool
	#pragma pack(1)
	template<typename T, typename PolicyT>
	class PodObjectPool:
		private FixedAlloc<PolicyT>
	{
	private:
		typedef FixedAlloc<PolicyT>		PoolT;

	public:
		typedef typename PoolT::AllocType		AllocType;
		typedef typename PoolT::size_type		size_type;

		using PoolT::AllocSize;
		using PoolT::GetAlloc;
		using PoolT::clear;


	public:
		PodObjectPool()
			: PoolT(sizeof(T))
		{}
		explicit PodObjectPool(const AllocType &alloc)
			: PoolT(alloc, sizeof(T))
		{}


	public:
		void *Allocate(size_type size, int nZero)
		{
			assert(size == sizeof(T));

			return PoolT::Allocate();
		}

		void *UnManagedAlloc(size_type size, int nZero)
		{
			assert(size == sizeof(T));

			return PoolT::Allocate();
		}


		void Manage(void *p, int nZero)
		{

		}


		T *Construct()
		{
			return new(PoolT::Allocate()) T();
		}

		void Destroy(T *obj)
		{
			PoolT::Deallocate(obj);
		}

	private:
		PodObjectPool(const PodObjectPool &);
		PodObjectPool &operator=(const PodObjectPool &);

	private:
		void swap(PodObjectPool &rhs);
	};
	#pragma pack()



	// -------------------------------------------------------------------------
	// class ObjectPoolTraits

	template<
		typename T, 
		typename PolicyT = Config::Stdlib, 
		int nHasDestructor = DestructorTraits<T>::hasDestructor
	>
	struct ObjectPoolTraits
	{
		typedef NormalObjectPool<T, PolicyT>	ObjectPoolType;
	};

	template<typename T, typename PolicyT>
	struct ObjectPoolTraits<T, PolicyT, 0>
	{
		typedef PodObjectPool<T, PolicyT>		ObjectPoolType;
	};


	// -------------------------------------------------------------------------
	// class ObjectPool

	template<typename T, typename PolicyT = Config::Stdlib>
	class ObjectPool
		: public ObjectPoolTraits<T, PolicyT>::ObjectPoolType
	{
	private:
		typedef typename ObjectPoolTraits<T, PolicyT>::ObjectPoolType Base;
		typedef typename Base::AllocType		AllocType;	


	public:
		ObjectPool()
		{}

		explicit ObjectPool(const AllocType &alloc)
			: Base(alloc)
		{}
	};
}







#endif