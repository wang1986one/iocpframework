#ifndef __NETWORK_IMPL_HPP
#define __NETWORK_IMPL_HPP


#include "../include/network/Socket.hpp"
#include "../include/MultiThread/Thread.hpp"



using namespace async;
using namespace async::network;



class NetworkImpl
{
public:
	// һ��Ͷ��Accept����
	enum { MAX_ACCEPT = 10 };

private:
	IODispatcher &io_;
	SocketPtr acceptor_;

	// Ͷ��Accept�߳�
	async::thread::ThreadImplEx acceptThread_;

public:
	explicit NetworkImpl(IODispatcher &io);
	~NetworkImpl();


public:
	void Start(u_short);
	void Stop();

	void Send(const SocketBufferPtr &);

private:
	void _OnAccept(const AsyncResultPtr &);
	void _OnRecv(const AsyncResultPtr &);
	void _OnSend(const AsyncResultPtr &);

private:
	DWORD _ThreadAccept();
};


#endif