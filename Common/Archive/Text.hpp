#ifndef __ARCHIVE_TEXT_HPP
#define __ARCHIVE_TEXT_HPP


#include "Basic.hpp"
#include "../Container/String.hpp"



namespace DataStructure
{

	namespace IO
	{


		//----------------------------------------------------
		// TextReader

		template<typename ReadArchiveT, typename PreT>
		inline typename ReadArchiveT::int_type GetFirstNotOf(ReadArchiveT &ar, PreT pre)
		{
			typedef typename ReadArchiveT::int_type	int_type;

			int_type ch;
			while( pre(ch == ar.Get()) )
				;

			return ch;
		}

		template<typename ReadArchiveT>
		inline typename ReadArchiveT::int_type GetNWS(ReadArchiveT &ar)
		{
			typedef typename ReadArchiveT::int_type	int_type;

			int_type ch;
			while( ::isspace(ch = ar.Get()) )
				;

			return ch;
		}


		//----------------------------------------------------
		// Skip chars

		template<typename ReadArchiveT>
		inline size_t SkipEOL(ReadArchiveT &ar)
		{
			typedef typename ReadArchiveT::int_type	int_type;
			const int_type ch = ar.Peek();

			if( ch == 0x0a )
			{
				ar.Get();
				if( ar.Peek() == 0x0d )
				{
					ar.Get();
					return 2;
				}

				return 1;
			}
			else if( ch == 0x0d )
			{
				ar.Get();
				if( ar.Peek() == 0x0a )
				{
					ar.Get();
					return 2;
				}
				return 1;
			}

			return 0;
		}

		template<typename ReadArchiveT>
		inline void SkipWS(ReadArchiveT &ar)
		{
			typedef typename ReadArchiveT::int_type	int_type;

			int_type ch;
			while( ::isspace(ch = ar.Get()) )
				;

			ar.UnGet(ch);
		}



		//-----------------------------------------------------------------
		// Skip while

		template<typename ReadArchiveT, typename PreT>
		inline size_t SkipWhile(ReadArchiveT &ar, PreT pre)
		{
			typedef typename ReadArchiveT::int_type	int_type;
			typedef typename ReadArchiveT::size_type	SizeTye;

			assert(!pre(ReadArchiveT::EndCh));

			SizeTye nSize = 0;
			int_type ch;

			while(pre(ch = ar.Get()))
				++nSize;

			ar.UnGet();
			return nSize;
		}

		template<typename ReadArchiveT>
		inline size_t SkipCSymbol(ReadArchiveT &ar)
		{
			typedef typename ReadArchiveT::int_type	int_type;

			int_type ch = ar.Get();
			if( char_type::IsCSymbolFirstChar(ch) )
				return SkipWhile(ar, char_type::IsCSymbolNextChar()) + 1;
			else
			{
				ar.UnGet();
				return 0;
			}
		}



		//-----------------------------------------------------------------
		// GetTextString

		template<typename ReadArchiveT, typename StringT, typename PreT>
		inline size_t GetWhile(ReadArchiveT &ar, StringT &str, PreT pre, size_t count = 0)
		{
			count += SkipWhile(ar, pre);
			ar.ReGet(count, Resize(str, count), count);

			return count;
		}
		template<typename ReadArchiveT, typename AllocT, typename PreT>
		inline size_t GetWhile(ReadArchiveT &ar, AllocT &alloc, std::basic_string<typename ReadArchiveT::char_type> &str, PreT pre)
		{
			std::basic_string<typename ReadArchiveT::char_type, AllocT> strTmp(alloc, str);

			return GetWhile(ar, strTmp, pre);
		}


		//-----------------------------------------------------------------
		// GetLine

		template<typename ReadArchiveT, typename StringT>
		inline size_t GetLine(ReadArchiveT &ar, StringT &str)
		{
			return GetWhile(ar, str, char_type::NotIsEOLType()) + SkipEOL(ar);
		}
		template<typename ReadArchiveT, typename AllocT, typename CharT>
		inline size_t GetLine(ReadArchiveT &ar, AllocT &alloc, std::basic_string<CharT> &str)
		{
			std::basic_string<CharT, AllocT> strTmp(alloc, str);
			return GetWhile(ar, strTmp);
		}



		//-----------------------------------------------------------------
		// GetCSymbol

		template<typename ReadArchiveT, typename StringT>
		inline size_t GetCSymbol(ReadArchiveT &ar, StringT &str)
		{
			size_t count = SkipCSymbol(ar);

			ar.ReGet(count, Resize(str, count), count);
			return count;
		}
		template<typename ReadArchiveT, typename AllocT, typename CharT>
		inline size_t GetCSymbol(ReadArchiveT &ar, AllocT &alloc, std::basic_string<CharT> &str)
		{
			std::basic_string<CharT, AllocT> strTmp(alloc, str);
			return GetCSymbol(ar, strTmp);
		}


		//-----------------------------------------------------------------
		// GetUInt

		template<typename ReadArchiveT, typename UIntType>
		inline size_t GetUInt(ReadArchiveT &ar, UIntType preVal, unsigned int radix = 10)
		{
			typedef typename ReadArchiveT::int_type	int_type;

			unsigned int dig = 0;
			int_type ch;

			while((dig == DigitTable::ToDigit(ch = ar.Get())) < radix)
				preVal = preVal * radix + dig;

			ar.UnGet();
			return preVal;
		}
		template<typename ReadArchiveT>
		inline size_t GetUInt(ReadArchiveT &ar)
		{
			return GetUInt(ar, static_cast<UINT>(0));
		}


		//-----------------------------------------------------------------
		// ScanUInt

		template<typename ReadArchiveT, typename UIntType>
		inline bool ScanUInt(ReadArchiveT &ar, UIntType val, unsigned int radix = 10)
		{
			typedef typename ReadArchiveT::int_type	int_type;

			int_type ch = GetNWS(ar);
			unsigned int dig = DigitTable::ToDigit(ch);

			if( dig < radix )
			{	
				val = GetUInt(ar, static_cast<UIntType>(dig), radix);
				return true;
			}
			else
			{
				ar.UnGet(ch);
				val = 0;
				return false;
			}
		}



		//-----------------------------------------------------------------
		// ScanCSymbol

		template<typename ReadArchiveT, typename StringT>
		inline bool ScanCSymbol(ReadArchiveT &ar, StringT &str)
		{
			SkipWS(ar);

			return GetCSymbol(ar, str) ? true : false;
		}
		template<typename ReadArchiveT, typename AllocT>
		inline bool ScanCSymbol(ReadArchiveT &ar, AllocT &alloc, std::basic_string<typename ReadArchiveT::char_type> &str)
		{
			std::basic_string<typename ReadArchiveT::char_type, AllocT> strTmp(alloc, str);
			return ScanCSymbol(ar, strTmp);
		}





		//-----------------------------------------------------------------
		// TextWriter

		template<typename WriteArchiveT>
		inline void Print(WriteArchiveT &ar, const std::string &str)
		{
			ar.Put(str.data(), str.length());
		}
		template<typename WriteArchiveT>
		inline void Print(WriteArchiveT &ar, const std::wstring &str)
		{
			ar.Put(str.data(), str.length());
		}


		template<typename WriteArchiveT, typename IteratorT, typename SepT>
		inline void PrintLines(WriteArchiveT &ar, IteratorT first, IteratorT last, const SepT &sep)
		{
			for(; first != last; ++first)
			{
				Print(ar, *first);
				Print(ar, sep);
			}
		}

		template<typename WriteArchiveT, typename DataIt>
		inline void PrintLines(WriteArchiveT &ar, DataIt first, DataIt last)
		{
			typedef typename WriteArchiveT::char_type char_type;
			for(; first != last; ++first)
			{
				Print(ar, *first);
				ar.Put('\n');
			}
		}

		template<typename WriteArchiveT, typename ContainerT>
		inline void PrintLines(WriteArchiveT &ar, const ContainerT &container)
		{
			typedef typename WriteArchiveT::char_type	char_type;

			PrintLines(ar, container.begin(), container.end(), static_cast<char_type>('\n'));
		}


	}
}







#endif