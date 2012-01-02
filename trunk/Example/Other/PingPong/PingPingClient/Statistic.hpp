#ifndef __STATISTIC_HPP
#define __STATISTIC_HPP

#include "../../../include/Network/TCP.hpp"
#include "../../../include/Timer/Timer.hpp"
#include "../../../include/MultiThread/Lock.hpp"



using namespace async::iocp;
using namespace async::thread;
using namespace async::network;
using namespace async::timer;

class Stats
{
private:
	typedef AutoLock<AutoCriticalSection> AutoLock;
	AutoCriticalSection mutex_;	
	size_t totalWrited_;
	size_t totalRead_;

public:
	Stats()
		: totalWrited_(0)
		, totalRead_(0)
	{}

	void Add(size_t writed, size_t readed)
	{
		AutoLock lock(mutex_);
		totalRead_ += readed;
		totalWrited_ += writed;
	}

	void Print()
	{
		AutoLock lock(mutex_);
		std::cout << totalWrited_ << " bytes Written\n";
		std::cout << totalRead_ << " bytes Readed\n";
	}
};



class Session
	: public std::tr1::enable_shared_from_this<Session>
{
	IODispatcher &io_;
	Tcp::Socket socket_;
	size_t blockSize_;
	size_t readDataLen_;

	char *readData_;
	char *writeData_;

	size_t bytesWritten_;
	size_t bytesRead_;

	size_t unWriteCount_;
	
	Stats &stats_;

public:
	Session(IODispatcher &io, size_t blockSize, Stats &stats)
		: io_(io)
		, blockSize_(blockSize)
		, socket_(io)
		, readDataLen_(0)
		, readData_(new char[blockSize_])
		, writeData_(new char[blockSize_])
		, unWriteCount_(0)
		, bytesWritten_(0)
		, bytesRead_(0)
		, stats_(stats)
	{
		for(size_t i = 0; i != blockSize_; ++i)
			writeData_[i] = static_cast<char>(i % 128);
	}

	~Session()
	{
		stats_.Add(bytesWritten_, bytesRead_);

		delete []readData_;
		delete []writeData_;
	}


public:
	void Start(const IPAddress& ip, u_short port)
	{
		socket_.Open();
		socket_.AsyncConnect(ip, port, 
			std::tr1::bind(&Session::_OnConnect, shared_from_this(), _Error));
	}

	void Stop()
	{
		io_.Post(std::tr1::bind(&Session::_Close, shared_from_this()));
	}

private:
	void _OnConnect(u_long err)
	{
		try
		{
			if( !err )
			{
				bool suc = socket_.SetOption(NoDelay(true));
				if( suc )
				{
					++unWriteCount_;
					AsyncWrite(socket_, Buffer(writeData_, blockSize_), 
						std::tr1::bind(&Session::_OnWrite, shared_from_this(), _Error, _Size));
					socket_.AsyncRead(Buffer(readData_, blockSize_), 
						std::tr1::bind(&Session::_OnRead, shared_from_this(), _Error, _Size));
				}
			}
			else
			{
				socket_.Close();
			}
		}
		catch(std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}


	Tcp::Socket &Socket()
	{
		return socket_;
	}

private:
	void _OnRead(u_long err, u_long size)
	{
		try
		{
			if( !err )
			{
				bytesRead_ += size;
				readDataLen_ = size;
				++unWriteCount_;

				if( unWriteCount_ == 1 )
				{
					std::swap(readData_, writeData_);
					AsyncWrite(socket_, Buffer(writeData_, readDataLen_), 
						std::tr1::bind(&Session::_OnWrite, shared_from_this(), _Error, _Size));
					socket_.AsyncRead(Buffer(readData_, blockSize_), 
						std::tr1::bind(&Session::_OnRead, shared_from_this(), _Error, _Size));
				}
			}
		}
		catch(const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}

	}	

	void _OnWrite(u_long err, u_long size)
	{
		if( !err )
		{

			bytesWritten_ += size;
			--unWriteCount_;
			try
			{
				if( unWriteCount_ == 1 )
				{
					std::swap(readData_, writeData_);

					AsyncWrite(socket_, Buffer(writeData_, readDataLen_), 
						std::tr1::bind(&Session::_OnWrite, shared_from_this(), _Error, _Size));
					socket_.AsyncRead(Buffer(readData_, blockSize_), 
						std::tr1::bind(&Session::_OnRead, shared_from_this(), _Error, _Size));
				}
			}
			catch(std::exception &e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}

	void _Close()
	{
		socket_.Shutdown(SD_BOTH);
		socket_.Close();
	}
};

typedef std::tr1::shared_ptr<Session> SessionPtr;


class Client
{
private:
	IODispatcher &io_;
	Timer timer_;

	std::list<SessionPtr> sessions_;
	Stats stats_;

public:
	Client(IODispatcher &io, const IPAddress &ip, u_short port, size_t blockSize, size_t sessionCnt, size_t timeOut)
		: io_(io)
		, timer_(io, timeOut, 0)
	{
		
		for(size_t i = 0; i != sessionCnt; ++i)
		{
			SessionPtr session(new Session(io, blockSize, stats_));
			session->Start(ip, port);
			sessions_.push_back(session);
		}

		timer_.BeginWait(std::tr1::bind(&Client::_OnTimer, this));

	}

	~Client()
	{
		std::for_each(sessions_.begin(), sessions_.end(),
			std::tr1::bind(&Session::Stop, std::tr1::placeholders::_1));
		sessions_.clear();

		Sleep(100);
		stats_.Print();
	}


private:
	void _OnTimer()
	{
		std::for_each(sessions_.begin(), sessions_.end(),
			std::tr1::bind(&Session::Stop, std::tr1::placeholders::_1));
		sessions_.clear();
		timer_.Cancel();
	}
};

#endif