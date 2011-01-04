#ifndef __MEMORY_POOL_HPP
#define __MEMORY_POOL_HPP


#include "PoolAlloc.hpp"


namespace DataStructure
{



	// -------------------------------------------------------------------------
	// class Pool, ScopedPool

	typedef PoolAlloc<Config::Stdlib> Pool;
	typedef PoolAlloc<Config::Scoped> ScopedPool;

	// -------------------------------------------------------------------------

}


#endif