// BlockClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../../include/Network/TCP.hpp"


using namespace async::network;

int _tmain(int argc, _TCHAR* argv[])
{

	try
	{
		OverlappedDispatcher io(1);

		Tcp::StreamSocket sock(io, Tcp::V4());
		sock.Connect(AF_INET, IPAddress::Parse("192.168.1.103"), 5050);

		char request[1024] = {0};
		std::cin.getline(request, 1024);

		size_t requestLen = ::strlen(request);
		
		AutoBufferPtr buf(MakeBuffer(request, requestLen));
		size_t sendLen = sock.Write(ConstBuffer(buf->data(), buf->size()));

		char reply[1024] = {0};

		AutoBufferPtr recvBuf(MakeBuffer(reply));
		size_t replyLen = sock.Read(MutableBuffer(recvBuf->data(), recvBuf->capacity()));
		recvBuf->resize(replyLen);

		std::cout.write(reply, replyLen);
		std::cout << "\n";

	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	

	system("pause");
	return 0;
}

