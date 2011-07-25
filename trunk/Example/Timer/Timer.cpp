// Timer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../include/Timer/Timer.hpp"



void AsyncWait(async::timer::Timer &timer)
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


void AsyncWait2()
{
	std::cout << "AsyncWait2" << std::endl;
}


void AsyncWait3(async::timer::TimerPtr &timer)
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

void AsyncWait4()
{
	std::cout << "AsyncWait4" << std::endl;
}


// Debug Memory Info
void DebugAllocSize(LONG allocCount, size_t size)
{
	std::cout << "Alloc " << allocCount << ": " << size << std::endl;
}

void DebugDeallocSize(LONG deallocCount, size_t size)
{
	std::cout << "Dealloc " << deallocCount << ": " << size << std::endl;
}



int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	{
		async::iocp::IODispatcher io;
		
		try
		{
			{
				//async::timer::Timer timer(io, 2000, 0);
				//timer.BeginWait(std::tr1::bind(&AsyncWait, std::tr1::ref(timer)));

				async::timer::Timer timer2(io, 4000, 0, std::tr1::bind(&AsyncWait2));
				timer2.BeginWait();

				system("pause");
			}
			

			{
				async::timer::TimerPtr timer3(new async::timer::Timer(io, 2000, 0));
				timer3->BeginWait(std::tr1::bind(&AsyncWait3, std::tr1::ref(timer3)));
			
				system("pause");
			}

			async::timer::Timer timer4(io, 500, 0);
			timer4.BeginWait(std::tr1::bind(&AsyncWait4));

			system("pause");
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}

	}
	
	

	system("pause");
	return 0;
}

