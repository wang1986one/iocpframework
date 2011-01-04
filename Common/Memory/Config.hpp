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
		���ܷ�����
			����һ���µ�AutoAllocʵ����һ���ȽϷ�ʱ�Ĳ��������û�Ӧע�������ڴ����Ĺ滮��
			������һ��ScopeAllocʵ���Ŀ�����С����������������Ϊ����ÿһ������ȥ����һ��ScopeAlloc��û�й�ϵ


			���ڶ������㷨���ԣ����ǲ���ȷ��������Ҫ��˽�д洢��Private GC Allocator�����ڴ�ռ��Ƕ��
			����˵��ͨ������Ҳ�����󡣶��ڽ������������ڴ�������£�ʹ��AutoAlloc�ǲ�̫���õ�������
			����Եģ������㷨������ڴ���٣�ʹ��ScopeAlloc�����Ի�÷ǳ�ƽ�ȵ����ܡ�


		���ۣ�
			AutoAlloc�н�ǿ�ľ����ԣ��������������޵ĳ��ϣ��ֲ��ĸ����㷨����
			��ScopeAlloc��ͨ���͵�Allocator���������κ�����£��㶼��ͨ��ʹ��ScopeAlloc�������ڴ�����Ի�����õ����ܻر���
	
	
	
			ScopeAlloc��AutoFreeAllocΨһ����������AutoFreeAlloc��ϵͳ�����ڴ棨���õ���malloc/free����
			��ScopeAlloc��һ���ڴ�أ���BlockPool�����õ���BlockPool::allocate/deallocate����
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