#ifndef __IOCP_OBJECT_HPP
#define __IOCP_OBJECT_HPP


#include "../Basic.hpp"
#include "pointer.hpp"
#include "MemoryMgr.hpp"


namespace async
{



	namespace IOCP
	{
		class NullCallback
		{
		public:
			static void Alloc(size_t){}
			static void Dealloc(size_t){}
		};


		//-------------------------------------------------------------------
		// class Callback
		// ����ʹ��

		class Callback
		{
			typedef std::tr1::function<void(LONG, size_t)> AllocCallback;
			typedef std::tr1::function<void(LONG, size_t)> DeallocCallback;

		private:
			static AllocCallback m_allocCallback;
			static DeallocCallback m_deallocCallback;

		public:
			static void Alloc(size_t size);
			static void Dealloc(size_t size);

			static void RegisterCallback(AllocCallback allocCallback, DeallocCallback deallocCallback);
		};



		//-------------------------------------------------------------------
		// class Object
		// ���ü�����new delete����

		class Object
		{
		private:
			volatile LONG m_nRefCount;

		public:
			Object()
				: m_nRefCount(0)
			{}
			~Object(){}

		public:
			void Object::AddRef()
			{
				InterlockedIncrement(&m_nRefCount);
			}

			void Object::Release()
			{
				if( InterlockedDecrement(&m_nRefCount) == 0 ) 
				{
					delete this;
				}
			}

		public:
			static void *operator new(size_t size)
			{
				Callback::Alloc(size);

				return MemoryMgr::GetMemory().allocate(size);
			}
			static void operator delete(void *ptr, size_t size)
			{
				Callback::Dealloc(size);

				if( ptr == NULL )
					return;

				return  MemoryMgr::GetMemory().deallocate((char *)ptr, size);
			}
		};


		typedef pointer<Object>	ObjectPtr;

		// VC ������չ����֤��ľ�̬��Ա������ͷ�ļ�����������
		__declspec(selectany) ObjectPtr nothing(new Object);
	}



}



#endif