#ifndef __IOCP_OBJECT_HPP
#define __IOCP_OBJECT_HPP


#include "../Basic.hpp"
#include "pointer.hpp"
#include "MemoryMgr.hpp"


namespace async
{



	namespace iocp
	{
		class NullCallback
		{
			typedef std::tr1::function<void(u_long, u_long)> AllocCallback;
			typedef std::tr1::function<void(u_long, u_long)> DeallocCallback;

		public:
			static void Alloc(u_long){}
			static void Dealloc(u_long){}

			static void RegisterCallback(const AllocCallback &, const DeallocCallback &){}
		};


		//-------------------------------------------------------------------
		// class Callback
		// 测试使用

		class DebugCallback
		{
			typedef std::tr1::function<void(u_long, u_long)> AllocCallback;
			typedef std::tr1::function<void(u_long, u_long)> DeallocCallback;

		private:
			static AllocCallback allocCallback_;
			static DeallocCallback deallocCallback_;

		public:
			static void Alloc(u_long size);
			static void Dealloc(u_long size);

			static void RegisterCallback(const AllocCallback &, const DeallocCallback &);
		};

		#ifndef _DEBUG
			typedef NullCallback DefaultDebug;
		#else
			typedef DebugCallback DefaultDebug; 
		#endif 


		////-------------------------------------------------------------------
		//// class Object
		//// 引用计数和new delete基类

		//class Object
		//{
		//private:
		//	volatile LONG m_nRefCount;

		//public:
		//	Object()
		//		: m_nRefCount(0)
		//	{}
		//	virtual ~Object(){}

		//public:
		//	void Object::AddRef()
		//	{
		//		InterlockedIncrement(&m_nRefCount);
		//	}

		//	void Object::Release()
		//	{
		//		if( InterlockedDecrement(&m_nRefCount) == 0 ) 
		//		{
		//			delete this;
		//		}
		//	}

		//public:
		//	static void *operator new(size_t size)
		//	{
		//		DefaultDebug::Alloc(size);

		//		return MemoryMgr::GetMemory().allocate(size);
		//	}
		//	static void operator delete(void *ptr, size_t size)
		//	{
		//		DefaultDebug::Dealloc(size);

		//		if( ptr == NULL )
		//			return;

		//		return  MemoryMgr::GetMemory().deallocate((char *)ptr, size);
		//	}
		//};
	}



}



#endif