#ifndef __SMART_POINTER_SMART_HANDLE_HPP
#define __SMART_POINTER_SMART_HANDLE_HPP


#include <Windows.h>



namespace DataStructure
{


	// Release algorithms (release policies)

	template<typename T, typename PointedBy>
	struct CCloseHandle
	{
		void operator()(T handle)
		{
			::CloseHandle(handle);
		}

		~CCloseHandle()
		{
		}
	};



	template<typename T, typename PointedBy>
	struct CCloseRegKey
	{
		void operator()(T handle)
		{
			::RegCloseKey(handle);
		}

		~CCloseRegKey()
		{
		}
	};


	template<typename T, typename PointedBy>
	struct CCloseLibrary
	{
		void operator()(T handle)
		{
			::FreeLibrary(handle);
		}

		~CCloseLibrary()
		{
		}
	};


	template<typename T, typename PointedBy>
	struct CCloseViewOfFile
	{
		void operator()(T handle)
		{
			::UnmapViewOfFile(handle);
		}

		~CCloseViewOfFile()
		{
		}
	};


	// empty class used as default CAutoHandle template parameter.
	class CEmptyClass
	{
	};


	// Class CSmartHandle which implements release policy.
	// Second template parameter is ReleaseAlgorithm which is template itself.

	template<
		typename HandleT, 
		template<typename, typename> class ReleaseAlgorithm, 
		typename PointedBy = CEmptyClass,          // used for smart pointers
		HandleT NULL_VALUE = NULL
	>
	class CSmartHandle 
		: private ReleaseAlgorithm<HandleT, PointedBy>
	{
	private:
		//ReleaseAlgorithm<HandleType, PointedBy> m_funcRelease;
		HandleT m_Handle;

	public:
		CSmartHandle()
			: m_Handle(NULL_VALUE)
		{
		}

		CSmartHandle(const HandleT &h)
			: m_Handle(h)
		{
		}

		CSmartHandle(const CSmartHandle &h)
		{
			CleanUp();
			m_Handle = h.m_Handle;
		}

		~CSmartHandle()
		{
			CleanUp();
		}

		CSmartHandle &operator=(const CSmartHandle &rhs) 
		{ 
			if( &rhs != this )
			{
				m_Handle = rhs.m_Handle;
			}

			return(*this);  
		}

		HandleT &operator=(const HandleT &hande) 
		{ 
			if( hande != m_Handle )
			{
				m_Handle = hande;
			}

			return m_Handle;  
		}

		operator HandleT()
		{
			return m_Handle;
		}

		operator HandleT() const
		{
			return m_Handle;
		}

		PointedBy* operator->()                 // for using as smart pointer
		{
			// NOTE: adding this operator allows to use CAutoHandle object as pointer.
			// However, if PointedBy is CHandlePlaceHolder (used for handles),
			// this is not compiled because CHandlePlaceHolder has no functions.
			// This is exactly what I need.
			return m_Handle;
		}

		bool IsValid()
		{
			return m_Handle != NULL_VALUE;
		}

		HandleT Detach()
		{
			HandleT hHandle = m_Handle;
			m_Handle = NULL_VALUE;

			return hHandle;
		}

	protected:
		void CleanUp()
		{
			if ( m_Handle != NULL_VALUE )
			{
				operator()(m_Handle);
				m_Handle = NULL_VALUE;
			}
		}

	};


	// Client code (definitions of standard Windows handles).
	typedef CSmartHandle<HANDLE,  CCloseHandle>		                    CAutoHandle;
	typedef CSmartHandle<HKEY,    CCloseRegKey>		                    CAutoRegKey;
	typedef CSmartHandle<PVOID,   CCloseViewOfFile>	                    CAutoViewOfFile;
	typedef CSmartHandle<HMODULE, CCloseLibrary>		                CAutoLibrary;
	typedef CSmartHandle<HANDLE,  CCloseHandle, CEmptyClass, 
		INVALID_HANDLE_VALUE>											CAutoFile;


}



#endif