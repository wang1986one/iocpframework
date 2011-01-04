#ifndef __IO_PRINT_TYPE_HPP
#define __IO_PRINT_TYPE_HPP



namespace DataStructure
{
	// Printf Format

	// for char
	struct CharPrintfFormat
	{
		typedef char FormatType;

		template<typename WriterT>
		static void Write(FormatType c, WriterT &w)
		{
			if( c == '\0' )
			{
				w.Write('\0');
				return;
			}

			const int buf_size = WriterT::BufSize;
			char buf[buf_size] = {0};

			::_snprintf(buf, buf_size, w.GetCharFormat(), c);
			w.Write(&buf[0], &buf[0] + ::strlen(buf));
		}
	};

	// for char *
	struct CharPointerPrintfFormat
	{
		typedef char * FormatType;

		template<typename WriterT>
		static void Write(const FormatType p, WriterT &w)
		{
			const int buf_size = WriterT::BufSize;
			char buf[buf_size] = {0};

			::_snprintf(buf, buf_size, w.GetCharPointerFormat(), p);
			w.Write(&buf[0], &buf[0] + ::strlen(buf));
		}
	};

	// for std::string
	struct StringAPrintfFormat
	{
		typedef std::string FormatType;

		template<typename WriterT>
		static void Write(const FormatType &s, WriterT &w)
		{
			CharPointerPrintfFormat::Write(s.c_str(), w);
		}
	};

	// for long
	struct LongPrintfFormat
	{
		typedef long FormatType;

		template<typename WriterT>
		static void Write(FormatType n, WriterT &w)
		{
			const int buf_size = WriterT::BufSize;
			char buf[buf_size] = {0};

			::_snprintf(buf, buf_size, w.GetLongFormat(), n);
			w.Write(&buf[0], &buf[0] + ::strlen(buf));
		}
	};

	// for unsigned long
	struct ULongPrintfFormat
	{
		typedef unsigned long FormatType;

		template<typename WriterT>
		static void Write(FormatType ul, WriterT &w)
		{
			const int buf_size = WriterT::BufSize;
			char buf[buf_size] = {0};

			::_snprintf(buf, buf_size, w.GetUlongFormat(), ul);
			w.Write(&buf[0], &buf[0] + ::strlen(buf));
		}
	};

	// for int
	struct IntPrintfFormat
	{
		typedef int FormatType;

		template<typename WriterT>
		static void Write(FormatType i, WriterT &w)
		{
			LongPrintfFormat::Write(i, w);
		}
	};

	// for unsigned int
	struct UIntPrintfFormat
	{
		typedef unsigned int FormatType;

		template<typename WriterT>
		static void Write(FormatType i, WriterT &w)
		{
			ULongPrintfFormat::Write(i, w);
		}
	};

	// for short
	struct ShortPrintfFormat
	{
		typedef short FormatType;

		template<typename WriterT>
		static void Write(FormatType s, WriterT &w)
		{
			IntPrintfFormat::Write(s, w);
		}
	};

	// for unsigned short
	struct UShortPrintfFormat
	{
		typedef unsigned short FormatType;

		template<typename WriterT>
		static void Write(FormatType s, WriterT &w)
		{
			UIntPrintfFormat::Write(s, w);
		}
	};

	// for bool
	struct BoolPrintfFormat
	{
		typedef bool FormatType;

		template<typename WriterT>
		static void Write(FormatType b, WriterT &w)
		{
			int i = b ? 1 : 0;
			IntPrintfFormat::Write(i, w);
		}
	};

	// for double
	struct DoublePrintfFormat
	{
		typedef double FormatType;

		template<typename WriterT>
		static void Write(FormatType d, WriterT &w)
		{
			const int buf_size = WriterT::BufSize;
			char buf[buf_size] = {0};

			_snprintf(buf, buf_size, w.GetDoubleFormat(), d);
			w.Write(buf, buf + ::strlen(buf));
		}
	};

	// for float
	struct FloatPrintfFormat
	{
		typedef float FormatType;

		template<typename WriterT>
		static void Write(FormatType f, WriterT &w)
		{
			DoublePrintfFormat::Write(f, w);
		}
	};

	// for void*
	struct VoidPointerPrintfFormat
	{
		typedef void * FormatType;

		template<typename WriterT>
		static void Write(const FormatType p, WriterT &w)
		{
			const int buf_size = WriterT::BufSize;
			char buf[buf_size] = {0};

			::_snprintf(buf, buf_size, w.GetVoidPointerFormat(), p);
			w.Write(buf, buf + ::strlen(buf));
		}
	};

}


#endif