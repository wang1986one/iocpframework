#ifndef __FILESYSTEM_FILE_HPP
#define __FILESYSTEM_FILE_HPP

#include "../IOCP/Dispatcher.hpp"
#include "../IOCP/Buffer.hpp"



namespace async
{
	using namespace iocp;

	namespace filesystem
	{
		// forward declare

		class File;
		typedef std::tr1::shared_ptr<File> FilePtr;
		

		//--------------------------------------------------------------------------------
		// class File

		class File
		{
		public:
			typedef iocp::IODispatcher	AsyncIODispatcherType;

		private:
			// File Handle
			HANDLE file_;
			// IO����
			AsyncIODispatcherType &io_;

		public:
			explicit File(AsyncIODispatcherType &);
			File(AsyncIODispatcherType &, HANDLE);
			File(AsyncIODispatcherType &, LPCTSTR, DWORD, DWORD, DWORD, DWORD, LPSECURITY_ATTRIBUTES = NULL, HANDLE = NULL);
			~File();

			// non-copyable
		private:
			File(const File &);
			File &operator=(const File &);


		public:
			// explicitת��
			operator HANDLE()					{ return file_; }
			operator const HANDLE () const		{ return file_; }

			// ��ʾ��ȡ
			HANDLE GetHandle()					{ return file_; }
			const HANDLE GetHandle() const		{ return file_; }

		public:
			// ��Ŀ���ļ�
			void Open(LPCTSTR, DWORD, DWORD, DWORD, DWORD, LPSECURITY_ATTRIBUTES = NULL, HANDLE = NULL);
			// �ر�
			void Close();
			
			// �Ƿ��
			bool IsOpen() const
			{ return file_ != INVALID_HANDLE_VALUE; }

			// ˢ��
			bool Flush();
			
			//	ȡ��
			bool Cancel();

			// �����ļ���С
			void SetFileSize();

			// �������ûص��ӿ�,ͬ������
		public:
			size_t Read(const void *buf, size_t len, const LARGE_INTEGER &offset);
			size_t Write(const void *buf, size_t len, const LARGE_INTEGER &offset);

			// �첽���ýӿ�
		public:
			template<typename HandlerT>
			iocp::AsyncIOCallback<HandlerT> *AsyncRead(void *buf, size_t len, const u_int64 &offset, const HandlerT &handler);

			template<typename HandlerT>
			iocp::AsyncIOCallback<HandlerT> *AsyncWrite(const void *buf, size_t len, const u_int64 &offset, const HandlerT &handler);

		private:
			template<typename AsyncT>
			void _AsyncReadImpl(const AsyncT &result, void *buf, size_t len, const u_int64 &offset);

			template<typename AsyncT>
			void _AsyncWriteImpl(const AsyncT &result, const void *buf, size_t len, const u_int64 &offset);
		};
	}



	namespace iocp
	{
		typedef async::filesystem::File File;

		// Socket ����
		template<>
		struct ObjectFactory< File >
		{
			typedef async::memory::FixedMemoryPool<true, sizeof(File)>		PoolType;
			typedef ObjectPool< PoolType >									ObjectPoolType;
		};
	}


	namespace filesystem
	{
		inline FilePtr MakeFile(File::AsyncIODispatcherType &io)
		{
			return FilePtr(ObjectAlloc<File>(io), &ObjectDeallocate<File>);
		}

		inline FilePtr MakeFile(File::AsyncIODispatcherType &io, HANDLE file)
		{
			return FilePtr(ObjectAlloc<File>(io, file), &ObjectDeallocate<File>);
		}

		inline FilePtr MakeFile(File::AsyncIODispatcherType &io, LPCTSTR path, DWORD access, DWORD shared, DWORD create, DWORD flag, LPSECURITY_ATTRIBUTES attribute = NULL, HANDLE templateMode = NULL)
		{
			return FilePtr(ObjectAlloc<File>(io, path, access, shared, create, flag, attribute, templateMode), &ObjectDeallocate<File>);
		}


		template<typename HandlerT>
		iocp::AsyncIOCallback<HandlerT> *File::AsyncRead(void *buf, size_t len, const u_int64 &offset, const HandlerT &handler)
		{
			if( !IsOpen() )
				throw std::logic_error("File not open");

			iocp::AsyncIOCallback<HandlerT> *asynResult(MakeAsyncIOCallback(handler));
			asynResult->AddRef();

			_AsyncReadImpl(asynResult, buf, len, offset);
			return asynResult;
		}

		template<typename HandlerT>
		iocp::AsyncIOCallback<HandlerT> *File::AsyncWrite(const void *buf, size_t len, const u_int64 &offset, const HandlerT &handler)
		{
			if( !IsOpen() )
				throw std::logic_error("File not open");

			iocp::AsyncIOCallback<HandlerT> *asynResult(MakeAsyncIOCallback(handler));
			asynResult->AddRef();

			_AsyncWriteImpl(asynResult, buf, len, offset);
			return asynResult;
		}


		namespace detail
		{
			inline DWORD HIDWORD(const u_int64 &src)
			{
				return (src >> 32) & 0xffffffff;
			}

			inline DWORD LODWORD(const u_int64 &src)
			{
				return src & 0xffffffff;
			}
		}

		// --------------------------------
		template<typename AsyncT>
		void File::_AsyncReadImpl(const AsyncT &result, void *buf, size_t len, const u_int64 &offset)
		{
			result->Offset		= detail::LODWORD(offset);
			result->OffsetHigh	= detail::HIDWORD(offset);

			DWORD bytesRead = 0;
			BOOL bSuc = ::ReadFile(file_, buf, len, &bytesRead, result);
			DWORD dwError = ::WSAGetLastError();

			if( !bSuc && dwError != ERROR_IO_PENDING )
			{
				result->Release();
				throw iocp::Win32Exception("ReadFile");
			}
		}

		template<typename AsyncT>
		void File::_AsyncWriteImpl(const AsyncT &result, const void *buf, size_t len, const u_int64 &offset)
		{
			result->Offset		= detail::LODWORD(offset);
			result->OffsetHigh	= detail::HIDWORD(offset);

			DWORD bytesRead = 0;
			BOOL bSuc = ::WriteFile(file_, buf, len, &bytesRead, result);
			DWORD dwError = ::WSAGetLastError();

			if( !bSuc && dwError != ERROR_IO_PENDING )
			{
				result->Release();
				throw iocp::Win32Exception("WriteFile");
			}
		}
	}

	
}




#endif