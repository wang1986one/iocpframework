// Sock4a.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Tunnel.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	iocp::IODispatcher io;

	Server svr(io, 5050, "127.0.0.1", 5051);
	svr.Start();

	system("pause");

	svr.Stop();
	return 0;
}

