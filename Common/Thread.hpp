#ifndef __THREAD_BASIC_HPP
#define __THREAD_BASIC_HPP



#include <Windows.h>
#include <memory>


#if defined(_MT) || defined(_DLL)
#include <process.h>
#endif // _MT

#include <cassert>		// For assert

#include <functional>	// For ThreadImplEx

#include <type_traits>


namespace DataStructure
{





	//-------------------------------------------------------------------
	// Thread creation helpers

#if defined(_MT)
	// CRTThreadTraits
	// This class is for use with OverlappedThreadPool or CWorkerThread
	// It should be used if the worker class will use CRT
	// functions.
	class CRTThreadTraits
	{
	public:
		static HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize, LPTHREAD_START_ROUTINE pfnThreadProc, void *pvParam, DWORD dwCreationFlags, DWORD *pdwThreadId)
		{
			assert(sizeof(DWORD) == sizeof(unsigned int)); // sanity check for pdwThreadId

			// _beginthreadex calls CreateThread which will set the last error value before it returns.
			return (HANDLE)_beginthreadex(lpsa, dwStackSize, (unsigned int (__stdcall *)(void *)) pfnThreadProc, pvParam, dwCreationFlags, (unsigned int *) pdwThreadId);
		}
	};
#endif

	// Win32ThreadTraits
	// This class is for use with OverlappedThreadPool or CWorkerThread
	// It should be used if the worker class will not use CRT
	// functions.
	class Win32ThreadTraits
	{
	public:
		static HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize, LPTHREAD_START_ROUTINE pfnThreadProc, void *pvParam, DWORD dwCreationFlags, DWORD *pdwThreadId)
		{
			return ::CreateThread(lpsa, dwStackSize, pfnThreadProc, pvParam, dwCreationFlags, pdwThreadId);
		}
	};

#if defined(_MT)
	typedef CRTThreadTraits DefaultThreadTraits;
#else
	typedef Win32ThreadTraits DefaultThreadTraits;
#endif


	//-------------------------------------------------------------------
	// Thread Wait helpers
	class Win32WaitTraits
	{
	public:
		static DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwTimeout)
		{
			return ::WaitForSingleObject(hHandle, dwTimeout);
		}
	};

	typedef Win32WaitTraits DefaultWaitTraits;




}


#endif