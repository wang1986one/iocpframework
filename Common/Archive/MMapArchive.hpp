#ifndef __ARCHIVE_MMAP_ARCHIVE_HPP
#define __ARCHIVE_MMAP_ARCHIVE_HPP


#include "MemoryArchive.hpp"

#include "../MMap.hpp"



namespace DataStructure
{
	
	//------------------------------------------------------
	// class MMapReadCache

	template<typename CharT>
	class MMapReadCache
	{
	private:
		typedef SegmentAccessBuffer<FileMappingReadOnly, 0, true>		SegmentType;
		typedef SegmentType::pos_type									pos_type;

	private:
		SegmentType		m_seg;
		pos_type		m_fileSize;


	private:
		class MMapReadCacheIterator
			: public std::iterator<std::random_access_iterator_tag, CharT>
		{
		private:
			enum { SegmentBits = SegmentType::SegmentBits };
			enum { SegmentSize = SegmentType::SegmentSize };
			enum { SegmentSizeMask = SegmentSize - 1};

		private:
			DWORD m_dwPos;
			DWORD m_dwSeg;
			MMapReadCache *m_pCache;


		public:
			MMapReadCacheIterator()
			{}
			MMapReadCacheIterator(pos_type pos, const MMapReadCache *pCache)
				: m_pCache((MMapReadCache *)pCache)
			{
				_Init(pos);
			}

		public:
			pos_type Tell() const
			{
				return m_dwSeg << SegmentBits | m_dwPos;
			}

			const CharT &operator*()
			{
				return *reinterpret_cast<const CharT *>(m_pCache->m_seg.ViewSegment(m_dwSeg) + m_dwPos);
			}

			MMapReadCacheIterator &operator++()
			{
				m_dwPos += sizeof(CharT);
				if( m_dwPos == SegmentSize )
				{
					++m_dwSeg;
					m_dwPos = 0;
				}

				return *this;
			}
			MMapReadCacheIterator &operator--()
			{
				if( m_dwPos == 0 )
				{
					--m_dwSeg;
					m_dwPos = SegmentSize - sizeof(CharT);
				}
				else
				{
					m_dwPos -= sizeof(CharT);
				}

				return *this;
			}
			MMapReadCacheIterator &operator+=(difference_type delta)
			{
				_Init(Tell() + delta * sizeof(CharT));
				return *this;
			}
			MMapReadCacheIterator &operator-=(difference_type delta)
			{
				_Init(Tell() - delta * sizeof(CharT));
				return *this;
			}

			MMapReadCacheIterator operator++(int)
			{
				MMapReadCacheIterator tmp = *this;
				++ *this;
				return tmp;
			}
			MMapReadCacheIterator operator--(int)
			{
				MMapReadCacheIterator tmp = *this;
				-- *this;
				return tmp;
			}

			friend MMapReadCacheIterator operator+(MMapReadCacheIterator iter, difference_type delta)
			{
				return iter += delta;
			}
			friend MMapReadCacheIterator operator-(MMapReadCacheIterator iter, difference_type delta)
			{
				return iter += -delta;
			}
			friend MMapReadCacheIterator operator+(difference_type delta, MMapReadCacheIterator iter)
			{
				return iter += delta;
			}
			friend MMapReadCacheIterator operator-(difference_type delta, MMapReadCacheIterator iter)
			{
				return iter += -delta;
			}
			friend difference_type operator-(const MMapReadCacheIterator &lhs, const MMapReadCacheIterator &rhs)
			{
				return (lhs.Tell() - rhs.Tell()) / sizeof(CharT);
			}

			bool operator==(const MMapReadCacheIterator &rhs) const
			{
				return m_dwPos == rhs.m_dwPos && m_dwSeg == rhs.m_dwSeg;
			}
			bool operator!=(const MMapReadCacheIterator &rhs) const
			{
				return !(*this == rhs);
			}

		private:
			void _Init(pos_type pos)
			{
				m_dwSeg = pos >> SegmentBits;
				m_dwPos	= pos & SegmentSizeMask;
			}
		};


	public:
		typedef MMapReadCacheIterator					iterator;
		typedef MMapReadCacheIterator					const_iterator;
		typedef MemReadArchive<iterator>				ArchiveType;


	public:
		MMapReadCache()
		{}

		MMapReadCache(LPCSTR szFile)
		{
			m_seg.Open(szFile, &m_fileSize);
		}

	private:
		MMapReadCache(const MMapReadCache &);
		MMapReadCache &operator=(const MMapReadCache &);

	public:
		bool Open(LPCSTR szFile)
		{
			return m_seg.Open(szFile, &m_fileSize) == S_OK;
		}

		void Close()
		{
			m_seg.Close();
		}

		bool Good() const
		{
			return m_seg.Good();
		}


	public:
		const_iterator begin() const
		{
			return const_iterator(0, this);
		}

		const_iterator end() const
		{
			return const_iterator(m_fileSize, this);
		}
	

	};


}




#endif