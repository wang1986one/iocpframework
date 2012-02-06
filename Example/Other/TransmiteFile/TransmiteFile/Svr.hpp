#ifndef __SERVICE_HPP
#define __SERVICE_HPP



#include "../../../../include/Network/TCP.hpp"
#include "../../../../include/Timer/Timer.hpp"

using namespace async;
using namespace async::network;
using namespace async::timer;

volatile long g_ClientNum = 0;


class Session
	: public std::tr1::enable_shared_from_this<Session>
{
	struct header
	{
		char name[MAX_PATH];
		size_t size;
	};

private:
	Tcp::Socket socket_;
	std::tr1::array<char, 32> buf_;

public:
	explicit Session(const SocketPtr &sock)
		: socket_(sock)
	{
		socket_.IOControl(NonBlockingIO(true));
		::InterlockedIncrement(&g_ClientNum);
	}
	~Session()
	{
		//Stop();
		::InterlockedDecrement(&g_ClientNum);
	}


public:
	Tcp::Socket& GetSocket()
	{
		return socket_;
	}

	void Start()
	{
		try
		{		
			HANDLE hFile = ::CreateFile(L"C:/Console.zip", GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
				NULL);
			LARGE_INTEGER size = {0};
			::GetFileSizeEx(hFile, &size);
	
			

			std::tr1::shared_ptr<header> buf(new header);
			strcpy(buf->name, "Console.zip");
			buf->size = (size_t)size.QuadPart;

			
			TRANSMIT_FILE_BUFFERS transmitBuf = {0};
			transmitBuf.Head = buf.get();
			transmitBuf.HeadLength = sizeof(header);
			
			AsyncCallbackBasePtr result(MakeAsyncCallback<iocp::AsyncCallback>(
				std::tr1::bind(&Session::_HandleWrite, shared_from_this(), _Size, _Error, hFile, buf)
				));
			::TransmitFile(socket_.Get()->GetHandle(), hFile, 0, 0, result.Get(), &transmitBuf, TF_USE_KERNEL_APC);
			result.Release();
			

		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void Stop()
	{
		socket_.Close();
	}

private:
	void _HandleRead(u_long bytes, u_long error)
	{
		try
		{
			if( bytes == 0 )
			{
				socket_.AsyncDisconnect(std::tr1::bind(&Session::_DisConnect, shared_from_this()));
				return;
			}

		
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _HandleWrite(u_long bytes, u_long error, HANDLE hFile, const std::tr1::shared_ptr<header> &head)
	{
		::CloseHandle(hFile);
		assert(bytes == head->size + sizeof(header));
	}

	void _DisConnect()
	{
		Stop();
	}
};

typedef std::tr1::shared_ptr<Session> SessionPtr;




// 定制自己的工厂
namespace async
{
	namespace iocp
	{
		template<>
		struct ObjectFactory< Session >
		{
			typedef async::memory::FixedMemoryPool<true, sizeof(Session)>	PoolType;
			typedef ObjectPool< PoolType >									ObjectPoolType;
		};
	}
}


template<typename T>
struct NoneDeletor
{
	void operator()(T *)
	{}
};


inline SessionPtr CreateSession(const SocketPtr &socket)
{
	return SessionPtr(ObjectAllocate<Session>(socket), &ObjectDeallocate<Session>);
	//return SessionPtr(new Session(io, socket));
}



class Server
{
private:
	IODispatcher &io_;
	Tcp::Accpetor acceptor_;
	std::auto_ptr<Timer> timer_;

public:
	Server(IODispatcher &io, short port)
		: io_(io)
		, acceptor_(io_, Tcp::V4(), port, INADDR_ANY)
	{}

	~Server()
	{
		//_StopServer();
	}

public:
	void Start()
	{
		timer_.reset(new Timer(io_, 2000, 0, std::tr1::bind(&Server::_OnTimer, this)));
		timer_->AsyncWait();

		_StartAccept();
	}

	void Stop()
	{
		_StopServer();
	}

private:
	void _StartAccept()
	{		
		try
		{
			acceptor_.AsyncAccept(0, 
				std::tr1::bind(&Server::_OnAccept, this, std::tr1::placeholders::_1, std::tr1::placeholders::_2/*std::tr1::cref(acceptSock)*/));
		} 
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void _StopServer()
	{
		acceptor_.Close();
	}

	void _OnTimer()
	{
		std::cout << "Clients: " << g_ClientNum << std::endl;
	}

private:
	void _OnAccept(u_long error, const SocketPtr &acceptSocket)
	{
		if( error != 0 )
			return;

		try
		{
			SessionPtr session(CreateSession(acceptSocket));
			session->Start();

			_StartAccept();
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
};



#endif