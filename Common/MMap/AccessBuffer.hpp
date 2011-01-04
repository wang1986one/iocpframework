#ifndef __MMAP_ACCESS_BUFFER_HPP
#define __MMAP_ACCESS_BUFFER_HPP



namespace DataStructure
{

	// -------------------------------------------------------------------------
	// class SegmentAllocBuffer 
	//
	// 可作为Allocator
	// 
	// 	SegmentSize = (1 << SegBits) is maximum bytes that user can allocate.
	//

	#pragma pack(1)
	struct SegmentAllocInfo
	{
		DWORD m_dwBasePage;		// index number of current block for allocating
		DWORD m_cbFree;			// free bytes of current block
	};
	#pragma pack()



	template<typename Base, int SegBits = 16>
	class SegmentAllocBuffer:
		public Base,
		private SegmentAllocInfo
	{
	public:
		typedef typename Base::size_type	size_type;
		typedef typename Base::pos_type		pos_type;

	private:
		char *m_pView;
	
	public:
		enum { AllocationGranularityBits = Base::AllocationGranularityBits };
		enum { SegmentBits = SegBits > AllocationGranularityBits ? SegBits : AllocationGranularityBits };
		enum { SegmentSize = 1 << SegmentBits };
		enum { SegmentSizeMax = SegmentSize - 1 };

	private:
		enum { AlignBits_ = SegmentBits - AllocationGranularityBits };
		enum { AlignSize_ = 1 << AlignBits_ };


		typedef typename Base::Utils		Utils;

	public:
		SegmentAllocBuffer()
			: m_pView(NULL)
		{
			m_dwBasePage	= 0;
			m_cbFree		= 0;
		}
		template<typename T1>
		SegmentAllocBuffer(T1 &arg1)
			: m_pView(NULL)
			, Base(arg1)
		{
			m_dwBasePage	= 0;
			m_cbFree		= 0;
		}
		template<typename T1>
		SegmentAllocBuffer(const T1 &arg1)
			: m_pView(NULL)
			, Base(arg1)
		{
			m_dwBasePage	= 0;
			m_cbFree		= 0;
		}
		~SegmentAllocBuffer()
		{
			if( m_pView != NULL )
				Utils::Unmap(m_pView, SegmentSize);
		}
	
	private:
		SegmentAllocBuffer(const SegmentAllocBuffer &);
		SegmentAllocBuffer &operator=(const SegmentAllocBuffer &);

	
	public:
		void Close()
		{
			if( m_pView != NULL )
			{
				Utils::Unmap(m_pView, SegmentSize);
				m_pView = NULL;
				m_cbFree = 0;

				Base::Close();
			}
		}

		void Flush()
		{
			if( m_pView != NULL )
			{
				Utils::Flush(m_pView, SegmentSize);
			}
		}

		bool InitBuffer(DWORD dwBasePage = 0)
		{
			assert(m_pView == NULL && m_cbFree == 0);

			m_pView = Base::AccessSegment(dwBasePage, AlignSize_);
			if( m_pView != NULL )
			{
				m_dwBasePage	= dwBasePage;
				m_cbFree		= SegmentSize;
				
				return true;
			}

			return false;
		}

		bool InitBuffer(const SegmentAllocInfo &info)
		{
			assert(m_pView == NULL && m_cbFree == 0);
			
			m_pView = Base::AccessSegment(info.m_dwBasePage, AlignSize_);
			if( m_pView != NULL )
			{
				*static_cast<SegmentAllocInfo *>(this) = info;
				return true;
			}

			return false;
		}


		void GetAllocInfo(SegmentAllocInfo &info) const 
		{
			info = *static_cast<const SegmentAllocInfo *>(this);
		}
		const SegmentAllocInfo &GetAllocInfo() const
		{
			return *static_cast<const SegmentAllocInfo *>(this);
		}

		char *AllocData(size_type size, pos_type &pos)
		{
			assert(SegmentBits >= AllocationGranularityBits);
			assert(size <= SegmentSize);

			while( m_pView != NULL )
			{	
				if( size <= m_cbFree )
				{
					char *pData = m_pView + (SegmentSize - m_cbFree);
					pos = (m_dwBasePage << AllocationGranularityBits) + (SegmentSize - m_cbFree);
					m_cbFree -= size;

					return pData;
				}

				if( size <= SegmentSize )
				{
					if( m_pView != NULL )
						Utils::Unmap(m_pView, SegmentSize);

					m_pView = Base::AllocSegment(AlignSize_, m_dwBasePage);
					assert(m_pView);

					if( m_pView != NULL )
					{
						m_cbFree = SegmentSize;
						continue;
					}

					m_cbFree = 0;
				}

				return NULL;
			}
		}
	};



	// -------------------------------------------------------------------------
	// class SegmentAccessBuffer
	//
	// 访问数据
	//	
	// SegmentSize = (1<<SegBits) is maximum bytes that user can access.
	// And user can't access memory across	the boundary of Segments.
	//
	template<typename Base, int SegBits = 16, bool ReadOnly = false>
	class SegmentAccessBuffer
		: public Base
	{
	private:
		char* m_pView;
		DWORD m_dwSeg;

	public:
		typedef typename Base::size_type size_type;
		typedef typename Base::pos_type  pos_type;

	public:
		enum { AllocationGranularityBits = Base::AllocationGranularityBits };
		enum { SegmentBits = SegBits > AllocationGranularityBits ? SegBits : AllocationGranularityBits };
		enum { SegmentSize = 1 << SegmentBits };
		enum { SegmentSizeMask = SegmentSize - 1 };

	private:
		enum { AlignBits_ = SegmentBits - AllocationGranularityBits };
		enum { AlignSize_ = 1 << AlignBits_ };

		typedef typename Base::Utils Utils;

	public:
		SegmentAccessBuffer() 
			: m_pView(NULL)
			, m_dwSeg(-1L) 
		{}

		template<typename T1>
		explicit SegmentAccessBuffer(T1& arg1)
			: m_pView(NULL)
			, m_dwSeg(-1L)
			, Base(arg1)
		{}

		template<typename T1>
		explicit SegmentAccessBuffer(const T1& arg1)
			: m_pView(NULL)
			, m_dwSeg((DWORD)-1)
			, Base(arg1) 
		{}

		~SegmentAccessBuffer()
		{
			if( m_pView != NULL )
				Utils::Unmap(m_pView, SegmentSize);
		}

	private:
		SegmentAccessBuffer(const SegmentAccessBuffer&);
		SegmentAccessBuffer &operator=(const SegmentAccessBuffer&);

	public:
		char* GetCurrentView() const
		{
			return m_pView;
		}

		DWORD GetCurrentSeg() const
		{
			return m_dwSeg;
		}

		void Close()
		{
			if( m_pView != NULL )
			{
				Utils::Unmap(m_pView, SegmentSize);
				m_pView = NULL;
				m_dwSeg = -1L;

				Base::Close();
			}
		}

		void Flush()
		{
			if( m_pView != NULL )
				Utils::Flush(m_pView, SegmentSize);
		}

		char* ViewSegment(DWORD dwSeg)
		{
			return dwSeg == m_dwSeg ? m_pView : _ViewSegment(dwSeg);
		}

		char* View(pos_type fc)
		{
			pos_type dwSeg = fc >> SegmentBits;
			return dwSeg == m_dwSeg ? m_pView + (SegmentSizeMask & static_cast<size_type>(fc)) : _View(dwSeg, fc);
		}


	private:
		char* _ViewSegment(DWORD dwSeg)
		{
			return _View(dwSeg, 0);
		}

		char* _View(DWORD dwSeg, pos_type fc)
		{
			if( m_pView != NULL )
				Utils::Unmap(m_pView, SegmentSize);

			m_pView = _Access(dwSeg, Int2Type<ReadOnly>());

			assert(m_pView);

			if( m_pView != NULL )
			{
				m_dwSeg = dwSeg;
				return m_pView + (SegmentSizeMask & static_cast<size_type>(fc));
			}
			else 
			{
				m_dwSeg = -1L;
				return NULL;
			}
		}

	private:
		char *_Access(DWORD dwSeg, Int2Type<true>)
		{
			return Base::ViewSegment(dwSeg << AlignBits_, AlignSize_);
		}
		char *_Access(DWORD dwSeg, Int2Type<false>)
		{
			return Base::AccessSegment(dwSeg << AlignBits_, AlignSize_);
		}
	};



	//-------------------------------------------------------------------------
	// class SegmentViewBuffer

	template<typename Owner, int SegBits = 16>
	class SegmentViewBuffer 
		: public SegmentAccessBuffer<Owner, SegBits, true>
	{
	private:
		typedef SegmentAccessBuffer<Owner, SegBits, true> Base;

	public:
		SegmentViewBuffer() 
		{}

		template<typename T1>
		SegmentViewBuffer(T1& arg1)
			: Base(arg1) 
		{}

		template<typename T1>
		SegmentViewBuffer(const T1& arg1)
			: Base(arg1) 
		{}
	};



	// -------------------------------------------------------------------------
	// class AccessBuffer

	template<typename Base, bool ReadOnly = false>
	class AccessBuffer 
		: public Base
	{
	public:
		typedef typename Base::size_type size_type;
		typedef typename Base::pos_type	pos_type;


	private:
		char* m_pView;
		DWORD m_dwBasePage;
		DWORD m_dwNextPage;

	public:
		enum { AllocationGranularity	 = Base::AllocationGranularity };
		enum { AllocationGranularityBits = Base::AllocationGranularityBits };
		enum { AllocationGranularityMask = Base::AllocationGranularityMask };

	public:
		AccessBuffer() 
			: m_pView(NULL)
			, m_dwBasePage(LONG_MAX)
			, m_dwNextPage(0) 
		{}
		template<typename T1>
		AccessBuffer(T1& arg1)
			: m_pView(NULL)
			, m_dwBasePage(LONG_MAX)
			, m_dwNextPage(0)
			, Base(arg1)
		{}
		template<typename T1>
		AccessBuffer(const T1& arg1)
			: m_pView(NULL)
			, m_dwBasePage(LONG_MAX)
			, m_dwNextPage(0)
			, Base(arg1) 
		{}
		~AccessBuffer()
		{
			if( m_pView != NULL )
			{
				Utils::Unmap(m_pView, (m_dwNextPage - m_dwBasePage) << AllocationGranularityBits);
			}
		}

	private:
		AccessBuffer(const AccessBuffer&);
		AccessBuffer &operator=(const AccessBuffer&);

		typedef typename Base::Utils Utils;

	public:
		void Close()
		{
			if( m_pView != NULL )
			{
				Utils::Unmap(m_pView, (m_dwNextPage - m_dwBasePage) << AllocationGranularityBits);

				m_pView		 = NULL;
				m_dwBasePage = LONG_MAX;
				m_dwNextPage = 0;

				Base::Close();
			}
		}

		void Flush()
		{
			if( m_pView != NULL )
			{
				Utils::Flush(m_pView, (m_dwNextPage - m_dwBasePage) << AllocationGranularityBits);
			}
		}

		char* View(pos_type offset, size_type count)
		{
			DWORD dwBasePage = static_cast<DWORD>(offset >> AllocationGranularityBits);
			DWORD dwNextPage = static_cast<DWORD>((offset + count + AllocationGranularityMask) >> AllocationGranularityBits);

			assert(count > 0);
			assert(dwBasePage < dwNextPage);

			if( m_dwNextPage < dwNextPage || m_dwBasePage > dwBasePage )
			{
				if( m_pView != NULL )
				{
					Utils::Unmap(m_pView, (m_dwNextPage - m_dwBasePage) << AllocationGranularityBits);
				}

				m_pView = _Access(dwBasePage, dwNextPage, Int2Type<ReadOnly>());

				assert(m_pView);

				if( m_pView == NULL )
					return NULL;

				m_dwBasePage = dwBasePage;
				m_dwNextPage = dwNextPage;
			}

			unsigned int offsetInPage = static_cast<unsigned int >(offset - (m_dwBasePage << AllocationGranularityBits));

			return m_pView + offsetInPage;
		}


	private:
		char *_Access(DWORD dwBasePage, DWORD dwNextPage, Int2Type<true>)
		{
			return Base::ViewSegment(dwBasePage, dwNextPage - dwBasePage);
		}
		char *_Access(DWORD dwBasePage, DWORD dwNextPage, Int2Type<false>)
		{
			return Base::AccessSegment(dwBasePage, dwNextPage - dwBasePage);
		}
	};





	//-------------------------------------------------------------------
	// class AccessBuffer

	template<typename Owner>
	class ViewBuffer 
		: public AccessBuffer<Owner, true>
	{
	private:
		typedef AccessBuffer<Owner, true> Base;

	public:
		ViewBuffer() 
		{}
		template<typename T1>
		ViewBuffer(T1& arg1)
			: Base(arg1) 
		{}
		template<typename T1>
		ViewBuffer(const T1& arg1)
			: Base(arg1) 
		{}
	};
}


#endif