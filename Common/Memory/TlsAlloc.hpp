#ifndef __MEMORY_TLS_ALLOC_HPP
#define __MEMORY_TLS_ALLOC_HPP


#include "MemoryBase.hpp"
#include "config.hpp"
#include "../Thread/ThreadTLS.hpp"


namespace DataStructure
{
	// -------------------------------------------------------------------------
	// class TlsPoolsAlloc

	template<typename PolicyT>
	class TlsPoolsAlloc
	{
	public:
		typedef PoolsAlloc<PolicyT>				PoolsType;
		typedef typename PolicyT::size_type		size_type;

	private:
		static ThreadTls m_threadTls;

	

	public:
		static void Put(PoolsType &rhs)
		{
			static int nRes = _Init();
			m_threadTls.Put(&rhs);
		}

		static PoolsType &Get()
		{
			PoolsType *pools = reinterpret_cast<PoolsType *>(m_threadTls.Get());

			assert(pools != NULL);

			return *pools;
		}

		static void *Allocate(size_type size)
		{
			return Get().Allocate(size);
		}

		static void Deallocate(void *p, size_type size)
		{
			return Get().Deallocate(p, size);
		}


		static size_type AllocSize(void *p, size_type size)
		{
			return Get().AllocSize(p, size);
		}


	private:
		static int _Init()
		{
			m_threadTls.Create();
		}
	};

	template<typename PolicyT>
	ThreadTls TlsPoolsAlloc<PolicyT>::m_threadTls;



	// -------------------------------------------------------------------------
	// class TlsPools
	typedef TlsPoolsAlloc<Config::Stdlib> TlsPools;

}




#endif