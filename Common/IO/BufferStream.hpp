#ifndef __IO_BUFFER_STREAM_HPP
#define __IO_BUFFER_STREAM_HPP


#pragma warning(push)
#pragma warning(disable : 4996)		// std::copy

namespace DataStructure
{
	//-----------------------------------------------------------------
	// class BufferWriter

	template<typename CharT, size_t _BufSize, typename WriterT>
	class BufferWriter
		: public WriterT
	{
	public:
		typedef CharT			char_type;

	private:
		typedef WriterT			BaseType;

	private:
		char_type buffer_[_BufSize];
		size_t pos_;

	public:
		BufferWriter()
			: pos_(0)
		{}
		template<typename T>
		explicit BufferWriter(T &t)
			: BaseType(t)
			, pos_(0)
		{}
		template<typename T>
		explicit BufferWriter(const T &t)
			: BaseType(t)
			, pos_(0)
		{}

		template<typename T1, typename T2>
		BufferWriter(T1 &val1, T2 &val2)
			: BaseType(val1, val2)
			, pos_(0)
		{}
		template<typename T1, typename T2>
		BufferWriter(const T1 &val1, const T2 &val2)
			: BaseType(val1, val2)
			, pos_(0)
		{}

		template<typename T1, typename T2, typename T3>
		BufferWriter(T1 &val1, T2 &val2, T3 &val3)
			: BaseType(val1, val2, val3)
			, pos_(0)
		{}
		template<typename T1, typename T2, typename T3>
		BufferWriter(const T1 &val1, const T2 &val2, const T3 &val3)
			: BaseType(val1, val2, val3)
			, pos_(0)
		{}

		~BufferWriter()
		{
			if( pos_ != 0 )
				Flush();
		}

	private:
		BufferWriter(const BufferWriter &);
		BufferWriter &operator=(const BufferWriter &);

	public:
		void Write(CharT c)
		{
			if( pos_ == _BufSize )
				Flush();

			buffer_[pos_++] = c;
		}

		template<typename IterT>
		void Write(IterT begin, IterT end)
		{
			typedef typename std::iterator_traits<IterT>::iterator_category	Type;

			_WriteImpl(begin, end, Type());
		}

		// Additional Function
		void Flush()
		{
			BaseType::Write(buffer_, buffer_ + pos_);
			pos_ = 0;
		}


	private:
		// 针对random access iterator
		template<typename IterT>
		void _WriteImpl(IterT begin, IterT end, std::random_access_iterator_tag)
		{
			assert(begin < end);

			while(begin < end)
			{
				if( pos_ == _BufSize )
					Flush();

				// 寻找最小区间
				size_t nWrite = std::min(static_cast<size_t>(std::distance(begin, end)), _BufSize - pos_);
				// 复制数据
				std::copy(begin, begin + nWrite, &buffer_[pos_]);

				pos_ += nWrite;
				std::advance(begin, nWrite);
			}
		}

		// 泛化一般形式
		template<typename IterT>
		void _WriteImpl(IterT begin, IterT end, ...)
		{
			std::for_each(begin, end, std::mem_fun(&BufferWriter::Write));
		}
	};



	//-----------------------------------------------------------------
	// class BufferReader

	template<typename CharT, size_t _BufSize, typename ReaderT>
	class BufferReader
		: public ReaderT
	{
	public:
		typedef CharT			char_type;
	private:
		typedef ReaderT			BaseType;

	private:
		char_type buffer_[_BufSize];
		size_t pos_;
		size_t end_;
		bool eofphs_;			// end of physical stream
		bool eof_;				// end of buffered data


	public:
		BufferReader()
		{}

		template<typename T>
		explicit BufferReader(T &val)
			: BaseType(val)
			, pos_(0)
			, end_(0)
			, eofphs_(false)
			, eof_(false)
		{}
		template<typename T>
		explicit BufferReader(const T &val)
			: BaseType(val)
			, pos_(0)
			, end_(0)
			, eofphs_(false)
			, eof_(false)
		{}

		template<typename T1, typename T2>
		BufferReader(T1 &val1, T2 &val2)
			: BaseType(val1, val2)
			, pos_(0)
			, end_(0)
			, eofphs_(false)
			, eof_(false)
		{}
		template<typename T1, typename T2>
		BufferReader(const T1 &val1, const T2 &val2)
			: BaseType(val1, val2)
			, pos_(0)
			, end_(0)
			, eofphs_(false)
			, eof_(false)
		{}

		template<typename T1, typename T2, typename T3>
		BufferReader(T1 &val1, T2 &val2, T3 &val3)
			: BaseType(val1, val2, val3)
			, pos_(0)
			, end_(0)
			, eofphs_(false)
			, eof_(false)
		{}
		template<typename T1, typename T2, typename T3>
		BufferReader(const T1 &val1, const T2 &val2, const T3 &val3)
			: BaseType(val1, val2, val3)
			, pos_(0)
			, end_(0)
			, eofphs_(false)
			, eof_(false)
		{}

	private:
		BufferReader(const BufferReader &);
		BufferReader &operator=(const BufferReader &);


	public:
		bool Read(char_type &c)
		{
			if( pos_ == end_ && _Refill() == false )
			{
				eof_ = true;
				return false;
			}

			c = buffer_[pos_++];
			pos_ %= _BufSize;

			return true;
		}

		template<typename IterT>
		size_t Read(IterT begin, size_t nCount)
		{
			typedef typename std::iterator_traits<IterT>::iterator_category Type;
			return _ReadImpl(begin, nCount, Type());
		}

	public:
		void GoBack(int nPos)
		{
			pos_ -= nPos;
			if( pos_ < 0 )
				pos_ += _BufSize;

			if( nPos > 0 )
				eof_ = false;
		}

		bool EndOfStream() const
		{
			return eof_;
		}

		bool Good() const
		{
			return !DeviceError() && !BadFormat();
		}

	private:
		// 针对Random Access特化
		template<typename IterT>
		size_t _ReadImpl(IterT begin, size_t nCount, std::random_access_iterator_tag)
		{
			size_t nRead = nCount;
			while(nCount > 0 && (pos_ != end_ || !eofphs_))
			{
				if( pos_ == end_ && _Refill() == false )
					break;

				size_t nChunk = end_ > 0 ? end_ - pos_ : _BufSize - pos_;
				nChunk = std::min(nChunk, nCount);

				begin = std::copy(&buffer_[pos_], &buffer_[pos_ + nChunk], begin);
				pos_ += nChunk;
				pos_ %= _BufSize;
				nCount -= nChunk;
			}

			if( nCount > 0 && eofphs_ )
				eof_ = true;

			return nRead - nCount;
		}

		template<typename IterT>
		size_t _ReadImpl(IterT begin, size_t nCount, ...)
		{
			size_t nRead = nCount;
			while(nCount > 0 && (pos_ != end_ || !eofphs_))
			{
				if( pos_ == end_ && _Refill() == false )
					break;

				*begin++ = buffer_[pos_++];
				pos_ = _BufSize;
				--nCount;
			}

			if( nCount > 0 && eofphs_ )
				eof_ = true;

			return nRead - nCount;
		}


		bool _Refill()
		{
			if( eofphs_ )
				return false;

			size_t nRead = BaseType::Read(&buffer_[end_], _BufSize);
			end_ += nRead;
			end_ %= _BufSize;

			if( nRead == _BufSize )
				return true;
			else
			{
				if( BaseType::EndOfStream() && !BaseType::DeviceError() )
				{
					eofphs_ = true;
					return nRead > 0;
				}
				else if( !BaseType::DeviceError() )
					return true;
				else
					return false;
			}
		}
	};
}



#pragma warning(pop)


#endif