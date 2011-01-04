#ifndef __MEMORY_AUTO_ALLOC_HPP
#define __MEMORY_AUTO_ALLOC_HPP


#include "RegionAlloc.hpp"


namespace DataStructure
{


	// -------------------------------------------------------------------------
	// class AutoAlloc
	// 利用malloc、free进行内存申请释放

	typedef RegionAlloc<Config::Stdlib> AutoAlloc;

	// -------------------------------------------------------------------------
}


#endif