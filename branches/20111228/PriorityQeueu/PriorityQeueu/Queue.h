#ifndef __QUEUE_HPP
#define __QUEUE_HPP





class HandlerPrority
{
	// forward declare
public:
	typedef std::tr1::function<void()> Callback;

	struct WrappedHandler;

	WrappedHandler Wrap(size_t priority, const async::iocp::AsyncCallbackFunc &callback);

private:
	class QueueHandler;


private:
	async::iocp::OverlappedDispatcher &io_;
	std::priority_queue<QueueHandler> handlers_;

public:
	explicit HandlerPrority(async::iocp::OverlappedDispatcher &io)
		: io_(io)
	{}

public:
	void Add(const WrappedHandler &handler);
	void ExcuteAll();

};


template<typename FuncT>
void HanlderInvoke(const FuncT &func, HandlerPrority::WrappedHandler *h)
{
	h->queue_.Add(h->priority_, func);
}




struct HandlerPrority::WrappedHandler
{
	//using async::iocp::AsyncResultPtr;
	//using async::iocp::AsyncResult;
	//using async::iocp::nothing;

	HandlerPrority &queue_;
	size_t priority_;
	async::iocp::AsyncResultPtr result_;

public:
	WrappedHandler(HandlerPrority &queue, size_t priority, const async::iocp::AsyncCallbackFunc &callback)
		: queue_(queue)
		, priority_(priority)
		, result_(new async::iocp::AsyncResult(async::iocp::nothing, async::iocp::nothing, async::iocp::nothing, async::iocp::nothing, callback))
	{}


public:
	const async::iocp::AsyncResultPtr &operator()() const
	{
		return result_;
	}
	/*template<typename Arg1>
	const AsyncResultPtr & operator()(const Arg1 &arg1)
	{
		return handler_(arg1);
	}
	template<typename Arg1, typename Arg2>
	const AsyncResultPtr & operator()(const Arg1 &arg1, const Arg2 &arg2)
	{
		return handler_(arg1, arg2);
	}*/

};


inline HandlerPrority::WrappedHandler HandlerPrority::Wrap(size_t priority, const async::iocp::AsyncCallbackFunc &callback)
{
	return HandlerPrority::WrappedHandler(*this, priority, callback);
}



class HandlerPrority::QueueHandler
{
private:
	size_t priority_;
	async::iocp::AsyncResultPtr result_;
	async::iocp::OverlappedDispatcher &io_;

public:
	QueueHandler(async::iocp::OverlappedDispatcher &io, size_t priority, const async::iocp::AsyncResultPtr &result)
		: priority_(priority)
		, result_(result)
		, io_(io)
	{}

	QueueHandler &operator=(const QueueHandler &rhs)
	{
		if( &rhs != this )
		{
			priority_	= rhs.priority_;
			result_		= rhs.result_;
			io_			= rhs.io_;
		}

		return *this;
	}

public:
	void Excute()
	{
		io_.Post(result_);
	}

	friend bool operator<(const QueueHandler &lhs, const QueueHandler &rhs)
	{
		return lhs.priority_ < rhs.priority_;
	}
};



#endif