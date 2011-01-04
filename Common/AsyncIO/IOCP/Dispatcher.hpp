#ifndef __IOCP_DISPATCHER_HPP
#define __IOCP_DISPATCHER_HPP


#include "Object.hpp"

#include "IOCP.hpp"
#include <vector>



namespace async
{

	namespace IOCP 
	{
		struct AsyncResult;
		typedef pointer<AsyncResult> AsyncResultPtr;

		typedef std::tr1::function<void(const AsyncResultPtr &asyncResult)> AsyncCallbackFunc;


		//---------------------------------------------------------------------------
		// class AsyncResult
		// ��Ӳ㣬���𻺳������շ��ߺͻص�����

		struct AsyncResult
			: public OVERLAPPED
			, public Object
		{
			// ������
			ObjectPtr	m_sender;
			// ������
			ObjectPtr	m_buffer;
			// �첽״̬
			ObjectPtr	m_asynState;
			// �ڲ�״̬
			ObjectPtr	m_internalState;
			// �ص�����
			AsyncCallbackFunc m_callback;


			AsyncResult::AsyncResult(const ObjectPtr &sender, const ObjectPtr &buffer,
				const ObjectPtr &asyncState, const ObjectPtr &internalState, const AsyncCallbackFunc &callback)
				: m_sender(sender), m_buffer(buffer), m_asynState(asyncState), m_internalState(internalState), m_callback(callback)
			{
				RtlZeroMemory((OVERLAPPED *)this, sizeof(OVERLAPPED));
			}

			void reset(const ObjectPtr &sender, const ObjectPtr &buffer,
				const ObjectPtr &asyncState, const ObjectPtr &internalState, const AsyncCallbackFunc &callback)
			{
				m_sender		= sender;
				m_buffer		= buffer;
				m_asynState		= asyncState;
				m_internalState = internalState;
				m_callback		= callback;
			}

		private:
			AsyncResult();
		};


		// -----------------------------------------------------------------
		// ��ȡϵͳ���ʵ��߳���
		extern size_t GetFitThreadNum(size_t perCPU = 2);



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
			// IOCP Handle
			CIOCP m_hIOCP;
			// �߳�����
			std::vector<HANDLE>	m_threads;

		public:
			IODispatcher(size_t numThreads = 0);
			~IODispatcher();

		public:
			// ���豸����ɶ˿�
			void Bind(HANDLE hObject);
			// ����ɶ˿�Ͷ������
			void Post(const AsyncResultPtr &ar);

			// ֹͣ����
			void Stop();

		private:
			void _ThreadIO();

		private:
			static size_t WINAPI _ThreadIOImpl(LPVOID pObj);
		};
	}


}


#endif