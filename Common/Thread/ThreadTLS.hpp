#ifndef __THREAD_THREAD_TLS_HPP
#define __THREAD_THREAD_TLS_HPP

#include "../Thread.hpp"


namespace DataStructure
{

	// -------------------------------------------------------------------------
	// class ThreadTls
	
	class ThreadTls
	{
	private:
		DWORD m_dwKey;

	public:
		ThreadTls()
			: m_dwKey(TLS_OUT_OF_INDEXES)
		{}

	public:
		void Create()
		{
			m_dwKey = ::TlsAlloc();
		}

		void clear()
		{
			::TlsFree(m_dwKey);
		}

		void Put(void *p) const
		{
			::TlsSetValue(m_dwKey, p);
		}

		void *Get() const
		{
			return ::TlsGetValue(m_dwKey);
		}
	};
}

#endif