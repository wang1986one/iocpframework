#ifndef __ARCHIVE_BINARY_HPP
#define __ARCHIVE_BINARY_HPP


#include "Basic.hpp"



namespace DataStructure
{
	namespace IO
	{


		// -------------------------------------------------------------------------
		// BinaryWriter 

		template<typename WriteArchiveT>
		inline void Put16I(WriteArchiveT &ar, UINT16 val)
		{
			//_ByteSwap16(val);

			ar.Put(reinterpret_cast<const char *>(&val), sizeof(val));
		}

		template<typename WriteArchiveT>
		inline void Put32I(WriteArchiveT &ar, UINT32 val)
		{
			//_ByteSwap32(val);

			ar.Put(reinterpret_cast<const char *>(&val), sizeof(val));
		}

		template<typename WriteArchiveT>
		inline void Put64I(WriteArchiveT &ar, UINT64 val)
		{
			//_ByteSwap64(val);

			ar.Put(reinterpret_cast<const char *>(&val), sizeof(val));
		}

		template<typename WriteArchiveT, typename StructT>
		inline void PutStruct(WriteArchiveT &ar, const StructT &val)
		{
			//_ByteSwapStruct(val);

			ar.Put(reinterpret_cast<const char *>(&val), sizeof(val));
		}


		template<typename WriteArchiveT, size_t N>
		inline void Put16I(WriteArchiveT &ar, const UINT16 (&arr)[N])
		{
			for(size_t i = 0; i != N; ++i)
				Put16I(ar, arr[i]);
		}

		template<typename WriteArchiveT, size_t N>
		inline void Put32I(WriteArchiveT &ar, const UINT32 (&arr)[N])
		{
			for(size_t i = 0; i != N; ++i)
				Put32I(ar, arr[i]);
		}

		template<typename WriteArchiveT, typename StructT, size_t N>
		inline void PutStruct(WriteArchiveT &ar, const StructT (&arr)[N])
		{
			for(size_t i = 0; i != N; ++i)
				PutStruct(ar, arr[i]);
		}


		// -------------------------------------------------------------------------
		// BinaryReader 

		template<typename ReadArchiveT>
		inline bool Get16I(ReadArchiveT &ar, UINT16 &val)
		{
			if( ar.Get(reinterpret_cast<char *>(&val), sizeof(val)) == sizeof(val) )
				return true;

			return false;
		}

		template<typename ReadArchiveT>
		inline bool Get32I(ReadArchiveT &ar, UINT32 &val)
		{
			if( ar.Get(reinterpret_cast<char *>(&val), sizeof(val)) == sizeof(val) )
				return true;

			return false;
		}

		template<typename ReadArchiveT, typename StructT>
		inline bool GetStruct(ReadArchiveT &ar, StructT &val)
		{
			if( ar.Get(reinterpret_cast<char *>(&val), sizeof(val)) == sizeof(val) )
				return true;

			return false;
		}


		template<typename ReadArchiveT, size_t N>
		inline void Get16I(ReadArchiveT &ar, const UINT16 (&arr)[N])
		{
			for(size_t i = 0; i != N; ++i)
			{
				if( !Get16I(ar, arr[i]) )
					return false;
			}

			return true;
		}

		template<typename ReadArchiveT, size_t N>
		inline void Get32I(ReadArchiveT &ar, const UINT32 (&arr)[N])
		{
			for(size_t i = 0; i != N; ++i)
			{
				if( !Get32I(ar, arr[i]) )
					return false;
			}

			return true;
		}

		template<typename ReadArchiveT, typename StructT, size_t N>
		inline void GetStruct(ReadArchiveT &ar, const StructT (&arr)[N])
		{
			for(size_t i = 0; i != N; ++i)
			{
				if( !GetStruct(ar, arr[i]) )
					return false;
			}

			return true;
		}



		template<typename ReadArchiveT>
		inline bool Peek16I(ReadArchiveT &ar, UINT16 &val)
		{
			const char *p = ar.Peek(sizeof(val));
			if( p != NULL )
			{
				val = *reinterpret_cast<const UINT16 *>(p);
				return true;
			}

			return false;
		}

		template<typename ReadArchiveT>
		inline bool Peek32I(ReadArchiveT &ar, UINT32 &val)
		{
			const char *p = ar.Peek(sizeof(val));
			if( p != NULL )
			{
				val = *reinterpret_cast<const UINT32 *>(p);
				return true;
			}

			return false;
		}

		template<typename ReadArchiveT, size_t N>
		inline bool Get16I(ReadArchiveT &ar, UINT16 (&arr)[N])
		{
			const size_t nRead = sizeof(UINT16) * N;
			return ar.Get(reinterpret_cast<char *>(arr), nRead) == nRead;
		}

		template<typename ReadArchiveT, size_t N>
		inline bool Get32I(ReadArchiveT &ar, UINT32 (&arr)[N])
		{
			const size_t nRead = sizeof(UINT32) * N;
			return ar.Get(reinterpret_cast<char *>(arr), nRead) == nRead;
		}

		template<typename ReadArchiveT, typename StructT, size_t N>
		inline bool GetStruct(ReadArchiveT &ar, StructT (&arr)[N])
		{
			const size_t nRead = sizeof(StructT) * N;
			return ar.Get(reinterpret_cast<char *>(arr), nRead) == nRead;
		}
	

		// -------------------------------------------------------------------------
		// Binary String Writer

		template<typename WriteArchiveT>
		inline void Puts(WriteArchiveT &ar, const char *pBuf, size_t nSize)
		{
			if( nSize < 254 )
				ar.Put(static_cast<UINT8>(nSize));
			else if( nSize < 65536 )
			{
				ar.Put(static_cast<UINT8>(254));
				Put16I(ar, static_cast<UINT16>(nSize));
			}
			else
			{
				ar.Put(static_cast<UINT8>(255));
				Put16I(ar, static_cast<UINT32>(nSize));
			}

			ar.Put(pBuf, nSize);
			if( !(nSize & 1) )
				ar.Put(static_cast<UINT8>(0));
		}

		template<typename WriteArchiveT>
		inline void Puts(WriteArchiveT &ar, const String &str)
		{
			Puts(ar, str.data(), str.size());
		}


		template<typename WriteArchiveT>
		inline void WPuts(WriteArchiveT &ar, const UINT16 *pBuf, size_t nSize)
		{
			if( nSize < 65535 )
				Put16I(ar, static_cast<UINT16>(nSize));
			else
			{
				Put16I(ar, static_cast<UINT16>(65535));
				Put32I(ar, static_cast<UINT32>(nSize));
			}

			Put16I(ar, pBuf, nSize);
		}

		template<typename WriteArchiveT>
		inline void WPuts(WriteArchiveT &ar, const std::basic_string<UINT16> &str)
		{
			WPuts(ar, str.data(), str.size());
		}



		//--------------------------------------------------------------------------
		// Binary String Reader

		template<typename ReadArchiveT, typename StringT>
		inline bool Gets(ReadArchiveT &ar, StringT &str)
		{
			typedef typename ReadArchiveT::size_type size_type;
			bool bSuc = false;

			size_type nSize = static_cast<size_type>(ar.Get());
			if( nSize < 254 )
			{
				// Do nothing
			}
			else if( nSize == 254 )
			{
				UINT16 tmp = 0;
				if( !Get16I(ar, tmp) )
					return false;

				nSize = tmp;
			}
			else if( nSize == 255 )
			{
				INT32 tmp = 0;
				if( !Get32I(ar, tmp) || tmp < 0 )
					return false;

				nSize = tmp;
			}
			else
				return false;

			if( ar.Get(str.resize(nSize) != nSize) )
				return false;
			if( !(nSize & 1) )
				ar.Get();	// Padding

			return true;
		}

		
		template<typename ReadArchiveT, typename StringT>
		inline bool WGets(ReadArchiveT &ar, StringT &str)
		{
			typedef typename ReadArchiveT::size_type size_type;
			bool bSuc = false;

			size_type nSize = 0;
			UINT16 tmp = 0;
			if( !Get16I(ar, tmp) )
			{
				return false;
			}
			else if( tmp == 65535 )
			{
				UINT32 tmp32 = 0;
				if( !Get32I(ar, tmp32) || tmp32 < 0 )
					return false;

				nSize = tmp;
			}
			else 
				nSize = tmp;

			return Get16I(ar, str.resize(nSize), nSize);
		}

	}


}


#endif