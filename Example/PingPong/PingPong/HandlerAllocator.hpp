#ifndef __HANDLER_ALLOCATOR_HPP
#define __HANDLER_ALLOCATOR_HPP


#include <type_traits>


template < size_t SizeT = 1024 >
class HandlerAllocator
{
	typename std::tr1::aligned_storage<SizeT, sizeof(int)>::type storage_;
	volatile long isUsed_;

public:
	HandlerAllocator()
		: isUsed_(0)
	{}

public:
	void *Allocate(size_t size)
	{
		if( ::InterlockedExchangeAdd(&isUsed_, 0) != 1 
			&& size < SizeT )
		{
			::InterlockedExchange(&isUsed_, 1);
			return &storage_;
		}
		else
			return ::operator new(size);
	}

	void Deallocate(void *p)
	{
		if( p == &storage_ )
			::InterlockedExchange(&isUsed_, 0);
		else
			::operator delete(p);
	}

private:
	HandlerAllocator(const HandlerAllocator &);
	HandlerAllocator &operator=(const HandlerAllocator &);
};

typedef HandlerAllocator<> DefHandlerAlloc;


template < typename HandlerT >
class CustomHandler
{
private:
	DefHandlerAlloc &allocator_;
	HandlerT handler_;

public:
	CustomHandler(DefHandlerAlloc &alloc, const HandlerT &handler)
		: allocator_(alloc)
		, handler_(handler)
	{}

public:
	template < typename Arg1 >
	void operator()(const Arg1 &arg1)
	{
		handler_(arg1);
	}

	template < typename Arg1, typename Arg2 >
	void operator()(const Arg1 &arg1, const Arg2 &arg2)
	{
		handler_(arg1, arg2);
	}
};


#endif