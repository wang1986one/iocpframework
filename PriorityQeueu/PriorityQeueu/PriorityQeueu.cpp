// PriorityQeueu.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Queue.h"
#include "../../include/Timer/Timer.hpp"


void Priority(const async::iocp::AsyncResultPtr &result, size_t priority)
{
	std::cout << "Priority: " << priority << std::endl;
}


void TimerHanlde(const async::iocp::AsyncResultPtr &result, const async::timer::TimerPtr &timer, HandlerPrority &priQueue)
{
	priQueue.Add(priQueue.Wrap(9, std::tr1::bind(&Priority, std::tr1::placeholders::_1, 9)));
	priQueue.Add(priQueue.Wrap(9, std::tr1::bind(&Priority, std::tr1::placeholders::_1, 4)));

	timer->Cancel();
}






int _tmain(int argc, _TCHAR* argv[])
{
	async::iocp::IODispatcher ioService(1);

	HandlerPrority priQueue(ioService);

	HandlerPrority::WrappedHandler handler = priQueue.Wrap(0, std::tr1::bind(&Priority, std::tr1::placeholders::_1, 0));
	priQueue.Add(handler);
	
	priQueue.Add(priQueue.Wrap(5, std::tr1::bind(&Priority, std::tr1::placeholders::_1, 5)));
	priQueue.Add(priQueue.Wrap(2, std::tr1::bind(&Priority, std::tr1::placeholders::_1, 2)));
	priQueue.Add(priQueue.Wrap(8, std::tr1::bind(&Priority, std::tr1::placeholders::_1, 8)));
	priQueue.Add(priQueue.Wrap(1, std::tr1::bind(&Priority, std::tr1::placeholders::_1, 1)));


	async::timer::TimerPtr timer(new async::timer::Timer(ioService, 2 * 1000, 0));
	timer->BeginWait(std::tr1::bind(&TimerHanlde, std::tr1::placeholders::_1, std::tr1::cref(timer), std::tr1::ref(priQueue)));


	system("pause");
	priQueue.ExcuteAll();

	system("pause");
	return 0;
}

