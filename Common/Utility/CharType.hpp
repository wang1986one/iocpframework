#ifndef __CHAR_TYPE_HPP
#define __CHAR_TYOE_HPP


namespace DataStructure
{


	// -------------------------------------------------------------------------
	// struct DigitTable

	const unsigned int STD_DIGIT_TABLE_BASE		= '0';
	const unsigned int STD_DIGIT_TABLE_MAX		= ('f' - '0' + 1);
	const unsigned int STD_DIGIT_INVALID		= 0xFF;


	template<typename Unused>
	struct DigitTableT
	{
		static unsigned char data[STD_DIGIT_TABLE_MAX];

		static unsigned ToDigit(unsigned int c)
		{
			unsigned int uIndex =  c - STD_DIGIT_TABLE_BASE;
			return uIndex < STD_DIGIT_TABLE_MAX ? data[uIndex] : STD_DIGIT_INVALID;
		}
	};


	template<typename Unused>
	unsigned char DigitTableT<Unused>::data[STD_DIGIT_TABLE_MAX] = 
	{
		0,	// 0 [48]
		1,	// 1 [49]
		2,	// 2 [50]
		3,	// 3 [51]
		4,	// 4 [52]
		5,	// 5 [53]
		6,	// 6 [54]
		7,	// 7 [55]
		8,	// 8 [56]
		9,	// 9 [57]
		STD_DIGIT_INVALID,	// : [58]
		STD_DIGIT_INVALID,	// ; [59]
		STD_DIGIT_INVALID,	// < [60]
		STD_DIGIT_INVALID,	// = [61]
		STD_DIGIT_INVALID,	// > [62]
		STD_DIGIT_INVALID,	// ? [63]
		STD_DIGIT_INVALID,	// @ [64]
		10,	// A [65]
		11,	// B [66]
		12,	// C [67]
		13,	// D [68]
		14,	// E [69]
		15,	// F [70]
		STD_DIGIT_INVALID,	// G [71]
		STD_DIGIT_INVALID,	// H [72]
		STD_DIGIT_INVALID,	// I [73]
		STD_DIGIT_INVALID,	// J [74]
		STD_DIGIT_INVALID,	// K [75]
		STD_DIGIT_INVALID,	// L [76]
		STD_DIGIT_INVALID,	// M [77]
		STD_DIGIT_INVALID,	// N [78]
		STD_DIGIT_INVALID,	// O [79]
		STD_DIGIT_INVALID,	// P [80]
		STD_DIGIT_INVALID,	// Q [81]
		STD_DIGIT_INVALID,	// R [82]
		STD_DIGIT_INVALID,	// S [83]
		STD_DIGIT_INVALID,	// T [84]
		STD_DIGIT_INVALID,	// U [85]
		STD_DIGIT_INVALID,	// V [86]
		STD_DIGIT_INVALID,	// W [87]
		STD_DIGIT_INVALID,	// X [88]
		STD_DIGIT_INVALID,	// Y [89]
		STD_DIGIT_INVALID,	// Z [90]
		STD_DIGIT_INVALID,	// [ [91]
		STD_DIGIT_INVALID,	// \ [92]
		STD_DIGIT_INVALID,	// ] [93]
		STD_DIGIT_INVALID,	// ^ [94]
		STD_DIGIT_INVALID,	// _ [95]
		STD_DIGIT_INVALID,	// ` [96]
		10,	// a [97]
		11,	// b [98]
		12,	// c [99]
		13,	// d [100]
		14,	// e [101]
		15,	// f [102]
	}

	typedef DigitTableT<void> DigitTable;


	// -------------------------------------------------------------------------
	// STD_CTYPE_XXX

	const unsigned int STD_CTYPE_UPPER		=				0x01;		/* upper case letter[A-Z] */
	const unsigned int STD_CTYPE_LOWER		=				0x02;		/* lower case letter[a-z] */
	const unsigned int STD_CTYPE_DIGIT		=				0x04;		/* digit[0-9] */
	const unsigned int STD_CTYPE_UNDERLINE	=				0x08;		/* underline[_] */
	const unsigned int STD_CTYPE_XDIGIT		=				0x10;		/* xdigit[0-9a-fA-F] */
	const unsigned int STD_CTYPE_SPACE		=				0x20;		/* [ \t\r\n] */
	const unsigned int STD_CTYPE_ADD		=				0x40;		/* [+] */
	const unsigned int STD_CTYPE_SUB		=				0x80;		/* [-], dash(?????)/hyphen(l???) */
	const unsigned int STD_CTYPE_MUL		=				0x100;		/* [*] */
	const unsigned int STD_CTYPE_DIV		=				0x200;		/* [/] */
	const unsigned int STD_CTYPE_LT			=				0x400;		/* [<] */
	const unsigned int STD_CTYPE_GT			=				0x800;		/* [>] */
	const unsigned int STD_CTYPE_EQ			=				0x1000;		/* [=] */
	const unsigned int STD_CTYPE_RDIV		=				0x2000;		/* [\], right-division, anti-slash */
	const unsigned int STD_CTYPE_DOT		=				0x4000;		/* [.] */
	const unsigned int STD_CTYPE_COLON		=				0x8000;		/* [:], colon */
	const unsigned int STD_CTYPE_PERCENT	=				0x10000;	/* [%] */
	const unsigned int STD_CTYPE_AND		=				0x20000;	/* [&] */
	const unsigned int STD_CTYPE_OR			=				0x40000;	/* [|] */
	const unsigned int STD_CTYPE_MAX_CHAR	=				128;

	const unsigned int STD_CTYPE_PATH_SEP		=			(STD_CTYPE_DIV | STD_CTYPE_RDIV);
	const unsigned int STD_CTYPE_ALPHA			=			(STD_CTYPE_UPPER | STD_CTYPE_LOWER);
	const unsigned int STD_SYMBOL_FIRST_CHAR	=			(STD_CTYPE_ALPHA);
	const unsigned int STD_SYMBOL_NEXT_CHAR		=			(STD_SYMBOL_FIRST_CHAR | STD_CTYPE_DIGIT);
	const unsigned int STD_CSYMBOL_FIRST_CHAR	=			(STD_CTYPE_ALPHA | STD_CTYPE_UNDERLINE);
	const unsigned int STD_CSYMBOL_NEXT_CHAR	=			(STD_CSYMBOL_FIRST_CHAR | STD_CTYPE_DIGIT);
	const unsigned int STD_XMLSYMBOL_FIRST_CHAR =			(STD_CSYMBOL_FIRST_CHAR);
	const unsigned int STD_XMLSYMBOL_NEXT_CHAR	=			(STD_CSYMBOL_NEXT_CHAR | STD_CTYPE_SUB);
		
	// -------------------------------------------------------------------------

	template<typename T, typename CharT>
	inline unsigned int IsStdCtype(const T &mask, const CharT &c)
	{
		return STD_CTYPE_MAX_CHAR > c ? (mask & c) : 0;
	}


#define STD_CTYPE_OP_(op, is)										\
	struct op														\
	{																\
		unsigned int operator()(unsigned int c) const				\
		{ return is(c); }											\
	}

#define STD_CTYPE_OP_AND_NOT_(op, is)								\
	STD_CTYPE_OP_(op, is);											\
	STD_CTYPE_OP_(Not##op, !is)

	

	// -------------------------------------------------------------------------
	// class char_type

	template<typename Unused>
	struct CharTypeT
	{
		static unsigned int data[STD_CTYPE_MAX_CHAR];

		static unsigned int Is(unsigned int typeMask, unsigned int c)
		{
			return IsStdCtype(typeMask, data[c]);
		}

		static unsigned int IsDigit(unsigned int c)
		{
			return IsStdCtype(STD_CTYPE_DIGIT, data[c]);
		}

		static unsigned int IsLower(unsigned int c)
		{
			return IsStdCtype(STD_CTYPE_LOWER, data[c]);
		}

		static unsigned int IsUpper(unsigned int c)
		{
			return IsStdCtype(STD_CTYPE_UPPER, data[c]);
		}

		static unsigned int IsAlpha(unsigned int c)
		{
			return IsStdCtype(STD_CTYPE_ALPHA, data[c]);
		}

		static unsigned int IsXDigit(unsigned int c)
		{
			return IsStdCtype(STD_CTYPE_XDIGIT, data[c]);
		}

		static unsigned int IsSpace(unsigned int c)
		{
			return IsStdCtype(STD_CTYPE_SPACE, data[c]);
		}

		static unsigned int IsSymbolFirstChar(unsigned int c)
		{
			return IsStdCtype(STD_SYMBOL_FIRST_CHAR, data[c]);
		}

		static unsigned int IsSymbolNextChar(unsigned int c)
		{
			return IsStdCtype(STD_SYMBOL_NEXT_CHAR, data[c]);;
		}

		static unsigned int IsCSymbolFirstChar(unsigned int c)
		{
			return IsStdCtype(STD_CSYMBOL_FIRST_CHAR, data[c]);
		}

		static unsigned int IsCSymbolNextChar(unsigned int c)
		{
			return IsStdCtype(STD_CSYMBOL_NEXT_CHAR, data[c]);
		}

		static unsigned int IsXMLSymbolFirstChar(unsigned int c)
		{
			return IsStdCtype(STD_XMLSYMBOL_FIRST_CHAR, data[c]);
		}

		static unsigned int IsXMLSymbolNextChar(unsigned int c)
		{
			return IsStdCtype(STD_XMLSYMBOL_NEXT_CHAR, data[c]);
		}

		static unsigned int IsPathSeparator(unsigned int c)
		{
			return c == STD_CTYPE_PATH_SEP;
		}

		static unsigned int IsUnderline(unsigned int c)
		{
			return c == STD_CTYPE_UNDERLINE;
		}

		// EOL ==> end Of Line / Line Break
		// 以ASCII为基础或兼容的字符集用LF或CR或CR+LF
		// 其中LF = 0x0A		CR = 0x0D
		static unsigned int IsEOL(unsigned int c)
		{
			return c == 0x0A || c == 0x0D || c == -1;
		}




		struct IsDigitType;					struct NotIsDigitType;
		struct IsLowerType;					struct NotIsLowerType;
		struct IsUpperType;					struct NotIsUpperType;
		struct IsAlphaType;					struct NotIsAlphaType;
		struct IsXDigitType;				struct NotIsXDigitType;
		struct IsCSymbolFirstCharType;		struct NotIsCSymbolFirstCharType;
		struct IsCSymbolNextCharType;		struct NotIsCSymbolNextCharType;
		struct IsXmlSymbolFirstCharType;	struct NotIsXmlSymbolFirstCharType;
		struct IsXmlSymbolNextCharType;		struct NotIsXmlSymbolNextCharType;
		struct IsUnderlineType;				struct NotIsUnderlineType;
		struct IsPathSeparatorType;			struct NotIsPathSeparatorType;
		struct IsEOLType;					struct NotIsEOLType;
		struct IsSpaceType;					struct NotIsSpaceType;
	

		STD_CTYPE_OP_AND_NOT_(IsDigitType,				IsDigit);
		STD_CTYPE_OP_AND_NOT_(IsLowerType,				IsLower);
		STD_CTYPE_OP_AND_NOT_(IsUpperType,				IsUpper);
		STD_CTYPE_OP_AND_NOT_(IsAlphaType,				IsAlpha);
		STD_CTYPE_OP_AND_NOT_(IsXDigitType,				IsXDigit);
		STD_CTYPE_OP_AND_NOT_(IsUnderlineType,			IsUnderline);
		STD_CTYPE_OP_AND_NOT_(IsPathSeparatorType,		IsPathSeparator);
		STD_CTYPE_OP_AND_NOT_(IsSymbolFirstCharType,	IsSymbolFirstChar);
		STD_CTYPE_OP_AND_NOT_(IsSymbolNextCharType,		IsSymbolNextChar);
		STD_CTYPE_OP_AND_NOT_(IsCSymbolFirstCharType,	IsCSymbolFirstChar);
		STD_CTYPE_OP_AND_NOT_(IsCSymbolNextCharType,	IsCSymbolNextChar);
		STD_CTYPE_OP_AND_NOT_(IsXmlSymbolFirstCharType, IsXMLSymbolFirstChar);
		STD_CTYPE_OP_AND_NOT_(IsXmlSymbolNextCharType,	IsXMLSymbolNextChar);
		STD_CTYPE_OP_AND_NOT_(IsEOLType,				IsEOL);
		STD_CTYPE_OP_AND_NOT_(IsSpaceType,				IsSpace);
	};

	template<typename Unused>
	unsigned CharTypeT<Unused>::data[STD_CTYPE_MAX_CHAR] =
	{
		0,	//   [0]
		0,	//   [1]
		0,	//   [2]
		0,	//   [3]
		0,	//   [4]
		0,	//   [5]
		0,	//   [6]
		0,	//   [7]
		0,	//   [8]
		STD_CTYPE_SPACE,	//   [9]
		STD_CTYPE_SPACE,	//   [10]
		0,	//   [11]
		0,	//   [12]
		STD_CTYPE_SPACE,	//   [13]
		0,	//   [14]
		0,	//   [15]
		0,	//   [16]
		0,	//   [17]
		0,	//   [18]
		0,	//   [19]
		0,	//   [20]
		0,	//   [21]
		0,	//   [22]
		0,	//   [23]
		0,	//   [24]
		0,	//   [25]
		0,	//   [26]
		0,	//   [27]
		0,	//   [28]
		0,	//   [29]
		0,	//   [30]
		0,	//   [31]
		STD_CTYPE_SPACE,					//   [32]
		0,	// ! [33]
		0,	// " [34]
		0,	// # [35]
		0,	// $ [36]
		STD_CTYPE_PERCENT,					// % [37]
		STD_CTYPE_AND,						// & [38]
		0,	// ' [39]
		0,	// ( [40]
		0,	// ) [41]
		STD_CTYPE_MUL,						// * [42]
		STD_CTYPE_ADD,						// + [43]
		0,	// , [44]
		STD_CTYPE_SUB,						// - [45]
		STD_CTYPE_DOT,						// . [46]
		STD_CTYPE_DIV,						// / [47]
		STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 0 [48]
		STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 1 [49]
		STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 2 [50]
		STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 3 [51]
		STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 4 [52]
		STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 5 [53]
		STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 6 [54]
		STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 7 [55]
		STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 8 [56]
		STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 9 [57]
		STD_CTYPE_COLON,					// : [58]
		0,	// ; [59]
		STD_CTYPE_LT,						// < [60]
		STD_CTYPE_EQ,						// = [61]
		STD_CTYPE_GT,						// > [62]
		0,	// ? [63]
		0,	// @ [64]
		STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// A [65]
		STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// B [66]
		STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// C [67]
		STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// D [68]
		STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// E [69]
		STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// F [70]
		STD_CTYPE_UPPER,	// G [71]
		STD_CTYPE_UPPER,	// H [72]
		STD_CTYPE_UPPER,	// I [73]
		STD_CTYPE_UPPER,	// J [74]
		STD_CTYPE_UPPER,	// K [75]
		STD_CTYPE_UPPER,	// L [76]
		STD_CTYPE_UPPER,	// M [77]
		STD_CTYPE_UPPER,	// N [78]
		STD_CTYPE_UPPER,	// O [79]
		STD_CTYPE_UPPER,	// P [80]
		STD_CTYPE_UPPER,	// Q [81]
		STD_CTYPE_UPPER,	// R [82]
		STD_CTYPE_UPPER,	// S [83]
		STD_CTYPE_UPPER,	// T [84]
		STD_CTYPE_UPPER,	// U [85]
		STD_CTYPE_UPPER,	// V [86]
		STD_CTYPE_UPPER,	// W [87]
		STD_CTYPE_UPPER,	// X [88]
		STD_CTYPE_UPPER,	// Y [89]
		STD_CTYPE_UPPER,	// Z [90]
		0,	// [ [91]
		STD_CTYPE_RDIV,		// \ [92]
		0,	// ] [93]
		0,	// ^ [94]
		STD_CTYPE_UNDERLINE,	// _ [95]
		0,	// ` [96]
		STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// a [97]
		STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// b [98]
		STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// c [99]
		STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// d [100]
		STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// e [101]
		STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// f [102]
		STD_CTYPE_LOWER,	// g [103]
		STD_CTYPE_LOWER,	// h [104]
		STD_CTYPE_LOWER,	// i [105]
		STD_CTYPE_LOWER,	// j [106]
		STD_CTYPE_LOWER,	// k [107]
		STD_CTYPE_LOWER,	// l [108]
		STD_CTYPE_LOWER,	// m [109]
		STD_CTYPE_LOWER,	// n [110]
		STD_CTYPE_LOWER,	// o [111]
		STD_CTYPE_LOWER,	// p [112]
		STD_CTYPE_LOWER,	// q [113]
		STD_CTYPE_LOWER,	// r [114]
		STD_CTYPE_LOWER,	// s [115]
		STD_CTYPE_LOWER,	// t [116]
		STD_CTYPE_LOWER,	// u [117]
		STD_CTYPE_LOWER,	// v [118]
		STD_CTYPE_LOWER,	// w [119]
		STD_CTYPE_LOWER,	// x [120]
		STD_CTYPE_LOWER,	// y [121]
		STD_CTYPE_LOWER,	// z [122]
		0,	// { [123]
		STD_CTYPE_OR,		// | [124]
		0,	// } [125]
		0,	// ~ [126]
		0,	// del [127]
	};

	typedef CharTypeT<void> CharType;





	// -------------------------------------------------------------------------
	// class IsCharType

	template<unsigned int TypeMask>
	struct IsCharType
	{
		unsigned int operator()(unsigned int c) const
		{
			return CharType::Is(TypeMask, c);
		}
	};


	// -------------------------------------------------------------------------
	// ArchiveCharTraitsT
	
	template<typename CharT>
	struct ArchiveCharTraitsT;

	template<>
	struct ArchiveCharTraitsT<char>
	{
		typedef unsigned int		Type;

		static Type Upper(Type c)
		{
			return ::toupper(c);
		}

		static Type Lower(Type c)
		{
			return ::tolower(c);
		}
	};

	template<>
	struct ArchiveCharTraitsT<wchar_t>
	{
		typedef unsigned int		Type;

		static Type Upper(Type c)
		{
			return ::toupper(c);
		}

		static Type Lower(Type c)
		{
			return ::tolower(c);
		}
	};


	// -------------------------------------------------------------------------
	// ToUpper/ToLower/CompareNoCase

	template<typename CharT>
	struct ToUpper
	{
		typedef ArchiveCharTraitsT<CharT> CharTraitsType;
		typedef typename CharTraitsType::Type Type;

		Type operator()(Type c) const
		{
			return CharTraitsType::Upper(c);
		}
	};

	template<typename CharT>
	struct ToLower
	{
		typedef ArchiveCharTraitsT<CharT> CharTraitsType;
		typedef typename CharTraitsType::Type Type;

		Type operator()(Type c) const
		{
			return CharTraitsType::Lower(c);
		}
	};

	template<typename CharT>
	struct CompareNoCase
	{
		typedef ArchiveCharTraitsT<CharT> CharTraitsType;
		typedef typename CharTraitsType::Type Type;

		Type operator()(Type c1, Type c2) const
		{
			return CharTraitsType::Upper(c1) - CharTraitsType::Upper(c2);
		}
	};

}







#endif