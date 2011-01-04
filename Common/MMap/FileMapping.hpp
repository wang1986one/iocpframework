#ifndef __MMAP_FILEMAPPING_HPP
#define __MMAP_FILEMAPPING_HPP



#include "MMap.hpp"
#include "AccessBuffer.hpp"



namespace DataStructure
{

	// -------------------------------------------------------------------------
	// class HandleProxy	

	template<typename Owner>
	class HandleProxy
	{
	public:
		typedef typename Owner::size_type	size_type;
		typedef typename Owner::pos_type	pos_type;
		typedef typename Owner::Utils		Utils;

	private: 
		Owner &m_owner;

	public:
		enum { AllocationGranularityBits= Owner::AllocationGranularityBits };
		enum { AllocationGranularity	= Owner::AllocationGranularity	   };
		enum { AllocationGranularityMask= Owner::AllocationGranularityMask };
	
	public:
		HandleProxy(Owner &owner)
			: m_owner(owner)
		{}

	public:
		char *ViewSegment(DWORD dwBasePage, DWORD dwPageCount)
		{
			return m_owner.ViewSegment(dwBasePage, dwPageCount);
		}

		char *AccessSegment(DWORD dwBasePage, DWORD dwPageCount)
		{
			return m_owner.AccessSegment(dwBasePage, dwPageCount);
		}

		char *AllocSegment(DWORD dwBasePage, DWORD dwPageCount)
		{
			return m_owner.AllocSegment(dwBasePage, dwPageCount);
		}
	};


	// -------------------------------------------------------------------------
	// class FileMapping

	// 对MapFile的一层包装，提供边界对齐的保护操作

	template<typename ConfigT>
	class FileMapping:
		public MapFile<ConfigT>
	{
	private:
		typedef MapFile<ConfigT>					BaseClass;

	public:
		typedef typename BaseClass::size_type		size_type;
		typedef typename BaseClass::pos_type		pos_type;

		typedef HandleProxy<FileMapping>			HandleType;
		typedef BaseClass							Utils;

	private:
		DWORD m_dwTotalPage;

		enum { _AGBits = 16 };
		// 分配粒度
		enum { _AllocationGranularityInvBits = sizeof(DWORD) * 8 - _AGBits };


	public:
		enum { AllocationGranularityBits	= _AGBits };
		enum { AllocationGranularity		= (1 << _AGBits) };
		enum { AllocationGranularityMask	= (AllocationGranularity - 1) };

	private:
		FileMapping(const FileMapping &);
		FileMapping &operator=(const FileMapping &);


	public:
		FileMapping()
			: m_dwTotalPage(0)
		{}
		template<typename T>
		FileMapping(T szFile, pos_type *offset = NULL)
		{
			Open(szFile, offset);
		}

	public:
		DWORD GetTotalPages() const
		{
			return m_dwTotalPage;
		}

		void Close()
		{
			BaseClass::Close();
			m_dwTotalPage = 0;
		}

		bool Resize(pos_type size)
		{
			m_dwTotalPage = (size + AllocationGranularityMask) >> AllocationGranularityBits;
	
			return BaseClass::Resize(size);
		}

		template<typename T>
		bool Open(const T szFile, pos_type *offset = NULL)
		{
			if( ConfigT::GetSizeOnOpen )
			{
				pos_type size = 0;
				bool hRes = BaseClass::Open(szFile, &size);

				m_dwTotalPage = static_cast<DWORD>((size + AllocationGranularityMask) >> AllocationGranularityBits);

				if( offset != NULL )
					*offset = size;

				return hRes;
			}
			else
			{
				m_dwTotalPage = 0;
				return BaseClass::Open(szFile, NULL);
			}
		}

		char *ViewSegment(DWORD dwBasePage, DWORD dwPageCount)
		{
			assert(BaseClass::Good());

			if( dwBasePage + dwPageCount > m_dwTotalPage )
			{
				if( dwBasePage >= m_dwTotalPage )
					return NULL;
				else
					dwPageCount = m_dwTotalPage - dwBasePage;
			}

			return reinterpret_cast<char *>(BaseClass::Map(dwBasePage << AllocationGranularityBits, dwPageCount << AllocationGranularityBits));
		}

		char *AccessSegment(DWORD dwBasePage, DWORD dwPageCount)
		{
			assert(BaseClass::Good());

			if( dwBasePage + dwPageCount > m_dwTotalPage )
			{
				m_dwTotalPage = dwBasePage + dwPageCount;

				BaseClass::Resize(m_dwTotalPage << AllocationGranularityBits);
			}

			return reinterpret_cast<char *>(BaseClass::Map(dwBasePage << AllocationGranularityBits, dwPageCount << AllocationGranularityBits));
		}

		char *AllocSegment(DWORD dwBasePage, DWORD dwPageCount)
		{
			assert(BaseClass::Good());

			dwBasePage += m_dwTotalPage;
			m_dwTotalPage += dwPageCount;

			BaseClass::Resize(m_dwTotalPage << AllocationGranularityBits);

			return reinterpret_cast<char *>(BaseClass::Map(dwBasePage << AllocationGranularityBits, dwPageCount << AllocationGranularityBits));
		}
		
	};


	typedef FileMapping<MappingReadWrite> FileMappingReadWrite;
	typedef FileMapping<MappingReadOnly>  FileMappingReadOnly;
}





#endif