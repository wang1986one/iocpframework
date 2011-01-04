#ifndef __MEMORY_CONFIG_HPP
#define __MEMORY_CONFIG_HPP


#include "SystemAlloc.hpp"
#include "ProxyAlloc.hpp"


namespace DataStructure
{
	namespace Config
	{
		class Stdlib;
		class Scoped;
	}



	/*
		性能分析：
			生成一个新的AutoAlloc实例是一个比较费时的操作，其用户应注意做好内存管理的规划。
			而生成一个ScopeAlloc实例的开销很小，你甚至可以哪怕为生成每一个对象都去生成一个ScopeAlloc都没有关系


			对于多数的算法而言，我们不能确定它所需要的私有存储（Private GC Allocator）的内存空间是多大。
			或者说，通常它们也许并不大。而在仅仅申请少量内存的情形下，使用AutoAlloc是不太经济的做法。
			而相对的，无论算法所需的内存多少，使用ScopeAlloc都可以获得非常平稳的性能。


		结论：
			AutoAlloc有较强的局限性，仅仅适用于有限的场合（局部的复杂算法）；
			而ScopeAlloc是通用型的Allocator，基本在任何情况下，你都可通过使用ScopeAlloc来进行内存管理，以获得良好的性能回报。
	
	
	
			ScopeAlloc和AutoFreeAlloc唯一的区别，在于AutoFreeAlloc向系统申请内存（调用的是malloc/free），
			而ScopeAlloc向一个内存池（即BlockPool，调用的是BlockPool::allocate/deallocate）。
	*/



	// -------------------------------------------------------------------------
	typedef BlockAlloc<Config::Stdlib> BlockPool;

	// -------------------------------------------------------------------------
	typedef ProxyAlloc<BlockPool> ProxyBlockPool;

	



	namespace Config
	{
		class Sys
		{
		public:
			enum { MemBlockBytes = 65536 /* 64k */ };

		public:
			// type definitions
			typedef StdlibAlloc::value_type			value_type;
			typedef StdlibAlloc::pointer			pointer;
			typedef StdlibAlloc::const_pointer		const_pointer; 
			typedef StdlibAlloc::reference			reference;
			typedef StdlibAlloc::const_reference		const_reference;
			typedef StdlibAlloc::size_type			size_type;
			typedef StdlibAlloc::difference_type		difference_type;

			typedef StdlibAlloc						AllocType;
			typedef StdlibAlloc						ClassType;
		};

		class Stdlib 
			: public Sys
		{
		public:
			// type definitions
			typedef Sys::value_type			value_type;
			typedef Sys::pointer			pointer;
			typedef Sys::const_pointer		const_pointer; 
			typedef Sys::reference			reference;
			typedef Sys::const_reference		const_reference;
			typedef Sys::size_type			size_type;
			typedef Sys::difference_type		difference_type;

			typedef Sys::AllocType			AllocType;	
			typedef Stdlib					ClassType;
		};


		class Scoped 
			: public Sys
		{
		public:
			// type definitions
			typedef Sys::value_type		value_type;
			typedef Sys::pointer		pointer;
			typedef Sys::const_pointer	const_pointer;
			typedef Sys::reference		reference;
			typedef Sys::const_reference	const_reference;
			typedef Sys::size_type		size_type;
			typedef Sys::difference_type	difference_type;

			typedef ProxyBlockPool		AllocType;
			typedef Scoped				ClassType;
		};
	}



	

}


#endif