#ifndef __MEMORY_PROXY_ALLOC_HPP
#define __MEMORY_PROXY_ALLOC_HPP


#include "BlockAlloc.hpp"


namespace DataStructure
{


	// -------------------------------------------------------------------------
	// class ProxyAlloc
	// 代理类，负责对内存声请释放的转发

	template<typename AllocT>
	class ProxyAlloc
	{
	public:
		// type definitions
		typedef typename AllocT::value_type			value_type;
		typedef typename AllocT::pointer			pointer;
		typedef typename AllocT::const_pointer		const_pointer;
		typedef typename AllocT::reference			reference;
		typedef typename AllocT::const_reference	const_reference;
		typedef typename AllocT::size_type			size_type;
		typedef typename AllocT::difference_type	difference_type;

		typedef ProxyAlloc<AllocT>				ClassType;
		typedef typename AllocT::AllocType		AllocType;		

	private:
		AllocT &m_alloc;

	public:
		enum { Padding = AllocT::Padding };

	public:
		ProxyAlloc(AllocT &alloc) 
			: m_alloc(alloc) 
		{}

		ProxyAlloc(const ProxyAlloc &rhs)
			: m_alloc(rhs.m_alloc)
		{}

		ProxyAlloc &operator=(const ProxyAlloc &rhs)
		{
			if( &rhs != this )
				m_alloc = rhs.m_alloc;

			return *this;
		}

	public:
		void* Allocate(size_type cb)	
		{ return m_alloc.Allocate(cb); }

		void Deallocate(void* p, size_t nSize = 0) 		
		{ m_alloc.Deallocate(p); }

		void swap(ClassType &o)		
		{ swap(m_alloc, o.m_alloc); }

		size_type AllocSize(void* p) const 
		{ return m_alloc.AllocSize(p); }

		AllocT &Instance() const
		{ return m_alloc; }

		AllocT *operator&() const 
		{ return &m_alloc; }

		operator AllocT() const
		{ return m_alloc; }
	};


	


}


#endif