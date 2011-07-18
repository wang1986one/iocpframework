#ifndef __IOCP_ASYNC_RESULT_HPP
#define __IOCP_ASYNC_RESULT_HPP



namespace async
{
	namespace iocp
	{


		
		//---------------------------------------------------------------------------
		// class AsyncCallbackBase

		struct AsyncCallbackBase;
		typedef Pointer<AsyncCallbackBase>	AsyncCallbackBasePtr;


		struct AsyncCallbackBase
			: public OVERLAPPED
		{
			AsyncCallbackBase()
			{
				RtlZeroMemory((OVERLAPPED *)this, sizeof(OVERLAPPED));
			}

			virtual ~AsyncCallbackBase()
			{

			}

			virtual void Invoke(u_long size, u_long error) = 0;

			template<typename KeyT, typename OverlappedT>
			static void Call(KeyT *key, OverlappedT *overlapped, u_long size, u_long error)
			{
				AsyncCallbackBasePtr p(static_cast<AsyncCallbackBase *>(overlapped));
				
				p->Invoke(size, error);
			}
		};


		// 回调接口
		
		typedef std::tr1::function<void(u_long size, u_long error)> CallbackType;
		static std::tr1::_Ph<1> _Size;
		static std::tr1::_Ph<2>	_Error;


		//---------------------------------------------------------------------------
		// class AsyncCallback

		struct AsyncCallback
			: public AsyncCallbackBase
		{
			CallbackType handler_;

			explicit AsyncCallback(const CallbackType &callback)
				: handler_(callback)
			{}
			virtual ~AsyncCallback()
			{

			}

		public:
			virtual void Invoke(u_long size, u_long error)
			{
				handler_(size, error);
			}

		private:
			AsyncCallback();
		};

		inline AsyncCallbackBase *MakeAsyncCallback(const CallbackType &handler)
		{
			return ObjectAllocate<AsyncCallback>(handler);
		}

	}
}



#endif