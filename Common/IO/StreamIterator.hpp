#ifndef __IO_FASTSTREAM_ITERATOR_HPP
#define __IO_FASTSTREAM_ITERATOR_HPP



namespace DataStructure
{

	//------------------------------------------------------------------------------
	// class FastOStreamIterator

	template<typename StreamT, typename T>
	class FastOStreamIterator
		: public std::iterator<std::output_iterator_tag, void, void, void, void>
	{
		typedef StreamT					StreamType;

	private:
		StreamType *stream_;
		const char *delim_;

	public:
		FastOStreamIterator()
			: stream_(NULL)
			, delim_(NULL)
		{}
		explicit FastOStreamIterator(StreamType &s, const char *delim = NULL)
			: stream_(&s)
			, delim_(delim)
		{}


	public:
		FastOStreamIterator &operator++()
		{
			return *this;
		}
		FastOStreamIterator &operator++(int)
		{
			return *this;
		}

		FastOStreamIterator &operator*()
		{
			return *this;
		}

		void operator=(const T &val)
		{
			*stream_ << val;

			if( delim_ != NULL )
				*stream_ << delim_;
		}

		const StreamType *GetStream() const
		{
			return stream_;
		}
	};

	template<typename StreamT, typename T>
	inline bool operator==(const FastOStreamIterator<StreamT, T> &lhs, const FastOStreamIterator<StreamT, T> &rhs)
	{
		return lhs.GetStream() == rhs.GetStream();
	}

	template<typename StreamT, typename T>
	inline bool operator!=(const FastOStreamIterator<StreamT, T> &lhs, const FastOStreamIterator<StreamT, T> &rhs)
	{
		return !(lhs == rhs);
	}



	//------------------------------------------------------------------------------
	// class FastIStreamIterator

	template<typename StreamT, typename T>
	class FastIStreamIterator
		: public std::iterator<std::input_iterator_tag, T, std::ptrdiff_t, const T *, const T &>
	{
		typedef StreamT		StreamType;

	private:
		StreamType *stream_;
		T value_;
		bool alreayRead_;

	public:
		FastIStreamIterator()
			: stream_(NULL)
			, value_(0)
			, alreayRead_(false)
		{}

		explicit FastIStreamIterator(StreamType &s)
			: stream_(&s)
			, value_(0)
			, alreayRead_(false)
		{}

	public:
		FastIStreamIterator &operator++()
		{
			_GetNext();
			return *this;
		}
		FastIStreamIterator &operator++(int)
		{
			FastIStreamIterator tmp;
			_GetNext();

			return tmp;
		}

		const T &operator*()
		{
			if( !alreayRead_ )
				_GetNext();

			return value_;
		}

		const T *operator->()
		{
			return &(operator*());
		}

		const StreamType *GetStream() const
		{
			return stream_;
		}

	private:
		void _GetNext()
		{
			if( !(*stream_ >> value_) )
				stream_ = NULL;

			alreayRead_ = true;
		}
	};

	template<typename StreamT, typename T>
	inline bool operator==(const FastIStreamIterator<StreamT, T> &lhs, const FastIStreamIterator<StreamT, T> &rhs)
	{
		return lhs.GetStream() == rhs.GetStream();
	}

	template<typename StreamT, typename T>
	inline bool operator!=(const FastIStreamIterator<StreamT, T> &lhs, const FastIStreamIterator<StreamT, T> &rhs)
	{
		return !(lhs == rhs);
	}
}






#endif