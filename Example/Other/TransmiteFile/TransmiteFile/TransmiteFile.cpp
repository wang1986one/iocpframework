// TransmiteFile.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Svr.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	async::iocp::IODispatcher io;
	Server svr(io, 5050);

	svr.Start();

	system("pause");
	svr.Stop();

	return 0;
}

