// Timer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../include/Timer/Timer.hpp"



void AsyncWait(const async::iocp::AsyncResultPtr &result, async::timer::Timer &timer)
{
	std::cout << "AsyncWait" << std::endl;

	static int count = 0;

	try
	{
		if( count++ == 5 )
			timer.SetTimer(200);
		if( count >= 10 )
			timer.Cancel();
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	
}


void AsyncWait2(const async::iocp::AsyncResultPtr &result)
{
	std::cout << "AsyncWait2" << std::endl;
}


void AsyncWait3(const async::iocp::AsyncResultPtr &result, async::timer::TimerPtr &timer)
{
	static int count = 1000;

	count += 500;

	try
	{
		if( count == 5000 )
			timer->SetTimer(count);
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cout << "AsyncWait3" << std::endl;
}

void AsyncWait4(const async::iocp::AsyncResultPtr &result)
{
	std::cout << "AsyncWait4" << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		async::iocp::IODispatcher io;

		async::timer::Timer timer(io, 2000);
		timer.BeginWait(std::tr1::bind(&AsyncWait, std::tr1::placeholders::_1, std::tr1::ref(timer)));

		async::timer::Timer timer2(io, 4000, std::tr1::bind(&AsyncWait2, std::tr1::placeholders::_1));
		timer2.BeginWait();


		async::timer::TimerPtr timer3(new async::timer::Timer(io, 2000));
		timer3->Wait();
		timer3->BeginWait(std::tr1::bind(&AsyncWait3, std::tr1::placeholders::_1, std::tr1::ref(timer3)));

		std::cin.get();
		async::timer::Timer timer4(io, 500);
		timer4.BeginWait(std::tr1::bind(&AsyncWait4, std::tr1::placeholders::_1));
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	

	system("pause");
	return 0;
}

