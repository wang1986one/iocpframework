#include "stdafx.h"
#include "ConnectionMgr.h"


namespace http
{

	void ConnectionMgr::Start(const ConnectionPtr &c)
	{
		connections_.insert(c);
		c->Start();
	}

	void ConnectionMgr::Stop(const ConnectionPtr &c)
	{
		connections_.erase(c);
		c->Stop();
	}

	void ConnectionMgr::StopAll()
	{
		std::for_each(connections_.begin(), connections_.end(),
			std::tr1::bind(&Connection::Stop, std::tr1::placeholders::_1));

		connections_.clear();
	}
}