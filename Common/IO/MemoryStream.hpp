#ifndef __IO_MEMORY_HPP
#define __IO_MEMORY_HPP

#pragma warning(push)
#pragma warning(disable : 4996)		// std::copy

namespace DataStructure
{

	//----------------------------------------------------------
	// class UnCheckedMemoryWrite

	template<typename CharT>
	class UnCheckedMemoryWriter
	{
	public:
		typedef CharT		char_type;

	private:
		char_type *buf_;

	public:
		template<size_t _N>
		explicit UnCheckedMemoryWriter(char_type (&arr)[_N])
			: buf_(arr)
		{}

		explicit UnCheckedMemoryWriter(char_type *buf)
			: buf_(buf)
		{}

	private:
		UnCheckedMemoryWriter(const UnCheckedMemoryWriter &);
		UnCheckedMemoryWriter &operator=(const UnCheckedMemoryWriter &);

	public:
		void Write(char_type c)
		{
			*buf_++ = c;
		}

		template<typename IterT>
		void Write(IterT begin, IterT end)
		{
			assert(end > begin);
			buf_ = std::copy(begin, end, buf_);
		}

	public:
		void Reset(char_type *buf)
		{
			buf_ = buf;
		}
		char_type *Position() const
		{
			return buf_;
		}

		// State and Error detection
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


	//-------------------------------------------------------------
	// class UnCheckedMemoryReader

	template<typename CharT>
	class UnCheckedMemoryReader
	{
	public:
		typedef CharT		char_type;

	private:
		char_type const *buf_;
		bool badFormat_;

	public:
		template<size_t _N>
		explicit UnCheckedMemoryReader(char_type (&arr)[_N])
			: buf_(arr)
		{}

		explicit UnCheckedMemoryReader(char_type const *buf)
			: buf_(buf)
			, badFormat_(false)
		{}

	private:
		UnCheckedMemoryReader(const UnCheckedMemoryReader &);
		UnCheckedMemoryReader &operator=(const UnCheckedMemoryReader &);

	public:
		bool Read(char_type &c)
		{
			c = *buf_++;
			return true;
		}

		template<typename IterT>
		size_t Read(IterT begin, size_t nCount)
		{			
			std::copy(buf_, buf_ + nCount, begin);
			std::advance(buf_, nCount);

			return nCount;
		}

	public:
		void Reset(char_type const *buf)
		{
			buf_ = buf;
		}

		char_type const *Position() const
		{
			return buf_;
		}

		void GoBack(int nSize)
		{
			std::advance(buf_, -nSize);
		}

		bool EndOfStream() const
		{
			return false;
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



	//----------------------------------------------------------
	// class BoundedMemoryWriter

	template<typename CharT>
	class BoundedMemoryWriter
	{
	public:
		typedef CharT		char_type;

	private:
		char_type * buf_;
		size_t left_;
		bool eof_;

	public:
		template<size_t _N>
		BoundedMemoryWriter(char_type (&arr)[_N])
			: buf_(arr)
			, left_(_N - 1)	// for '\0'
			, eof_(false)
		{}

		BoundedMemoryWriter(char_type *buf, size_t nSize)
			: buf_(buf)
			, left_(nSize)
			, eof_(false)
		{}

	private:
		BoundedMemoryWriter(const BoundedMemoryWriter &);
		BoundedMemoryWriter &operator=(const BoundedMemoryWriter &);

	public:
		void Write(char_type c)
		{
			if( left_ != 0 )
			{
				*buf_++ = c;
				--left_;
			}
			else
				eof_ = true;
		}

		template<typename IterT>
		void Write(IterT begin, IterT end)
		{
			assert(end > begin);

			// 得到最大区间
			size_t nDistance = std::distance(begin, end);
			size_t nCount = std::min(nDistance, left_);

			// 复制
			buf_ = std::copy(begin, begin + nCount, buf_);
			left_ -= nCount;

			if( left_ != 0 && begin != end )
				eof_ = true;
		}

	public:
		void Reset(char_type *buf, size_t nCount)
		{
			buf_	= buf;
			left_	= nCount;
			eof_	= false;
		}
		char_type *Position() const
		{
			return buf_;
		}

		size_t Left() const
		{
			return left_;
		}

		// State and Error detection
		bool EndOfStream() const
		{
			return eof_;
		}
		bool DeviceError() const
		{
			return eof_;
		}

		bool Good() const
		{
			return !DeviceError();
		}
	};


	//----------------------------------------------------------
	// class BoundedMemoryWriter

	template<typename CharT>
	class BoundedMemoryReader
	{
	public:	
		typedef CharT		char_type;

	private:
		char_type const *buf_;
		size_t left_;
		bool badFormat_;
		bool eof_;

	public:
		template<size_t _N>
		BoundedMemoryReader(char_type (&arr)[_N])
			: buf_(arr)
			, left_(_N - 1)	// for '\0'
			, badFormat_(false)
			, eof_(false)
		{}

		BoundedMemoryReader(char_type const *buf, size_t nCount)
			: buf_(buf)
			, left_(nCount)
			, badFormat_(false)
			, eof_(false)
		{}

	private:
		BoundedMemoryReader(const BoundedMemoryReader &);
		BoundedMemoryReader &operator=(const BoundedMemoryReader &);

	public:
		bool Read(char_type &c)
		{
			if( left_ != 0 )
			{
				c = *buf_++;
				--left_;

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
			while(nCount > 0 && left_ > 0)
			{
				*begin++ = *buf_++;
				--nCount;
				--left_;
			}

			if( left_ == 0 && nCount != 0 )
				eof_ = true;

			return nRead - nCount;
		}

	public:
		void Reset(char_type const *buf, size_t nCount)
		{
			buf_	= buf;
			left_	= nCount;
			eof_	= false;
			badFormat_ = false;
		}

		char_type const *Position() const
		{
			return buf_;
		}

		size_t Left() const
		{
			return left_;
		}

		void GoBack(int nSize)
		{
			std::advance(buf_, -nSize);
			left_	+= nSize;

			if( nSize > 0 )
				eof_ = false;
		}

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

#pragma warning(pop)



#endif