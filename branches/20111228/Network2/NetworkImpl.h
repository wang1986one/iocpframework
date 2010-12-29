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
	// 回调接口
	typedef std::tr1::function<void(const SocketPtr &, const SocketBufferPtr &, const SOCKADDR_IN &)>	AcceptCallback;
	typedef std::tr1::function<void(const SocketPtr &, const SocketBufferPtr &)>						RecvCallback;
	typedef std::tr1::function<void(const SocketPtr &, const SocketBufferPtr &, size_t)>				SendCallback;
	typedef std::tr1::function<void(const SocketPtr &)>													DisconnectCallback;
	typedef std::tr1::function<void(const SocketPtr &, const std::exception &)>							ErrorCallback;

public:
	// 一次投递Accept个数
	enum { MAX_ACCEPT = 1 };

private:
	OverlappedDispatcher &io_;
	SocketPtr acceptor_;

	// 投递Accept线程
	async::thread::ThreadImplEx acceptThread_;

	// 回调接口
	AcceptCallback acceptCallback_;
	RecvCallback recvCallback_;
	SendCallback sendCallback_;
	DisconnectCallback disconnectCallback_;
	ErrorCallback errorCallback_;

public:
	explicit NetworkImpl(OverlappedDispatcher &io);
	~NetworkImpl();


public:
	// 注册回调
	void Register(const AcceptCallback &acceptCallback = NULL, const RecvCallback &RecvCallback = NULL
		, const SendCallback &sendCallback = NULL, const DisconnectCallback &disconnectCallback = NULL
		, const ErrorCallback &errorCallback = NULL);
	// 启动
	void Start(u_short);
	// 停止
	void Stop();

	// 发送数据
	void Send(const SocketPtr &, const SocketBufferPtr &);
	AsyncResultPtr Send(const SocketPtr &, const AsyncResultPtr &, size_t);

	// IOCP回调处理
private:
	void _OnAccept(const AsyncResultPtr &);
	void _OnRecv(const AsyncResultPtr &, const SocketPtr &);
	void _OnSend(const AsyncResultPtr &, const SocketPtr &);
	void _OnDisConnect(const AsyncResultPtr &, const SocketPtr &);
private:
	DWORD _ThreadAccept();
};


#endif