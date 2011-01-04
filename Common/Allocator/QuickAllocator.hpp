#ifndef __QUICK_ALLOCATOR_HPP
#define __QUICK_ALLOCATOR_HPP


#include <type_traits>
#include "../Constraints.hpp"
#include "../Thread/Lock.hpp"



namespace DataStructure
{

	namespace detail
	{
		template<size_t Size, size_t Align>
		union FreeBlock
		{
			typedef typename TypeWithAligment<Align>::type aligner_type;
			aligner_type aligner_;
			char bytes_[Size];
			FreeBlock *next_;
		};

		

		template<size_t Size, size_t Align>
		struct AllocatorImpl
		{
			typedef FreeBlock<Size, Align> BlockType;
			typedef async::thread::AutoCriticalSection Mutex;
			typedef async::thread::AutoLock<Mutex>	AutoLock;

			enum { ITEM_PER_PAGE = 512 / Size };

			static Mutex &MutexInstance()
			{
				static FreeBlock<sizeof(Mutex), std::tr1::alignment_of<Mutex>::value> fbm;
				static Mutex *mutex = new(&fbm) Mutex;

				return *mutex;
			}

			static Mutex *mutexInit_;
			static BlockType *free_;
			static BlockType *page_;
			static size_t last_;

			static void *allocate()
			{
				AutoLock lock(MutexInstance());

				BlockType *block = free_;
				if( block != 0 )
				{
					free_ = block->next_;
					return block;
				}
				else
				{
					if( last_ == ITEM_PER_PAGE )
					{
						page_ = ::new BlockType[ITEM_PER_PAGE];
						last_ = 0;
					}

					return &page_[last_++];
				}
			}

			static void *allocate(size_t sz)
			{
				if( sz != Size )
					return ::operator new(sz);
				else
					return allocate();
			}

			static void deallocate(void *ptr)
			{
				if( ptr != 0 )
				{
					AutoLock lock(MutexInstance());

					BlockType *block = static_cast<BlockType *>(ptr);
					block->next_ = free_;
					free_ = block;
				}
			}

			static void deallocate(void *ptr, size_t sz)
			{
				if( sz != Size )
					return ::operator delete(ptr);
				else
					return deallocate(ptr);
			}
		};

		template<size_t Size, size_t Align>
		typename AllocatorImpl<Size, Align>::Mutex *AllocatorImpl<Size, Align>::mutexInit_ = &AllocatorImpl<Size, Align>::MutexInstance();

		template<size_t Size, size_t Align>
		FreeBlock<Size, Align> *AllocatorImpl<Size, Align>::free_ = 0;

		template<size_t Size, size_t Align>
		FreeBlock<Size, Align> *AllocatorImpl<Size, Align>::page_ = 0;

		template<size_t Size, size_t Align>
		size_t AllocatorImpl<Size, Align>::last_ = AllocatorImpl<Size, Align>::ITEM_PER_PAGE;
	}

	template< typename T >
	struct QuickAllocator
		: public detail::AllocatorImpl<sizeof(T), std::tr1::alignment_of<T>::value>
	{

	};
}







#endif