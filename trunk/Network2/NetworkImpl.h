#ifndef __NETWORK_IMPL_HPP
#define __NETWORK_IMPL_HPP


#include "../include/network/Socket.hpp"
#include "../include/MultiThread/Thread.hpp"


//#include <Comm/Allocator/ContainerAllocator.hpp>


using namespace async;
using namespace async::network;



class NetworkImpl
{
public:
	// ������
	typedef async::thread::CAutoLock<async::thread::CAutoCriticalSection>	LockType;
	// ����Զ������
	typedef std::map<SOCKET, SOCKADDR_IN/*, std::less<SOCKET>, ContainerAllocator<std::pair<SOCKET, SOCKADDR_IN>>*/>	ClientSockets;
	
	// һ��Ͷ��Accept����
	enum { MAX_ACCEPT = 10 };

private:
	IODispatcher &io_;
	SocketPtr acceptor_;
	
	ClientSockets		remotes_;
	mutable async::thread::CAutoCriticalSection mutex_;

	// Ͷ��Accept�߳�
	async::thread::ThreadImplEx acceptThread_;

public:
	explicit NetworkImpl(IODispatcher &io);
	~NetworkImpl();


public:
	void Start(u_short);
	void Stop();

	void OnAccept(const AsyncResultPtr &);
	void OnRecv(const AsyncResultPtr &);
	void OnSend(const AsyncResultPtr &);

	void Send(const SocketBufferPtr &);


	size_t GetClientSize() const;

private:
	DWORD _ThreadAccept();

	//size_t _ParseProtocol(const SocketBufferPtr &buffer);

	// ����Զ������
	void _AddRemote(const SocketPtr &, const SOCKADDR_IN &);
	// �Ƴ�Զ������
	void _RemoveRemote(const SocketPtr &);

};


#endif