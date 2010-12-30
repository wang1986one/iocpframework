#ifndef __MEM_ALLOCATOR_HPP
#define __MEM_ALLOCATOR_HPP

#include <type_traits>



namespace async
{
	namespace memory
	{

		// -----------------------------------------------------------------
		// class MemAllocator
		// ¿‡À∆”⁄std::allocator

		template<typename T, typename AllocT>
		class MemAllocator
		{
		public:
			typedef typename std::tr1::remove_volatile<T>::type		NonVolatileValueType;
			typedef typename std::tr1::remove_const<NonVolatileValueType>::type value_type;
			typedef value_type*										pointer;
			typedef const value_type*								const_pointer;
			typedef typename value_type&							reference;
			typedef const value_type&								const_reference;
			typedef size_t											size_type;
			typedef ptrdiff_t										difference_type;
			
			typedef AllocT											allocator_type;

			// rebind allocator to type U
			template<typename U>
			struct rebind 
			{
				typedef MemAllocator<U, AllocT> other;
			};


		private:
			AllocT &alloc_;

		public:
			explicit MemAllocator(AllocT &alloc)
				: alloc_(alloc)
			{}
			MemAllocator(const MemAllocator &rhs)
				: alloc_(rhs.alloc_)
			{}
			template<typename U>
			explicit MemAllocator(const MemAllocator<U, AllocT>& rhs)
				: alloc_(rhs.alloc_)
			{}
			~MemAllocator()
			{}

		public:
			pointer address(reference value) const 
			{
				return &value;
			}
			const_pointer address(const_reference value) const 
			{
				return &value;
			}

			size_type max_size() const
			{
				size_type count = static_cast<size_type>(-1) / sizeof(value_type);

				return 0 < count ? count : 1;
			}

			pointer allocate() 
			{
				pointer ret = reinterpret_cast<pointer>(alloc_.Allocate(sizeof(value_type)));

				return ret;
			}

			pointer allocate(size_type num, const void* = 0) 
			{
				pointer ret = reinterpret_cast<pointer>(alloc_.Allocate(num * sizeof(value_type)));

				return ret;
			}

			void deallocate(pointer p) 
			{
				return alloc_.Deallocate(p, sizeof(value_type));
			}
			void deallocate(pointer p, size_type num) 
			{
				return alloc_.Deallocate(p, num * sizeof(value_type));
			}
		};
	}
}


#endif