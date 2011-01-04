#ifndef __IO_STREAM_HPP
#define __IO_STREAM_HPP



#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>

#include <iterator>
#include <algorithm>
#include <type_traits>

#include "../Constraints.hpp"

#include "TypeHelper.hpp"
#include "FileStream.hpp"
#include "MemoryStream.hpp"
#include "SequenceStream.hpp"
#include "BufferStream.hpp"
#include "DynamicStream.hpp"

#include "StreamIterator.hpp"

namespace DataStructure
{
	
	/*
	
	Writer <--> UnCheckedMemoryWriter		BoundedMemoryWriter		SequenceWriter .... 
	  ^
	  |
	  |				  FormatType
	  |					  ^
	  |					  |
	FastOStream	<------  Write



	Reader <--> UnCheckedMemoryReader	BoundedMemoryReader		SequenceReader .... 
	^
	|
	|				  FormatType
	|					  ^
	|					  |
	FastIStream	<------  Read
	 
	*/




	//-------------------------------------------------------
	// class FastOstream

	template<typename FormatT, typename WriterT>
	class FastOStream
		: public WriterT
	{
	public:
		typedef WriterT							BaseType;
		typedef typename BaseType::char_type	char_type;

	public:
		FastOStream()
		{}

		template<typename T>
		explicit FastOStream(T &val)
			: BaseType(val)
		{}
		template<typename T>
		explicit FastOStream(const T &val)
			: BaseType(val)
		{}
		
		template<typename T1, typename T2>
		FastOStream(T1 &val1, T2 &val2)
			: BaseType(val1, val2)
		{}
		template<typename T1, typename T2>
		FastOStream(const T1 &val1, const T2 &val2)
			: BaseType(val1, val2)
		{}

		template<typename T1, typename T2, typename T3>
		FastOStream(T1 &val1, T2 &val2, T3 &val3)
			: BaseType(val1, val2, val3)
		{}
		template<typename T1, typename T2, typename T3>
		FastOStream(const T1 &val1, const T2 &val2, const T3 &val3)
			: BaseType(val1, val2, val3)
		{}

	private:
		FastOStream(const FastOStream &);
		FastOStream &operator=(const FastOStream &);

	public:
		// Insert
		template<typename T>
		FastOStream &Insert(const T &val)
		{
			// Ñ°ÕÒ×îºÏÊÊµÄFormatter
			typedef typename Internal::InspectType<T>::value_type						Type;
			typedef typename Internal::FindFormatType<FormatT, Type>::FormatType		FormatType;
	
			FormatType::Write<char_type>(val, *this);

			return *this;
		}
	
		operator bool() const
		{
			return BaseType::Good();
		}

		// Support endl,ends
		FastOStream &operator<<(FastOStream& (__cdecl *pFun)(FastOStream&))
		{
			return pFun(*this);
		}
	};


	//---------------------------------------------------------
	// class FastIStream

	template<typename FormatT, typename ReaderT>
	class FastIStream
		: public ReaderT
	{
	public:
		typedef FormatT							FormatType;
		typedef ReaderT							BaseType;
		typedef typename BaseType::char_type	char_type;

	public:
		FastIStream()
		{}

		template<typename T>
		explicit FastIStream(T &val)
			: BaseType(val)
		{}
		template<typename T>
		explicit FastIStream(const T &val)
			: BaseType(val)
		{}

		template<typename T1, typename T2>
		FastIStream(T1 &val1, T2 &val2)
			: BaseType(val1, val2)
		{}
		template<typename T1, typename T2>
		FastIStream(const T1 &val1, const T2 &val2)
			: BaseType(val1, val2)
		{}

		template<typename T1, typename T2, typename T3>
		FastIStream(T1 &val1, T2 &val2, T3 &val3)
			: BaseType(val1, val2, val3)
		{}
		template<typename T1, typename T2, typename T3>
		FastIStream(const T1 &val1, const T2 &val2, const T3 &val3)
			: BaseType(val1, val2, val3)
		{}

	private:
		FastIStream(const FastIStream &);
		FastIStream &operator=(const FastIStream &);

	public:
		template<typename T>
		FastIStream &Extract(T &t)
		{
			typedef typename Internal::InspectType<T>::value_type					Type;
			typedef typename Internal::FindFormatType<FormatT, Type>::FormatType	FormatType;
			
			FormatType::Read<char_type>(t, *this);
			return *this;
		}

		operator bool() const
		{
			return BaseType::Good();
		}
	};


	// Generic stream inserter
	template<typename FormatT, typename WriterT, typename T>
	inline FastOStream<FormatT, WriterT> &operator<<(FastOStream<FormatT, WriterT> &os, const T &val)
	{
		return os.Insert(val);
	}

	// Generic stream extractor
	template<typename FormatT, typename ReaderT, typename T>
	inline FastIStream<FormatT, ReaderT> &operator>>(FastIStream<FormatT, ReaderT> &in, T &t)
	{
		return in.Extract(t);
	}

	
	// Helper manipulator
	template<typename FormatT, typename ReaderT>
	inline FastOStream<FormatT, ReaderT> &Endl(FastOStream<FormatT, ReaderT> &os)
	{
		os << '\n';
		return os;
	}

	template<typename FormatT, typename ReaderT>
	inline FastOStream<FormatT, ReaderT> &Ends(FastOStream<FormatT, ReaderT> &os)
	{
		os << '\0';
		return os;
	}



	// FileStream
	typedef FastOStream<BasicFormat, FileWriter>						FileOStream;
	typedef FastIStream<BasicFormat, FileReader>						FileIStream;

	// MemoryStream

	typedef FastOStream<BasicFormat, UnCheckedMemoryWriter<char>>		UnCheckedOStream;
	typedef FastOStream<BasicFormat, BoundedMemoryWriter<char>>			CheckedOStream;
	typedef FastIStream<BasicFormat, UnCheckedMemoryReader<char>>		UnCheckedIStream;
	typedef FastIStream<BasicFormat, BoundedMemoryReader<char>>			CheckedIStream;

	typedef FastOStream<BasicFormat, UnCheckedMemoryWriter<wchar_t>>	UnCheckedOStreamW;
	typedef FastOStream<BasicFormat, BoundedMemoryWriter<wchar_t>>		CheckedOStreamW;
	typedef FastIStream<BasicFormat, UnCheckedMemoryReader<wchar_t>>	UnCheckedIStreamW;
	typedef FastIStream<BasicFormat, BoundedMemoryReader<wchar_t>>		CheckedIStreamW;


	// SequenceStreamhai

	typedef FastOStream<BasicFormat, SequenceWriter<std::string>>		StringOStream;
	typedef FastOStream<BasicFormat, SequenceWriter<std::wstring>>		StringOStreamW;

	typedef FastIStream<BasicFormat, SequenceReader<std::string>>		StringIStream;
	typedef FastIStream<BasicFormat, SequenceReader<std::wstring>>		StringIStreamW;


	// BufferStream

	typedef FastOStream<BasicFormat, BufferWriter<char, 256, UnCheckedMemoryWriter<char>>>			BufferUnCheckedOStream;
	typedef FastOStream<BasicFormat, BufferWriter<char, 256, BoundedMemoryWriter<char>>>			BufferCheckedOStream;
	typedef FastOStream<BasicFormat, BufferWriter<wchar_t, 256, UnCheckedMemoryWriter<wchar_t>>>	BufferUnCheckedOStreamW;
	typedef FastOStream<BasicFormat, BufferWriter<wchar_t, 256, BoundedMemoryWriter<wchar_t>>>		BufferCheckedOStreamW;

	typedef FastIStream<BasicFormat, BufferReader<char, 256, UnCheckedMemoryReader<char>>>			BufferUnCheckedIStream;
	typedef FastIStream<BasicFormat, BufferReader<char, 256, BoundedMemoryReader<char>>>			BufferCheckedIStream;
	typedef FastIStream<BasicFormat, BufferReader<wchar_t, 256, UnCheckedMemoryReader<wchar_t>>>	BufferUnCheckedIStreamW;
	typedef FastIStream<BasicFormat, BufferReader<wchar_t, 256, BoundedMemoryReader<wchar_t>>>		BufferCheckedIStreamW;


	// DynamicStream

	typedef FastOStream<BasicFormat, DynamicWriter<char>>				DynamicOStream;
	typedef FastOStream<BasicFormat, DynamicWriter<wchar_t>>			DynamicOStreamW;

	typedef FastIStream<BasicFormat, DynamicReader<char>>				DynamicIStream;
	typedef FastIStream<BasicFormat, DynamicReader<wchar_t>>			DynamicIStreamW;
}





#endif