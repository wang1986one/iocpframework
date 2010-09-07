#ifndef __IOCP_CALLBACK_HPP
#define __IOCP_CALLBACK_HPP


namespace async
{
	namespace iocp
	{

		// --------------------------------------
		// class CallbackBase

		struct CallbackBase
		{
			// 利用函数指针，避免virtual function
			typedef void (*CallbackFuncPtr)(CallbackBase *);
			CallbackFuncPtr callback_;

			CallbackBase(CallbackFuncPtr callback)
				: callback_(callback)
			{}

			void Invoke()
			{
				callback_(this);
			}
		};

		template<typename HandlerT>
		struct CallbackOperate
			: public CallbackBase
		{
			typedef CallbackOperate<HandlerT>	ClassType;

			HandlerT handler_;

			CallbackOperate(const HandlerT &handler)
				: CallbackBase(&CallbackOperate<HandlerT>::Call)
				, handler_(handler)
			{}

		private:
			static void Call(CallbackBase *param)
			{
				ClassType *pThis = static_cast<ClassType *>(param);
				pThis->handler_();
			}
		};
	}
}



#endif