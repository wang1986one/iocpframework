#ifndef __FILE_BUFFER_HPP
#define __FILE_BUFFER_HPP


#include "../Memory.hpp"
#include "AutoBuffer.hpp"

#include <SmartPointer/SmartHandle.h>


namespace DataStructure
{
	
	template<typename Alloc>
	class FileBufferT
		: public AutoBufferT<Alloc>
	{
	private:
		typedef AutoBufferT<Alloc> Base;

	public:
		FileBufferT()
		{}

		template<typename InputFileT>
		explicit FileBufferT(const InputFileT &file)
		{
			Read(file);
		}

	public:
		template<typename InputFileT>
		HRESULT Read(InputFileT filePath)
		{
			CAutoHandle hFile = ::CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ,
				NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
			if( !hFile.IsValid() )
				return HRESULT_FROM_WIN32(::GetLastError());

			HRESULT hRes = _Read(hFile);

			return hRes;
		}


	private:
		HRESULT _Read(CAutoHandle &hFile)
		{
			assert( !Base::Good() );

			DWORD dwFileSizeHigh = 0;
			DWORD dwFileSize = ::GetFileSize(hFile, &dwFileSizeHigh);
			if( dwFileSizeHigh > 0 )
				return E_OUTOFMEMORY;

			::ReadFile(hFile, Base::Allocate(dwFileSize), dwFileSize, &dwFileSizeHigh, NULL);
		
			return (dwFileSizeHigh == dwFileSize ? S_OK : STG_E_READFAULT);
		}
	};


	typedef FileBufferT<StdlibAlloc>			StdFileBuffer;
	typedef FileBufferT<Allocator<char> >		FileBufferA;
	typedef FileBufferT<Allocator<wchar_t> >	FileBufferW;
}







#endif