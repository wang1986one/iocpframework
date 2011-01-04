#ifndef __THREAD_THREADPOOL_HPP
#define __THREAD_THREADPOOL_HPP




namespace async
{
	
	/*
		�̳߳ص����������Ϊ���ԣ�

							Timer						RegisterWait					BindIO							QueueUser
	
		�̵߳ĳ�ʼ��ֵ		����1						0								0								0

		������һ���߳�ʱ		�����õ�һ���̳߳غ���ʱ		ÿ63��ע�����һ���߳�				ϵͳ��ʹ����̽��������������Ӱ�죺
																							1. �Դ�����̺߳��Ѿ���ȥһ��ʱ��(��λms)
																							2. ʹ��WT_EXECUTELONGFUNCTION
																							3. �Ѿ��ŶӵĹ�����Ŀ������������ĳ����ֵ

		���̱߳�����ʱ		��������ֹʱ					���Ѿ�ע��ĵȴ���������Ϊ0		���߳�û��δ�����IO����
																						�����Ѿ�������һ����ֵ������		���߳̿�����һ����ֵ������
	
		�߳���εȴ�			�����ȴ�						WaitForMultipleObjects			�����ȴ�							GetQueuedCompletionStatus

		ʲô�����߳�			�ȴ���ʱ��֪ͨ�Ŷӵ�APC		�ں˶����Ϊ��֪ͨ״̬				�Ŷӵ��û�APC������ɵ�IO����		չʾ����ɵ�״̬��IO��ʾ
		



	*/

	


	/*
		WT_EXECUTEDEFAULT-- Ĭ������£��ûص��������Ŷӵ��� i/o �����̡߳���ͨ��IO��ɶ˿�ʵ�ֵ�
		�ص��������Ŷӣ�����ζ�����ǲ���ִ��һ�� alertable �ĵȴ� I/O ��ɶ˿�ʹ�õ��̡߳� 
		��ˣ������� I/O ������ APC���������ڵȴ���Ϊ���ܱ�֤���߳��ڻص���ɺ󽫽��� alertable �ȴ�״̬��

		WT_EXECUTEINIOTHREAD--�ص�������һ�� I/O �����߳����Ŷӡ� ���߳̽�ִ��һ�� alertable �ĵȴ��� 
		����Ч�ʽϵͣ���ˣ������ص�����ǰ�߳����� apc �ͺ���̷߳��ص��̳߳أ���Ӧִ�� APC ʱ����Ӧʹ�ô˱�־�� 
		�ص���������Ϊ APC �Ŷӡ� ����ú���ִ�� alertable �ȴ�����������ؽ�������������⡣

		WT_EXECUTEINPERSISTENTTHREAD--�ص��������Ŷӵ���Զ������ֹ���̡߳� �����ܱ�֤��ͬһ���߳���ÿ��ʹ�á�
		�˱�־Ӧ�����ڶ����񣬻������ܻ�Ӱ��������ʱ�������� 
		��ע�⵱ǰû�й��������߳��������־ã���������κι��� I/O ���󣬲�����ֹ�����̡߳�

		WT_EXECUTELONGFUNCTION--�ص���������ִ��һ����ʱ��ĵȴ��� �˱�־���԰������������Ӧ����һ�����̵߳�ϵͳ��

		WT_TRANSFER_IMPERSONATION--�ص�������ʹ���ڵ�ǰ�ķ������ƣ�����һ�����̻���ģ�����ơ�
		���δָ���˱�־����ص�������ִ�н������ơ�

	*/

	namespace thread 
	{
		namespace detail
		{
			struct ThreadParamBase
			{
				// ���ú���ָ�룬����virtual function
				typedef void (*CallbackFuncPtr)(ThreadParamBase *, BOOLEAN);
				CallbackFuncPtr callback_;

				ThreadParamBase(CallbackFuncPtr callback)
					: callback_(callback)
				{}

				virtual ~ThreadParamBase()
				{}

				void Invoke()
				{
					callback_(this, FALSE);
				}
				void Invoke(BOOLEAN wait)
				{
					callback_(this, wait);
				}
			};
			template<typename HandlerT>
			struct ThreadParam
				: public ThreadParamBase
			{
				HandlerT handler_;

				ThreadParam(const HandlerT &handler)
					: ThreadParamBase(&ThreadParam<HandlerT>::Call)
					, handler_(handler)
				{}

			private:
				static void Call(ThreadParamBase *param, BOOLEAN wait)
				{
					ThreadParam<HandlerT> *pThis = static_cast<ThreadParam<HandlerT> *>(param);
					pThis->handler_(wait);
				}
			};
		}

		enum { MAX_THREADS = 10 };

		// ��������̳߳�����
		void inline SetThreadPoolMaxThread(DWORD dwFlag, DWORD dwThreads = MAX_THREADS)
		{
			WT_SET_MAX_THREADPOOL_THREADS(dwFlag, dwThreads);
		}

		// handler ������
		template<typename HandlerT>
		detail::ThreadParamBase *HandlerAllocate(const HandlerT &handler, ...)
		{
			return new detail::ThreadParam<HandlerT>(handler);
		}

		//----------------------------------------------------------------------------------
		// class QueueWorkItem

		class QueueWorkItemPool
		{
		private:
			std::auto_ptr<detail::ThreadParamBase> callback_;

		public:
			template<typename HandlerT>
			explicit QueueWorkItemPool(const HandlerT &callback)
				: callback_(HandlerAllocate(callback))
			{}

		public:
			bool Call(ULONG nFlags = WT_EXECUTEDEFAULT)
			{
				return TRUE == ::QueueUserWorkItem(&QueueWorkItemPool::WorkerThreadProc, callback_.get(), nFlags);
			}

		private:
			static DWORD WINAPI WorkerThreadProc(LPVOID pvParam)
			{
				detail::ThreadParamBase *p(static_cast<detail::ThreadParamBase *>(pvParam));

				try 
				{
					p->Invoke();
				}
				catch(...) 
				{}

				return 0;
			}

		private:
			QueueWorkItemPool(const QueueWorkItemPool &);
			QueueWorkItemPool &operator=(const QueueWorkItemPool &);
		};



		//----------------------------------------------------------------------------------
		// class QueueTimer

		/*
			WT_EXECUTEDEFAULT				�÷�IO������߳�����������Ŀ
			WT_EXECUTEINIOTHREAD			�����Ҫ��ĳ��ʱ�䷢��һ���첽IO����
			WT_EXECUTEPERSISTENTTHREAD		�����Ҫ��һ����������ֹ���е��߳�������ù�����Ŀ
			WT_EXECUTELONGFUNCTION			�����Ϊ������Ŀ��Ҫ�ܳ�ʱ��������


			WT_EXECUTEDEFAULT
			0x00000000 Ĭ������£��ص��������Ŷӵ��� i/o �����̡߳�

			WT_EXECUTEINTIMERTHREAD
			0x00000020 �ɱ���ļ�ʱ���̵߳��ûص������� �˱�־Ӧ�����ڶ����񣬻������ܻ�Ӱ��������ʱ�������� �ص���������Ϊ APC �Ŷӡ� ����Ӧִ�б����Ȳ�����

			WT_EXECUTEINIOTHREAD
			0x00000001 �ص��������Ŷ���һ�� I/O �����̡߳� ����ú���Ӧִ���̵߳ȴ�����״̬��ʱ����Ӧʹ�ô˱�־�� �ص���������Ϊ APC �Ŷӡ� ����ú���ִ�б����Ȳ�����������ؽ�������������⡣

			WT_EXECUTEINPERSISTENTTHREAD
			0x00000080 �ص��������Ŷӵ���Զ������ֹ���̡߳� �������ܱ�֤��ͬһ�߳���ÿ��ʹ�á� �˱�־Ӧ�����ڶ����񣬻������ܻ�Ӱ��������ʱ�������� ��ע��Ŀǰû�й��������߳��������־���Ȼû�й��������߳̽���ֹ������κι���� I/O ����

			WT_EXECUTELONGFUNCTION
			0x00000010 �ص���������ִ��һ����ʱ��ĵȴ��� �˱�־�ɰ���ϵͳ�������Ƿ���Ӧ����һ���µ��̡߳�

			WT_EXECUTEONLYONCE
			0x00000008 ��ʱ����һ��ֻ������Ϊ�ѷ����ź�״̬�� ��������˴˱�־ �� ��������Ϊ�㡣

			WT_TRANSFER_IMPERSONATION
			0x00000100 �ص�������ʹ�õ�ǰ�ķ������ƣ������ǽ��̻�ģ�����ơ� �����ָ���˱�־����ص�����ִֻ�н������ơ�

		*/
		class QueueTimerPool
		{
			std::auto_ptr<detail::ThreadParamBase> callback_;
			HANDLE newTimer_;

		public:
			template<typename HandlerT>
			explicit QueueTimerPool(const HandlerT &handler)
				: callback_(HandlerAllocate(handler))
				, newTimer_(NULL)
			{}

			~QueueTimerPool()
			{
				if( newTimer_ != NULL )
					Cancel();
			}

		public:
			bool Call(DWORD dwDueTime, DWORD dwPeriod, ULONG nFlags = WT_EXECUTEDEFAULT)
			{
				return TRUE == ::CreateTimerQueueTimer(&newTimer_, NULL, 
					reinterpret_cast<WAITORTIMERCALLBACK>(&QueueTimerPool::WorkerThreadProc), 
					callback_.get(), dwDueTime, dwPeriod, nFlags);
			}

			// �ı䶨ʱ���ĵ���ʱ��͵�������
			bool Change(DWORD dwDueTime, DWORD dwPeriod)
			{
				return TRUE == ::ChangeTimerQueueTimer(NULL, newTimer_, dwDueTime, dwPeriod);
			}

			// ��Ӧ��ΪhCompletionEvent����INVALID_HANDLE_VALUE
			// ����Ǵ��ݵ�NULL����ú������Ϸ���
			// ����Ǵ��ݵ�һ���¼������������Ϸ��أ����ҵ���ʱ�������Ѿ��ŶӵĹ�����Ŀ��ɺ󣬻ᴥ�����¼�(��Ӧ����Ϊ�������¼�)
			bool Cancel(HANDLE hCompletionEvent = NULL)
			{
				if( ::DeleteTimerQueueTimer(NULL, newTimer_, hCompletionEvent) )
				{
					newTimer_ = NULL;
					return true;
				}

				return false;
			}

		private:
			static void WINAPI WorkerThreadProc(PVOID pvParam, BOOL bTimeout)
			{
				detail::ThreadParamBase *p = static_cast<detail::ThreadParamBase *>(pvParam);

				try 
				{
					p->Invoke(static_cast<BOOLEAN>(bTimeout));
				}
				catch(...) 
				{}
			}

		private:
			QueueTimerPool(const QueueTimerPool &);
			QueueTimerPool &operator=(const QueueTimerPool &);
		};



		/*
			Ͷ��һ���첽IO����,��IO��ɶ˿��ϣ��ص�����Ҳ�����̳߳��߳���ִ��

			������Ӧ�ó��򷢳�ĳЩ�첽IO���󣬵���Щ�������ʱ����Ҫ��һ���̳߳�׼��������������ɵ�IO����
			BindIoCompletionCallback���ڲ�����CreateIoCompletionPort������hDevice���ڲ���ɶ˿ڵľ����
			���øú������Ա�֤������һ���߳�ʼ���ڷ�IO����У����豸��ص���ɼ����ص�������̵ĵ�ַ��
			�����������豸��IO�������ʱ����IO�����֪�������ĸ��������Ա��ܹ����������IO����


		*/

		//----------------------------------------------------------------------------------
		// class BindIOCompletionCallBack 

		template<int N>
		class IOCompletionPool
		{
		public:
			typedef std::tr1::function<void(DWORD, DWORD, OVERLAPPED *)> Callback;
			static Callback m_func;

		private:
			IOCompletionPool();
			~IOCompletionPool();
			IOCompletionPool(const IOCompletionPool &);
			IOCompletionPool &operator=(const IOCompletionPool &);

		public:
			static bool Call(Callback callback, HANDLE hBindHandle)
			{
				m_func = callback;

				bool result = false;
				if( ::BindIoCompletionCallback(hBindHandle, &IOCompletionPool::_OverlappedCompletionRoutine, 0) )
				{
					result = true;
				}

				return result;
			}

		private:
			static void WINAPI _OverlappedCompletionRoutine(DWORD dwError, DWORD dwNum, OVERLAPPED *pOverlapped)
			{
				try 
				{
					m_func(dwError, dwNum, pOverlapped);
				}
				catch(...) 
				{
				}
			}
		};
	
		template<int N> typename IOCompletionPool<N>::Callback IOCompletionPool<N>::m_func;




	
		/*
			WT_EXECUTEDEFAULT	0x00000000 
			Ĭ������£��ûص��������Ŷӵ��� i/o �����̡߳�

			WT_EXECUTEINIOTHREAD	0x00000001 
			�ص�������һ�� I/O �����߳����Ŷӡ� ����ú���Ӧ�ȴ� alertable ״̬���߳���ִ�У�Ӧʹ�ô˱�־�� 
			�ص���������Ϊ APC �Ŷӡ� ����ú���ִ�� alertable �ȴ�������������ؽ�������������⡣

			WT_EXECUTEINPERSISTENTTHREAD 0x00000080 
			�ص��������Ŷӵ���Զ������ֹ���̡߳� �����ܱ�֤��ͬһ���߳���ÿ��ʹ�á� �˱�־Ӧ�����ڶ����񣬻������ܻ�Ӱ�������ȴ������� 
			��ע�⵱ǰû�й��������߳��������־õ���Ȼû�й��������̣߳�������κι��� I/O �������ֹ��

			WT_EXECUTEINWAITTHREA 0x00000004 
			�ɱ���ĵȴ��̵߳��ûص������� �˱�־Ӧ�����ڶ����񣬻������ܻ�Ӱ�������ȴ������� 
			�����һ���̻߳�ȡ�������͵��ûص�����ʱ�� UnregisterWait �� UnregisterWaitEx ��������ͼ��ȡ��ͬ�������ͻᷢ��������

			WT_EXECUTELONGFUNCTION	0x00000010 
			�ص���������ִ��һ����ʱ��ĵȴ��� �˱�־���԰������������Ӧ����һ�����̵߳�ϵͳ��

			WT_EXECUTEONLYONCE	0x00000008 
			�ص���������һ�θ��߳̽����ٵȴ��þ���� ����ÿ�εȴ��������֮ǰ�ȴ�������ȡ���������ü�ʱ����

			WT_TRANSFER_IMPERSONATION	0x00000100 
			�ص�������ʹ���ڵ�ǰ�ķ������ƣ�����һ�����̻���ģ�����ơ� ���δָ���˱�־����ص�������ִ�н������ơ�

		*/

		//----------------------------------------------------------------------------------
		// class WaitObjectPool

		class WaitObjectPool
		{
		private:
			std::auto_ptr<detail::ThreadParamBase> callback_;
			HANDLE waitObject_;

		public:
			template<typename HandlerT>
			explicit WaitObjectPool(const HandlerT &handler)
				: callback_(HandlerAllocate(handler))
				, waitObject_(NULL)
			{}

			~WaitObjectPool()
			{
			}

		private:
			WaitObjectPool(const WaitObjectPool &);
			WaitObjectPool &operator=(const WaitObjectPool &);
			

		public:
			bool Call(HANDLE hObject, ULONG dwWait = INFINITE, ULONG nFlags = WT_EXECUTEDEFAULT)
			{
				return TRUE == ::RegisterWaitForSingleObject(&waitObject_, hObject, 
					&WaitObjectPool::WorkerThreadProc, callback_.get(), dwWait, nFlags);
			}	
			// ȡ��
			bool Cancel(HANDLE hCompletion = NULL)
			{
				return TRUE == ::UnregisterWaitEx(waitObject_, hCompletion);
			}


		private:
			static void WINAPI WorkerThreadProc(PVOID pvParam, BOOLEAN bTimerOrWaitFired)
			{
				detail::ThreadParamBase *p(static_cast<detail::ThreadParamBase *>(pvParam));

				try 
				{
					p->Invoke(bTimerOrWaitFired);
				}
				catch(...) 
				{}
			}
		};

	}

}

#endif //