#ifndef __IO_FILE_STREAM_HPP
#define __IO_FILE_STREAM_HPP

namespace DataStructure
{

	//------------------------------------------
	// class FileWriter

	class FileWriter
	{
	public:
		typedef char char_type;

	private:
		FILE *file_;

	public:
		explicit FileWriter(FILE *file)
			: file_(file)
		{}

	public:
		void Write(char c)
		{
			::fputc(c, file_);
		}
		void Write(char c, size_t /*nCount*/)
		{
			c;
			//::fpu
		}

		template<typename IterT>
		void Write(IterT begin, IterT end)
		{
			for(; begin != end; ++begin)
				Write(*begin);
		}

		void Write(const char *begin, const char *end)
		{
			::fwrite(begin, 1, end - begin, file_);
		}

	public:
		void Reset(FILE *file)
		{
			file_ = file;
		}

		bool EndOfStream() const
		{
			return 0 != ::feof(file_);
		}
		bool DeviceError() const
		{
			return 0 == ::ferror(file_);
		}
		bool Good() const
		{
			return !DeviceError();
		}
	};


	//------------------------------------------
	// class FileReader

	class FileReader
	{
	public:
		typedef char	char_type;

	private:
		FILE *file_;
		bool badFormat_;

	public:
		explicit FileReader(FILE *file)
			: file_(file)
			, badFormat_(false)
		{}

	public:
		bool Read(char &c)
		{
			int nRet = ::fgetc(file_);
			if( nRet != EOF )
			{
				c = static_cast<char>(nRet);
				return true;
			}
	
			return false;
		}

		template<typename IterT>
		size_t Read(IterT begin, size_t nCount)
		{
			size_t nRead = nCount;
			char c = 0;

			while( nCount-- != 0 && Read(c) )
				*begin = c;

			return nRead - nCount;
		}

		size_t Read(char *p, size_t nCount)
		{
			return ::fread(p, 1, nCount, file_);
		}

	public:
		void Reset(FILE *file)
		{
			file_ = file;
		}

		bool EndOfStream() const
		{
			return 0 != ::feof(file_);
		}

		bool DeviceError() const
		{
			return 0 == ::ferror(file_);
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