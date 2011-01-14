#ifndef __IOCP_DISPATCHER_HPP
#define __IOCP_DISPATCHER_HPP


#include "Object.hpp"
#include "IOCP.hpp"
#include "AsyncResult.hpp"
#include "../MultiThread/Tls.hpp"

#include <vector>



namespace async
{

	namespace iocp 
	{
		
		namespace detail
		{
			/************************************************************************
			*  
			*	Int2Type		: ������ӳ��Ϊ���ͣ��ɱ����ڼ�������Ľ��ѡ��ͬ�ĺ������ﵽ��̬����
			*	���÷�ʽ			: Int2Type<isPolymorphics>
			*	��������			: ����Ҫ����ĳ�������ڳ�������һ����������ͬ�ĺ���
			*					  �б�Ҫ�ڱ�����ʵʩ��̬����
			*
			*	Type2Type		: ���ú������ػ��ƣ�ģ��templateƫ�ػ��������������ͱ��������ͱ���Ϣ
			*
			***********************************************************************/
			template<int v>
			struct Int2Type
			{
				enum { value = v };
			};

			template<typename T>
			struct Type2Type
			{
				typedef T value_type;
			};
		}


		// ��ȡ�ʺ�ϵͳ���߳���
		inline size_t GetFitThreadNum(size_t perCPU = 2)
		{
			SYSTEM_INFO systemInfo = {0};
			::GetSystemInfo(&systemInfo);

			return perCPU * systemInfo.dwNumberOfProcessors + 2;
		}



		//------------------------------------------------------------------
		// class IODispatcher
		// ��ɶ˿�ʵ��

		class IODispatcher
		{
		public:
			// �߳���������
			typedef std::vector<HANDLE>		Threads;
			typedef Threads::iterator		ThreadsIter;

		private:
			// iocp Handle
			Iocp iocp_;
			// �߳�����
			std::vector<HANDLE>	threads_;

		public:
			explicit IODispatcher(size_t numThreads = GetFitThreadNum());
			~IODispatcher();

		public:
			// ���豸����ɶ˿�
			void Bind(HANDLE);
			// ����ɶ˿�Ͷ������
			template<typename AsyncPtrT>
			void Post(AsyncPtrT);
			// ���������̳߳���ʱ����������з���
			template<typename AsyncPtrT>
			void Dispatch(AsyncPtrT);

			// ֹͣ����
			void Stop();

		private:
			void _ThreadIO();

		private:
			static size_t WINAPI _ThreadIOImpl(LPVOID);
		};

		namespace detail
		{
			template<typename IOCPT, typename AsyncPtrT>
			inline bool PostStatus(IOCPT &iocp, AsyncPtrT result, detail::Int2Type<FALSE> &)
			{
				return iocp.PostStatus(reinterpret_cast<ULONG_PTR>(&*result), 0, 0);
			}
			template<typename IOCPT, typename AsyncPtrT>
			inline bool PostStatus(IOCPT &iocp, AsyncPtrT result, detail::Int2Type<TRUE> &)
			{
				return iocp.PostStatus(0, 0, &*result);
			}
		}

		template<typename AsyncPtrT>
		void IODispatcher::Post(AsyncPtrT result)
		{
			result->AddRef();

			typedef std::tr1::remove_pointer<AsyncPtrT>::type AsyncCallbackType;
			if( !detail::PostStatus(iocp_, result, detail::Int2Type<AsyncCallbackType::IS_OVERLAPPED>()) )
				throw Win32Exception("iocp_.PostStatus");
		}


		template<typename AsyncPtrT>
		void IODispatcher::Dispatch(AsyncPtrT result)
		{	
			if( async::thread::CallStack<IODispatcher>::Contains(this) )
				AsyncCallbackBase::Call(result);
			else
				Post(result);
		}
	}
}



#endif