#ifndef __NETWORK_IMPL_HPP
#define __NETWORK_IMPL_HPP


#include "../include/network/Socket.hpp"
#include "../include/MultiThread/Thread.hpp"



using namespace async;
using namespace async::network;


// ------------------------------------------------
// class NetworkImpl

class NetworkImpl
{
	// �ص��ӿ�
	typedef std::tr1::function<void(const SocketPtr &, const SocketBufferPtr &, const SOCKADDR_IN &)>	AcceptCallback;
	typedef std::tr1::function<void(const SocketPtr &, const SocketBufferPtr &)>						RecvCallback;
	typedef std::tr1::function<void(const SocketPtr &, const SocketBufferPtr &, size_t)>				SendCallback;
	typedef std::tr1::function<void(const SocketPtr &)>													DisconnectCallback;
	typedef std::tr1::function<void(const SocketPtr &, const std::exception &)>							ErrorCallback;

public:
	// һ��Ͷ��Accept����
	enum { MAX_ACCEPT = 1 };

private:
	OverlappedDispatcher &io_;
	SocketPtr acceptor_;

	// Ͷ��Accept�߳�
	async::thread::ThreadImplEx acceptThread_;

	// �ص��ӿ�
	AcceptCallback acceptCallback_;
	RecvCallback recvCallback_;
	SendCallback sendCallback_;
	DisconnectCallback disconnectCallback_;
	ErrorCallback errorCallback_;

public:
	explicit NetworkImpl(OverlappedDispatcher &io);
	~NetworkImpl();


public:
	// ע��ص�
	void Register(const AcceptCallback &acceptCallback = NULL, const RecvCallback &RecvCallback = NULL
		, const SendCallback &sendCallback = NULL, const DisconnectCallback &disconnectCallback = NULL
		, const ErrorCallback &errorCallback = NULL);
	// ����
	void Start(u_short);
	// ֹͣ
	void Stop();

	// ��������
	void Send(const SocketPtr &, const SocketBufferPtr &);
	AsyncResultPtr Send(const SocketPtr &, const AsyncResultPtr &, size_t);

	// IOCP�ص�����
private:
	void _OnAccept(const AsyncResultPtr &);
	void _OnRecv(const AsyncResultPtr &, const SocketPtr &);
	void _OnSend(const AsyncResultPtr &, const SocketPtr &);
	void _OnDisConnect(const AsyncResultPtr &, const SocketPtr &);
private:
	DWORD _ThreadAccept();
};


#endif