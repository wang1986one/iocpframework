// PriorityQeueu.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Queue.h"
#include "../../include/Timer/Timer.hpp"


void Priority(size_t priority)
{
	std::cout << "Priority: " << priority << std::endl;
}


void TimerHanlde(const async::iocp::AsyncCallbackBasePtr &result, const async::timer::TimerPtr &timer, HandlerPrority &priQueue)
{
	priQueue.Add(priQueue.Wrap(9, std::tr1::bind(&Priority, 9)));
	priQueue.Add(priQueue.Wrap(9, std::tr1::bind(&Priority, 4)));

	timer->Cancel();
}






int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	async::iocp::IODispatcher ioService(1);

	HandlerPrority priQueue(ioService);

	HandlerPrority::WrappedHandler handler = priQueue.Wrap(0, std::tr1::bind(&Priority, 0));
	priQueue.Add(handler);
	
	priQueue.Add(priQueue.Wrap(5, std::tr1::bind(&Priority, 5)));
	priQueue.Add(priQueue.Wrap(2, std::tr1::bind(&Priority, 2)));
	priQueue.Add(priQueue.Wrap(8, std::tr1::bind(&Priority, 8)));
	priQueue.Add(priQueue.Wrap(1, std::tr1::bind(&Priority, 1)));


	system("pause");
	priQueue.ExcuteAll();

	system("pause");
	return 0;
}

