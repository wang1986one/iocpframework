#ifndef __MEMORY_SCOPED_ALLOC_HPP
#define __MEMORY_SCOPED_ALLOC_HPP




namespace DataStructure
{



	// -------------------------------------------------------------------------
	// class ScopedAlloc 利用内存池来进行内存分配调用

	// 通常在任何情况下，都可用ScopedAlloc来进行内存管理，获取良好的性能回报

	typedef RegionAlloc<Config::Scoped> ScopedAlloc;

	// -------------------------------------------------------------------------

}




#endif