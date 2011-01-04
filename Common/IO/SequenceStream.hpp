#ifndef __IO_SEQUENCE_STREAM_HPP
#define __IO_SEQUENCE_STREAM_HPP




namespace DataStructure
{

	//-------------------------------------------------------------------
	// class SequenceWriter

	template<typename ContainerT>
	class SequenceWriter
	{
	public:
		typedef typename ContainerT::value_type		char_type;

	private:
		typedef ContainerT							ContainerType;


	private:
		ContainerType &container_;

	public:
		explicit SequenceWriter(ContainerType &con)
			: container_(con)
		{}

	private:
		SequenceWriter(const SequenceWriter &);
		SequenceWriter &operator=(const SequenceWriter &);

	public:
		void Write(char_type c)
		{
			container_.push_back(c);
		}

		template<typename IterT>
		void Write(IterT begin, IterT end)
		{
			container_.insert(container_.end(), begin, end);
		}

	public:
		void Reset(ContainerType &con)
		{
			container_ = con;
		}

		bool EndOfStream() const
		{
			return false;
		}
		bool DeviceError() const
		{
			return false;
		}
		bool Good() const
		{
			return !DeviceError();
		}
	};


	//-------------------------------------------------------------------
	// class SequenceReader

	template<typename ContainerT>
	class SequenceReader
	{
	public:
		typedef typename ContainerT::value_type			char_type;

	private:
		typedef ContainerT								ContainerType;
		typedef typename ContainerType::const_iterator	const_iterator;

	private:
		ContainerType &container_;
		const_iterator begin_;
		const_iterator end_;
		bool badFormat_;
		bool eof_;

	public:
		explicit SequenceReader(ContainerType &con)
			: container_(con)
			, begin_(con.begin())
			, end_(con.end())
			, badFormat_(false)
			, eof_(false)
		{}

	private:
		SequenceReader(const SequenceReader &);
		SequenceReader &operator=(const SequenceReader &);

	public:
		bool Read(char_type &c)
		{
			if( begin_ != end_ )
			{
				c = *begin_++;
				return true;
			}
			else
			{
				eof_ = true;
				return false;
			}
		}

		template<typename IterT>
		size_t Read(IterT begin, size_t nCount)
		{
			size_t nRead = nCount;
			while(nCount > 0 && begin_ != end_)
			{
				*begin++ = *begin_++;
				--nCount;
			}

			if( begin_ == end_ && nCount > 0 )
				eof_ = true;

			return nRead - nCount;
		}

	public:
		void Reset(const ContainerType &con)
		{
			container_	= con;
			begin_		= con.begin();
			end_		= con.end();
			eof_		= false;
		}

		void GoBack(int nSize)
		{
			std::advance(begin_, -nSize);
			if( nSize > 0 )
				eof_ = false;
		}

	public:
		bool EndOfStream() const
		{
			return eof_;
		}
		bool DeviceError() const
		{
			return false;
		}
		
		void SetBadFormat(bool b)
		{
			badFormat_ = b;
		}
		bool BadFormat() const
		{
			return badFormat_;
		}

		bool Good() const
		{
			return !DeviceError() && !BadFormat();
		}

	};
}



#endif