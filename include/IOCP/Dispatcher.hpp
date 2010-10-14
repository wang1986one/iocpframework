#ifndef __IOCP_DISPATCHER_HPP
#define __IOCP_DISPATCHER_HPP


#include "Object.hpp"
#include "IOCP.hpp"
#include "AsyncResult.hpp"

#include <vector>



namespace async
{

	namespace iocp 
	{
		
		namespace internal
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

		template<typename AsyncT>
		class IODispatcher
		{
			typedef AsyncT					AsyncType;

		public:
			// �߳���������
			typedef std::vector<HANDLE>		Threads;
			typedef Threads::iterator		ThreadsIter;

		private:
			// iocp Handle
			CIOCP iocp_;
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
			void Post(AsyncPtrT &);
			// ���������̳߳���ʱ����������з���
			template<typename AsyncPtrT>
			void Dispatch(AsyncPtrT &);

			// ֹͣ����
			void Stop();

		private:
			template<typename AsyncPtrT>
			void _PostImpl(AsyncPtrT &, internal::Int2Type<FALSE>);
			template<typename AsyncPtrT>
			void _PostImpl(AsyncPtrT &, internal::Int2Type<TRUE>);

			void _ThreadIO();

		private:
			static size_t WINAPI _ThreadIOImpl(LPVOID);
		};

		typedef IODispatcher<AsyncResult>			OverlappedDispatcher;
		typedef IODispatcher<AsyncCallbackBase>		AsyncCallbackDispatcher;
	}
}


#include "DispatcherImpl.hpp"

#endif