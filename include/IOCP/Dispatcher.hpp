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
			*	Int2Type		: 常整数映射为类型，由编译期计算出来的结果选择不同的函数，达到静态分派
			*	调用方式			: Int2Type<isPolymorphics>
			*	适用情形			: 有需要根据某个编译期常数调用一个或数个不同的函数
			*					  有必要在编译期实施静态分派
			*
			*	Type2Type		: 利用函数重载机制，模拟template偏特化，利用轻量型型别来传递型别信息
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


		// 获取适合系统的线程数
		inline size_t GetFitThreadNum(size_t perCPU = 2)
		{
			SYSTEM_INFO systemInfo = {0};
			::GetSystemInfo(&systemInfo);

			return perCPU * systemInfo.dwNumberOfProcessors + 2;
		}



		//------------------------------------------------------------------
		// class IODispatcher
		// 完成端口实现

		template<typename AsyncT>
		class IODispatcher
		{
			typedef AsyncT					AsyncType;

		public:
			// 线程容器类型
			typedef std::vector<HANDLE>		Threads;
			typedef Threads::iterator		ThreadsIter;

		private:
			// iocp Handle
			CIOCP iocp_;
			// 线程容器
			std::vector<HANDLE>	threads_;

		public:
			explicit IODispatcher(size_t numThreads = GetFitThreadNum());
			~IODispatcher();

		public:
			// 绑定设备到完成端口
			void Bind(HANDLE);
			// 向完成端口投递请求
			template<typename AsyncPtrT>
			void Post(AsyncPtrT &);
			// 当仅不在线程池中时才向调度器中分派
			template<typename AsyncPtrT>
			void Dispatch(AsyncPtrT &);

			// 停止服务
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