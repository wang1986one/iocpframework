#ifndef __IOCP_DISPATCHER_HPP
#define __IOCP_DISPATCHER_HPP


#include "Object.hpp"
#include "IOCP.hpp"
#include "AsyncResult.hpp"
#include "WinException.hpp"
#include "../MultiThread/Tls.hpp"

#include <vector>
#include <type_traits>


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
			typedef std::vector<HANDLE>			Threads;
			typedef Threads::const_iterator		ThreadsIter;
			typedef std::tr1::function<void()>	InitCallback;
			typedef std::tr1::function<void()>	UninitCallback;

		private:
			// iocp Handle
			Iocp iocp_;
			// �߳�����
			std::vector<HANDLE>	threads_;
			// �̴߳������ʼ������
			InitCallback initCallback_;
			// �߳��˳�ʱ��������
			UninitCallback unInitCallback_;

		public:
			explicit IODispatcher(size_t numThreads = GetFitThreadNum(), const InitCallback &init = 0, const UninitCallback &unint = 0);
			~IODispatcher();

		public:
			// ���ó�ʼ������������
			//void Register(const InitCallback &init, const UninitCallback &unint);
			// ���豸����ɶ˿�
			void Bind(HANDLE);
			// ����ɶ˿�Ͷ������
			template<typename HandlerT>
			void Post(const HandlerT &handler);
			// ���������̳߳���ʱ����������з���
			template<typename HandlerT>
			void Dispatch(const HandlerT &handler);

			// ֹͣ����
			void Stop();

		private:
			void _ThreadIO();

		private:
			static size_t WINAPI _ThreadIOImpl(LPVOID);
		};


		template < typename HandlerT >
		void IODispatcher::Post(const HandlerT &handler)
		{
			AsyncCallbackBasePtr async(MakeAsyncCallback(handler));

			async->AddRef();
			if( !iocp_.PostStatus(reinterpret_cast<ULONG_PTR>(&*(async.Get())), 0, 0) )
				throw Win32Exception("iocp_.PostStatus");
		}

		template < typename HandlerT >
		void IODispatcher::Dispatch(const HandlerT &handler)
		{
			if( async::thread::CallStack<IODispatcher>::Contains(this) )
			{
				AsyncCallbackBasePtr async(MakeAsyncCallback(handler));
				AsyncCallbackBase::Call(async);
			}
			else
				Post(handler);
		}
	}
}



#endif