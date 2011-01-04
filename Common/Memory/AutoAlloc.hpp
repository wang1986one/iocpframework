#ifndef __MEMORY_AUTO_ALLOC_HPP
#define __MEMORY_AUTO_ALLOC_HPP


#include "RegionAlloc.hpp"


namespace DataStructure
{


	// -------------------------------------------------------------------------
	// class AutoAlloc
	// ����malloc��free�����ڴ������ͷ�

	typedef RegionAlloc<Config::Stdlib> AutoAlloc;

	// -------------------------------------------------------------------------
}


#endif