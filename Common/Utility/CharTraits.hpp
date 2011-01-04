#ifndef __CHAR_TRAITS_HPP
#define __CHAR_TRAITS_HPP


#include <wchar.h>
#include <locale>



namespace DataStructure
{

	template<typename CharT>
	struct CharTraits
	{
		typedef CharT	char_type;

		static void Assign(char_type &c1, const char_type &c2)
		{
			c1 = c2;
		}
		
		static bool Equal(const char_type &c1, const char_type &c2)
		{
			return c1 == c2;
		}
		static bool NotEqual(const char_type &c1, const char_type &c2)
		{
			return !(c1 == c2);
		}
		static bool LeftLess(const char_type &c1, const char_type &c2)
		{
			return c1 < c2;
		}

		static char_type Eos()
		{
			return char_type();		// the NULL character
		}
		static bool IsDel(char_type a)
		{
			return 0;
		}


		// Speed-up functions
		static int Compare(const char_type *str1, const char_type *str2, size_t n)
		{
			for(size_t i = 0; i < n; ++i)
			{
				if( NotEqual(str1[i], str2[i]) )
					return LeftLess(str1[i], str2[i]) ? -1 : 1;
			}

			return 0;
		}

		static size_t Length(const char_type* s)
		{
			size_t l = 0;

			while(NotEqual(*s++, Eos()))
				++l;

			return l;
		}

		static char_type *copy(char_type* s1, const char_type* s2, size_t n)
		{
			for(; n--; )
				Assign(s1[n], s2[n]);

			return s1;
		}

		static char_type* Move(char_type* s1, const char_type* s2, size_t n)
		{
			char_type a[n] = {0};

			for(size_t i = 0; i < n; ++i)
				Assign(a[i], s2[i]);
			for(size_t i = 0; i < n; ++i)
				Assign(s1[i], a[i]);

			return s1;
		}

		static char_type* Set(char_type* s1, const char_type &c, size_t n)
		{
			for(; n--; )
				Assign(s1[n], c);

			return s1;
		}
	};

	template<>
	struct CharTraits<char> 
	{
		typedef char	char_type;

		static void Assign(char_type& c1, const char_type& c2)
		{ c1 = c2; }

		static bool Equal(const char_type & c1, const char_type& c2)
		{ return (c1 == c2); }

		static bool NotEqual(const char_type& c1, const char_type& c2)
		{ return (c1 != c2); }

		static bool LeftLess(const char_type& c1, const char_type& c2)
		{ return (c1 < c2); }

		static char_type Eos() 
		{ return 0; }

		static bool IsDel(char_type a)
		{ return ::isspace(a) != 0 ; }

		static int Compare(const char_type* s1, const char_type* s2, size_t n)
		{ return ::memcmp(s1, s2, n); }

		static size_t Length(const char_type* s)
		{ return strlen(s); }

		static char_type* copy(char_type* s1, const char_type* s2, size_t n)
		{ return reinterpret_cast<char_type *>(::memcpy(s1, s2, n)); }

		static char_type* Move(char_type* s1, const char_type* s2, size_t n)
		{ return reinterpret_cast<char_type *>(::memmove(s1, s2, n)); }

		static char_type* Set(char_type* s1, const char_type& c, size_t n)
		{ return reinterpret_cast<char_type *>(::memset(s1, c, n)); }
	};

	template<>
	struct CharTraits<wchar_t> 
	{
		typedef wchar_t		char_type;

		static void Assign(char_type& c1, const char_type& c2)
		{ c1 = c2; }

		static bool Equal(const char_type & c1, const char_type& c2)
		{ return (c1 == c2); }

		static bool NotEqual(const char_type& c1, const char_type& c2)
		{ return (c1 != c2); }

		static bool LeftLess(const char_type& c1, const char_type& c2)
		{ return (c1 < c2); }

		static char_type Eos() 
		{ return 0; }

		static bool IsDel(char_type a)
		{ return ::iswspace(a) != 0; }

		static int Compare(const char_type* s1, const char_type* s2, size_t n)
		{ return ::wmemcmp(s1, s2, n); }

		static size_t Length(const char_type* s)
		{ return ::wcslen(s); }

		static char_type* copy(char_type* s1, const char_type* s2, size_t n)
		{ return ::wmemcpy(s1, s2, n); }

		static char_type* Move(char_type* s1, const char_type* s2, size_t n)
		{ return reinterpret_cast<char_type *>(::wmemmove(s1, s2, n)); }

		static char_type* Set(char_type* s1, const char_type& c, size_t n)
		{ return ::wmemset(s1, c, n); }
	};



	//----------------------------------------------------------------------
	// class CompareNoCaseTraits

	template<typename CharT>
	struct CompareNoCaseTraits
		: public std::char_traits<CharT>
	{
		static bool eq(CharT c1, CharT c2)
		{
			return ::toupper(c1) == ::toupper(c2);
		}

		static bool lt(CharT c1, CharT c2)
		{
			return ::toupper(c1) < ::toupper(c2);
		}

		static int compare(const CharT *s1, const CharT *s2, size_t n)
		{
			return ::memicmp(s1, s2, n);
		}

		static const CharT *find(const CharT *s, int n, CharT c)
		{
			while( n-- > 0 && ::toupper(*s) != ::toupper(c) )
				++s;

			return n > 0 ? s : 0;
		}
	};
}

#endif