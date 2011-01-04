#ifndef __ARCHIVE_RECORD_HPP
#define __ARCHIVE_RECORD_HPP



namespace DataStructure
{
	//-----------------------------------------------------------
	// class MemRecordArchive

	struct RecordHeaderType
	{
		enum { RecordEOF = 0 };

		unsigned int m_nRecordID;
		unsigned int m_nData;
	};


	template<typename BaseWriterT, typename HeaderT = RecordHeaderType>
	class RecordWriter
		: public BaseWriterT
	{
	private:
		typedef BaseWriterT						BaseType;

	public:
		typedef HeaderT							HeaderType;
		typedef typename BaseType::pos_type		pos_type;

		enum { RecordEOF = HeaderType::RecordEOF };

	private:
		unsigned int m_nRecordID;
		pos_type m_offset;

	public:
		RecordWriter()
			: m_nRecordID(RecordEOF)
		{}

		template<typename InitArgT>
		explicit RecordWriter(InitArgT &file)
			: BaseType(file)
			, m_nRecordID(RecordEOF)
		{}
		template<typename InitArgT>
		explicit RecordWriter(const InitArgT &file)
			: BaseType(file)
			, m_nRecordID(RecordEOF)
		{}
		template<typename InitArgT1, typename InitArgT2>
		RecordWriter(InitArgT1 &arg1, InitArgT2 &arg2)
			: BaseType(arg1, arg2)
			, m_nRecordID(RecordEOF)
		{}
		template<typename InitArgT1, typename InitArgT2>
		RecordWriter(const InitArgT1 &arg1, const InitArgT2 &arg2)
			: BaseType(arg1, arg2)
			, m_nRecordID(RecordEOF)
		{}
		template<typename InitArgT1, typename InitArgT2, typename InitArgT3>
		RecordWriter(InitArgT1 &arg1, InitArgT2 &arg2, InitArgT3 &arg3)
			: BaseType(arg1, arg2, arg3)
			, m_nRecordID(RecordEOF)
		{}
		template<typename InitArgT1, typename InitArgT2, typename InitArgT3>
		RecordWriter(const InitArgT1 &arg1, const InitArgT2 &arg2, InitArgT3 &arg3)
			: BaseType(arg1, arg2, arg3)
			, m_nRecordID(RecordEOF)
		{}

	public:
		bool BeginRecord(unsigned int nRecordID)
		{
			assert(nRecordID == RecordEOF);

			HeaderType hdr = {RecordEOF, 0};
				
			BaseType::Put(reinterpret_cast<const char *>(&hdr), sizeof(HeaderType));
			m_nRecordID = nRecordID;
			m_offset = BaseType::Tell();

			return true;
		}

		bool EndRecord()
		{
			const unsigned int nData = BaseType::Tell() - m_offset;
			HeaderType hdr = {m_nRecordID, nData};

			BaseType::Reput(nData + sizeof(HeaderType), reinterpret_cast<const char *>(&hdr), sizeof(HeaderType));
			m_nRecordID = RecordEOF;

			return true;
		}
	};


	//-------------------------------------------------------------
	// class RecordInfo

	template<typename HeaderT>
	struct RecordInfo
		: public HeaderT
	{
		typedef HeaderT			HeaderType;
	public:
		typedef char			value_type;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;
		typedef const char *	pointer;
		typedef const char *	const_pointer;
		typedef const char *	iterator;
		typedef const char *	const_iterator;

	public:
		const char *m_pData;

	public:
		const_iterator begin() const
		{
			return m_pData;
		}

		const_iterator end() const
		{
			return m_pData + HeaderType::m_nData;
		}
	};


	//-------------------------------------------------------------
	// class RecordReader

	template<typename ReadArchiveT, typename HeaderT = RecordHeaderType>
	class RecordReader
		: private ReadArchiveT
	{
	private:
		typedef ReadArchiveT						BaseType;
		
	public:
		typedef HeaderT								HeaderType;
		typedef RecordInfo<HeaderType>				RecordInfoType;
		typedef typename ReadArchiveT::char_type		char_type;

		enum { RecordEOF = HeaderType::RecordEOF };


	public:
		RecordReader()
		{}

		template<typename InitArgT>
		explicit RecordReader(InitArgT &file)
			: BaseType(file)
		{}
		template<typename InitArgT>
		explicit RecordReader(const InitArgT &file)
			: BaseType(file)
		{}

		template<typename ArgT1, typename ArgT2>
		RecordReader(ArgT1 &arg1, ArgT2 &arg2)
			: BaseType(arg1, arg2)
		{}
		template<typename ArgT1, typename ArgT2>
		RecordReader(const ArgT1 &arg1, const ArgT1 &arg2)
			: BaseType(arg1, arg2)
		{}

		template<typename ArgT1, typename ArgT2, typename ArgT3>
		RecordReader(ArgT1 &arg1, ArgT2 &arg2, ArgT3 &arg3)
			: BaseType(arg1, arg2, arg3)
		{}
		template<typename ArgT1, typename ArgT2, typename ArgT3>
		RecordReader(const ArgT1 &arg1, const ArgT1 &arg2, const ArgT3 &arg3)
			: BaseType(arg1, arg2, arg3)
		{}


	public:
		const char_type *NextRecord(HeaderType &hdr)
		{
			const HeaderType *pHdr = reinterpret_cast<const HeaderType *>(BaseType::Get(sizeof(HeaderType)));
			if( pHdr == NULL )
			{
				hdr.m_nRecordID = RecordEOF;
				hdr.m_nData = 0;

				return NULL;
			}

			hdr = *pHdr;
			return BaseType::Get(hdr.m_nData);
		}

		const char_type *Next(RecordInfo &info)
		{
			info.m_pData = NextRecord(info);
			return info.m_pData;
		}
	};

}






#endif