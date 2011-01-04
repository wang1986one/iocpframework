#ifndef __IO_FORMAT_TYPE_HPP
#define __IO_FORMAT_TYPE_HPP


#include <limits>
#include "../Utility/CharType.hpp"



namespace DataStructure
{
	// helper function
	namespace Internal
	{

		// IsSpaceImpl
		inline bool IsSpaceImpl(char c)
		{
			return ::isspace(c) != 0;
		}
		inline bool IsSpaceImpl(wchar_t c)
		{
			return ::iswspace(c) != 0;
		}

		// SprintfImpl
		template<size_t _N, typename T>
		inline size_t SprintfImpl(char (&arr)[_N], T d)
		{
			return ::_snprintf_s(arr, _N, "%f", d);
		}
		template<size_t _N, typename T>
		inline size_t SprintfImpl(wchar_t (&arr)[_N], T d)
		{
			return ::_snwprintf_s(arr, _N, L"%f", d);
		}

		// IsDigitImpl
		inline bool IsDigitImpl(char c)
		{
			return 0 != ::isdigit(c);
		}
		inline bool IsDigitImpl(wchar_t c)
		{
			return 0 != ::iswdigit(c);
		}
	}
	
	// IsSpace
	template<typename CharT>
	inline bool IsSpace(CharT c)
	{
		return 0 != CharType::IsSpace(c);	//Internal::IsSpaceImpl(c);
	}

	// IsDigit
	template<typename CharT>
	inline bool IsDigit(CharT c)
	{
		return 0 != CharType::IsDigit(c);	//Internal::IsDigitImpl(c);
	}


	// Sprintf
	template<typename CharT, size_t _N, typename T>
	inline size_t Sprintf(CharT (&arr)[_N], const T &d)
	{
		return Internal::SprintfImpl(arr, d);
	}

	// Widen
	template<typename CharT>
	inline CharT Widen(char c)
	{
		static std::locale loc(""); 
		static const std::ctype<CharT> &ct = std::use_facet<std::ctype<CharT> >(loc);

		return ct.widen(c);
	}

	// Helper 忽略空格，读取整行
	template<typename CharT, typename ReaderT>
	inline ReaderT &SkipWhiteSpace(ReaderT &r)
	{
		CharT c = 0;
		while(r.Read(c) && IsSpace(c))
			;

		if( r && !r.EndOfStream() )
			r.GoBack(1);

		return r;
	}

	template<typename ReaderT, typename CharT>
	inline ReaderT &GetLine(ReaderT &r, std::basic_string<CharT> &strLine)
	{
		strLine.clear();

		CharT c = 0;
		while(r.Read(c) && c != '\n')
			strLine += c;

		return r;
	}


	//-------------------------------------------------------
	// basic format


	namespace Internal
	{
		// empty formatter
		struct EmptyFormat
		{
			typedef void FormatType;
		};
	}

	// for char
	struct CharFormat
	{
		typedef char FormatType;

		template<typename CharT, typename WritterT>
		static void Write(FormatType c, WritterT &w)
		{
			w.Write(c);
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &c, ReaderT &r)
		{
			if( r )
			{
				if( r.EndOfStream() )
					r.SetBadFormat(true);
				else
				{
					if( !r.Read(c) )
						r.SetBadFormat(true);
				}
			}
		}
	};

	// for wchar_t
	struct WCharFormat
	{
		typedef wchar_t FormatType;

		template<typename CharT, typename WritterT>
		static void Write(FormatType c, WritterT &w)
		{
			w.Write(c);
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &c, ReaderT &r)
		{
			if( r )
			{
				if( r.EndOfStream() )
					r.SetBadFormat(true);
				else
				{
					if( !r.Read(c) )
						r.SetBadFormat(true);
				}
			}
		}
	};


	// for char *
	struct CharPointerFormat
	{
		typedef char * FormatType;

		template<typename CharT, typename WritterT>
		static void Write(const char * const p, WritterT &w)
		{
			w.Write(p, p + strlen(p));
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType p, ReaderT &r)
		{
			SkipWhiteSpace<CharT>(r);

			if( r && r.EndOfStream() )
			{
				r.SetBadFormat(true);
				return;
			}

			char c = 0;
			while(r.Read(c) && c != '\0' && !IsSpace(c))
				*p++ = c;
			*p = '\0';

			if( r && !r.EndOfStream() )
				r.GoBack(1);
		}
	};

	// for wchar_t *
	struct WCharPointerFormat
	{
		typedef wchar_t * FormatType;

		template<typename CharT, typename WritterT>
		static void Write(const wchar_t * const p, WritterT &w)
		{
			w.Write(p, p + ::wcslen(p));
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType p, ReaderT &r)
		{
			SkipWhiteSpace<CharT>(r);

			if( r && r.EndOfStream() )
			{
				r.SetBadFormat(true);
				return;
			}

			wchar_t c = 0;
			while(r.Read(c) && c != '\0' && !IsSpace(c))
				*p++ = c;
			*p = '\0';

			if( r && !r.EndOfStream() )
				r.GoBack(1);
		}
	};


	// for std::string
	struct StringAFormat
	{
		typedef std::string FormatType;

		template<typename CharT, typename WriterT>
		static void Write(const FormatType &s, WriterT &w)
		{
			w.Write(s.begin(), s.end());
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &s, ReaderT &r)
		{
			SkipWhiteSpace<CharT>(r);

			if( r && r.EndOfStream() )
			{
				r.SetBadFormat(true);
				return;
			}

			s.clear();
			char c = 0;
			while(r.Read(c) && c != '\0' && !IsSpace(c))
			{
				s.push_back(c);
			}

			if( r && !r.EndOfStream() )
			{
				r.GoBack(1);
			}
		}
	};

	// for std::wstring
	struct StringWFormat
	{
		typedef std::wstring FormatType;

		template<typename CharT, typename WriterT>
		static void Write(const FormatType &s, WriterT &w)
		{
			w.Write(s.begin(), s.end());
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &s, ReaderT &r)
		{
			SkipWhiteSpace<CharT>(r);

			if( r && r.EndOfStream() )
			{
				r.SetBadFormat(true);
				return;
			}

			s.clear();
			CharT c = 0;
			while(r.Read(c) && c != '\0' && !IsSpace(c))
			{
				s.push_back(c);
			}

			if( r && !r.EndOfStream() )
			{
				r.GoBack(1);
			}
		}
	};


	// for long
	struct LongFormat
	{
		typedef long FormatType;

		template<typename CharT, typename WriterT>
		static void Write(FormatType n, WriterT &w)
		{
			if( n == 0 )
			{
				w.Write('0');
				return;
			}

			const size_t buflen = std::numeric_limits<FormatType>::digits10 + 2;
			CharT buf[buflen] = {0};

			CharT *end = buf + buflen;
			CharT *p = end;

			unsigned long ul = n < 0 ? -n : n;
			while(ul)
			{
				*--p = '0' + ul % 10;
				ul /= 10;
			}

			if( n < 0 )
			{
				*--p = '-';
			}

			w.Write(p, end);
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &n, ReaderT &r)
		{
			SkipWhiteSpace<CharT>(r);

			if( r && r.EndOfStream() )
			{
				r.SetBadFormat(true);
				return;
			}

			CharT c = 0;
			FormatType t = 0;
			bool minus = false;
			bool read_something = false;
			if( r.Read(c) )
			{
				if( c == '-' )
				{
					minus = true;
				}
				else if( c ==  '+' )
				{
					// OK, this we can ignore
				}
				else if( IsDigit(c) )
				{
					read_something = true;
					t += c -  '0';
				}
				else
				{
					r.GoBack(1);
					return;
				}
			}

			while(r.Read(c) && IsDigit(c))
			{
				read_something = true;
				t *= 10;
				t += c - '0';
			}

			if( !r.DeviceError() )
			{
				if( read_something )
				{
					n = t;
					if( minus )
					{
						n *= -1;
					}
				}
				else
				{
					r.SetBadFormat(true);
				}

				if( !r.EndOfStream() )
				{
					r.GoBack(1);
				}
			}
		}
	};


	// for unsigned long
	struct ULongFormat
	{
		typedef unsigned long FormatType;

		template<typename CharT, typename WriterT>
		static void Write(FormatType ul, WriterT &w)
		{
			if( ul == 0 )
			{
				w.Write('0');
				return;
			}

			const size_t buflen = std::numeric_limits<FormatType>::digits10 + 2;
			CharT buf[buflen] = {0};

			CharT *end = buf + buflen;
			CharT *p = end;

			while (ul)
			{
				*--p = '0' + ul % 10;
				ul /= 10;
			}

			w.Write(p, end);
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &n, ReaderT &r)
		{
			SkipWhiteSpace<CharT>(r);

			if( r && r.EndOfStream() )
			{
				r.SetBadFormat(true);
				return;
			}

			CharT c = 0;
			FormatType t = 0;
			bool read_something = false;
			while( r.Read(c) && IsDigit(c) )
			{
				read_something = true;
				t *= 10;
				t += c - '0';
			}

			if( !r.DeviceError() )
			{
				if (read_something)
					n = t;
				else
					r.SetBadFormat(true);

				if( !r.EndOfStream() )
					r.GoBack(1);
			}
		}
	};

	// for int
	struct IntFormat
	{
		typedef int FormatType;

		template<typename CharT, typename WriterT>
		static void Write(FormatType i, WriterT &w)
		{
			LongFormat::Write<CharT>(i, w);
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &i, ReaderT &r)
		{
			long n = 0;
			LongFormat::Read<CharT>(n, r);

			if( r )
				i = static_cast<FormatType>(n);
		}
	};

	// for unsigned int
	struct UIntFormat
	{
		typedef unsigned int FormatType;

		template<typename CharT, typename WriterT>
		static void Write(FormatType i, WriterT &w)
		{
			ULongFormat::Write<CharT>(i, w);
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &i, ReaderT &r)
		{
			unsigned long un = 0;
			ULongFormat::Read(un, r);

			if( r )
				i = static_cast<FormatType>(un);
		}
	};

	// for short
	struct ShortFormat
	{
		typedef short FormatType;

		template<typename CharT, typename WriterT>
		static void Write(FormatType s, WriterT &w)
		{
			IntFormat::Write<CharT>(s, w);
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &s, ReaderT &r)
		{
			int n = 0;
			IntFormat::Read(n, r);

			if( r )
				s = static_cast<FormatType>(n);
		}
	};

	// for unsigned short
	struct UShortFormat
	{
		typedef unsigned short FormatType;

		template<typename CharT, typename WriterT>
		static void Write(FormatType s, WriterT &w)
		{
			UIntFormat::Write<CharT>(s, w);
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &s, ReaderT &r)
		{
			unsigned int un = 0;
			UIntFormat::Read(un, r);

			if( r )
				s = static_cast<FormatType>(un);
		}
	};

	// for bool
	struct BoolFormat
	{
		typedef bool FormatType;

		template<typename CharT, typename WriterT>
		static void Write(bool b, WriterT &w)
		{
			w.Write(b ? '1' : '0');
		}

		template<typename CharT, typename ReaderT>
		static void Read(bool &b, ReaderT &r)
		{
			SkipWhiteSpace<CharT>(r);

			if( r && r.EndOfStream() )
			{
				r.SetBadFormat(true);
				return;
			}

			CharT c = 0;
			if( r.Read(c) )
			{
				if( c == '0' || c == '1' )
				{
					b = (c == '1');
				}
				else
				{
					r.GoBack(1);
					r.SetBadFormat(true);
				}
			}
		}
	};

	

	// for double
	struct DoubleFormat
	{
		typedef double FormatType;

		template<typename CharT, typename WriterT>
		static void Write(FormatType d, WriterT &w)
		{
			// Is this a good "general case"?

			const size_t buf_size = 64; // arbitrary choice
			CharT buf[buf_size] = {0};

			size_t nRet = Sprintf(buf, d);
			w.Write(buf, buf + nRet);
		}

		template<typename CharT, typename ReaderT>
		static void Read(FormatType &d, ReaderT &r)
		{
			SkipWhiteSpace<CharT>(r);

			if( r && r.EndOfStream() )
			{
				r.SetBadFormat(true);
				return;
			}

			CharT c = 0;

			// is this good for the "general case"?

			double t1 = 0.0;
			double t2 = 0.0;
			int digits_after_dot = 0;
			bool minus = false;
			bool read_some_digits = false;

			// this is a *very* simple parser with the grammar:
			// ( '+' | '-' )? digit+ ('.' digit+)?

			// read the sign (or first digit)
			if( r.Read(c) )
			{
				if( c == '-' )
				{
					minus = true;
				}
				else if( c == '+' )
				{
					minus = false;
				}
				else if( ::isdigit(c) )
				{
					t1 = c - '0';
					read_some_digits = true;
				}
				else
				{
					r.GoBack(1);
					r.SetBadFormat(true);

					return;
				}
			}

			// read the digits before dot
			while( r.Read(c) )
			{
				if( IsDigit(c) )
				{
					t1 *= 10;
					t1 += c - '0';
					read_some_digits = true;
				}
				else
				{
					if( c == '.' )
						break;
					else
					{
						if( read_some_digits )
						{
							d = minus ? -t1 : t1;
							r.GoBack(1);
							return;
						}
						else
						{
							r.GoBack(2);
							r.SetBadFormat(true);
							return;
						}
					}
				}
			}

			// read the digits after the dot
			read_some_digits = false;
			while(r.Read(c))
			{
				if( IsDigit(c) )
				{
					t2 *= 10;
					t2 += c - '0';
					++digits_after_dot;
					read_some_digits = true;
				}
				else
					break;
			}

			if( read_some_digits )
			{
				// compose integer and fractional part
				while( digits_after_dot-- )
				{
					t2 /= 10;
				}
				d = t1 + t2;
				if( minus )
				{
					d *= -1;
				}

				if( r && !r.EndOfStream() )
				{
					r.GoBack(1);
				}
			}
			else
			{
				// the dot does not belong to token
				d = minus ? -t1 : t1;
				r.GoBack(2);
			}
		}
	};

	// for float
	struct FloatFormat
	{
		typedef float FormatType;

		template<typename CharT, typename WriterT>
		static void Write(FormatType f, WriterT &w)
		{
			DoubleFormat::Write<CharT>(f, w);
		}

		template<typename ReaderT>
		static void Read(FormatType &f, ReaderT &r)
		{
			double d = 0.0;
			DoubleFormat::Read(d, r);

			if( r )
				f = static_cast<float>(d);
		}
	};
}


#endif