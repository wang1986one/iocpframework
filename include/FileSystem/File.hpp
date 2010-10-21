#ifndef __FILESYSTEM_FILE_HPP
#define __FILESYSTEM_FILE_HPP

#include "../IOCP/Dispatcher.hpp"
#include "../IOCP/Buffer.hpp"







namespace async
{
	
	namespace filesystem
	{

		using namespace async::iocp;

		// forward declare
		class File;
		typedef pointer<File> FilePtr;


		// class FileBuffer
		typedef BufferT<char, async::memory::MemAllocator<char, MemoryMgr::FileMemoryPool> > FileBuffer;
		typedef pointer<FileBuffer> FileBufferPtr;



		//--------------------------------------------------------------------------------
		// class File

		class File
			: public Object
		{
		private:
			HANDLE file_;
			OverlappedDispatcher &io_;

		public:
			File(OverlappedDispatcher &, LPCTSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE hTemplate = NULL);
			~File();

			// non-copyable
		private:
			File(const File &);
			File &operator=(const File &);


		public:
			operator HANDLE()
			{
				return file_;
			}
			operator const HANDLE () const
			{
				return file_;
			}

			HANDLE GetHandle()
			{
				return file_;
			}
			const HANDLE GetHandle() const
			{
				return file_;
			}

		public:
			void Close();

		public:
			// Read
			AsyncResultPtr BeginRead(const FileBufferPtr &buf, size_t nOffset, size_t nBufSize, const LARGE_INTEGER *fileOffset
				, const AsyncCallbackFunc &callback);
			size_t EndRead(const AsyncResultPtr &asynResult);

			// Write
			AsyncResultPtr BeginWrite(const FileBufferPtr &buf, size_t nOffset, size_t nBufSize, const LARGE_INTEGER *fileOffset
				, const AsyncCallbackFunc &callback );
			size_t EndWrite(const AsyncResultPtr &asynResult);


			
		};
	}





}




#endif