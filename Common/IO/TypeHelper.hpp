#ifndef __IO_TYPE_HELPER_HPP
#define __IO_TYPE_HELPER_HPP


#include "FormatType.hpp"
#include "PrintType.hpp"


namespace DataStructure
{
	// typelist for storing formatters

	template<typename H, typename T>
	struct FormatList
	{
		typedef H HeadType;
		typedef T TailType;
	};

	// helper for composing typelist
	template<
		typename F1,
		typename F2 = Internal::EmptyFormat,
		typename F3 = Internal::EmptyFormat,
		typename F4 = Internal::EmptyFormat,
		typename F5 = Internal::EmptyFormat,
		typename F6 = Internal::EmptyFormat,
		typename F7 = Internal::EmptyFormat,
		typename F8 = Internal::EmptyFormat,
		typename F9 = Internal::EmptyFormat,
		typename F10 = Internal::EmptyFormat,
		typename F11 = Internal::EmptyFormat,
		typename F12 = Internal::EmptyFormat,
		typename F13 = Internal::EmptyFormat,
		typename F14 = Internal::EmptyFormat,
		typename F15 = Internal::EmptyFormat,
		typename F16 = Internal::EmptyFormat,
		typename F17 = Internal::EmptyFormat,
		typename F18 = Internal::EmptyFormat,
		typename F19 = Internal::EmptyFormat
	>
	struct Formatter
	{
		typedef F1	HeadType;
		typedef FormatList<F2,
			FormatList<F3,
			FormatList<F4,
			FormatList<F5,
			FormatList<F6,
			FormatList<F7,
			FormatList<F8,
			FormatList<F9,
			FormatList<F10,
			FormatList<F11,
			FormatList<F12,
			FormatList<F13,
			FormatList<F14,
			FormatList<F15,
			FormatList<F16,
			FormatList<F17,
			FormatList<F18,
			FormatList<F19,
			FormatList<Internal::EmptyFormat, void>
			> > > > > > > > > > > > > > > > > > TailType;
	};


	// helper typedef for basic formatters typelist
	typedef Formatter<BoolFormat, 
		CharFormat, 
		WCharFormat,
		CharPointerFormat, 
		WCharPointerFormat,
		StringAFormat,
		StringWFormat,
		IntFormat,
		UIntFormat,
		ShortFormat,
		UShortFormat,
		LongFormat,
		ULongFormat,
		FloatFormat,
		DoubleFormat> BasicFormat;

	// helper typedef for printf formatters typelist
	typedef Formatter<BoolPrintfFormat,
		CharPrintfFormat, 
		CharPointerPrintfFormat, 
		StringAPrintfFormat,
		IntPrintfFormat,
		UIntPrintfFormat,
		ShortPrintfFormat,
		UShortPrintfFormat,
		LongPrintfFormat,
		ULongPrintfFormat,
		FloatPrintfFormat,
		DoublePrintfFormat> PrintfFormat;



	namespace Internal
	{
		// helper for seaching in the typelist
		template<typename T1, typename T2, typename FormatT, typename TailT>
		struct SearchList
		{
			typedef typename SearchList<
				typename TailT::HeadType::FormatType,
				T2, 
				typename TailT::HeadType,
				typename TailT::TailType>::FormatType FormatType;
		};

		// Special case for type match
		template<typename T, typename FormatT, typename TailT>
		struct SearchList<T, T, FormatT, TailT>
		{
			typedef FormatT FormatType;
		};

		// Special case for end-of-list
		template<typename T, typename TailT>
		struct SearchList<void, T, EmptyFormat, TailT>
		{
			typedef EmptyFormat	FormatType;
		};


		// class FindFormatType
		template<typename FormatT, typename T>
		struct FindFormatType
		{
			typedef typename SearchList<
				typename FormatT::HeadType::FormatType,
				T,
				typename FormatT::HeadType,
				typename FormatT::TailType>::FormatType	FormatType;
		};


		// General case(no array, no const, no reference)
		template<typename T>
		struct InspectType
		{
			typedef T value_type;
		};

		template<typename T>
		struct InspectType<T &>
		{
			typedef T value_type;
		};

		template<typename T>
		struct InspectType<const T &>
		{
			typedef T value_type;
		};

		template<typename T>
		struct InspectType<const T *>
		{
			typedef T * value_type;
		};

		// array case
		template<typename T, size_t _N>
		struct InspectType<T[_N]>
		{
			typedef T * value_type;
		};

		template<typename T, size_t _N>
		struct InspectType<const T[_N]>
		{
			typedef T * value_type;
		};

		template<typename T>
		struct InspectType<T[]>
		{
			typedef T * value_type;
		};

		template<typename T>
		struct InspectType<const T[]>
		{
			typedef T * value_type;
		};

	}
}




#endif