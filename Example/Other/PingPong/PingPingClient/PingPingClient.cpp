// PingPingClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Statistic.hpp"

#include <atlbase.h>
#include <atlconv.h>

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		if( argc != 7 )
		{
			std::cerr << "Usage: Client <host> <port> <threads> <blocksize> "
				<< "<sessions> <time> \n"; 

			system("pause");
			return 1;
		}

		u_long ip = IPAddress::Parse(std::string(CT2A(argv[1])));
		u_short port = _tstoi(argv[2]);
		size_t threads = _tstoi(argv[3]);
		size_t blockSize = _tstoi(argv[4]);
		size_t sessions = _tstoi(argv[5]);
		size_t time = _tstoi(argv[6]);

		IODispatcher io(threads);

		Client cli(io, ip, port, blockSize, sessions, time);
		system("pause");
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	

	return 0;
}

