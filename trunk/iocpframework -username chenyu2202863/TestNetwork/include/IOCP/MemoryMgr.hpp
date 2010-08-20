#ifndef __IOCP_MEMORY_MANAGER_HPP
#define __IOCP_MEMORY_MANAGER_HPP

//#include <Comm/Memory/SGIMemoryPool.hpp>
//#include <Comm/Memory/FixedMemoryPool.hpp>
//#include <Comm/Allocator/MemAllocator.hpp>



namespace async
{


	namespace IOCP
	{


		class MemoryMgr
		{
		public:
			// 小对象使用--Socket,File, Object...
			//typedef async::SGIMTMemoryPool SmallObjectMemoryPool;
			//typedef async::MemAllocator<char, SmallObjectMemoryPool>	AllocType;

			typedef std::allocator<char> AllocType;


			//  Socket和File内存池
			static const size_t DEFAULT_SOCKET_SIZE = 4 * 1024;		
			static const size_t DEFAULT_FILE_SIZE	= 4 * 1024;

			//typedef async::SGIMemoryPool<true, DEFAULT_SOCKET_SIZE>		SocketMemoryPool;
			//typedef async::SGIMemoryPool<true, DEFAULT_FILE_SIZE>		FileMemoryPool;

			typedef std::allocator<char> SocketMemoryPool;
			typedef std::allocator<char> FileMemoryPool;

		public:
			// 小对象使用
			static AllocType &GetMemory()
			{
				//static SmallObjectMemoryPool memoryPool;
				//static AllocType alloc(memoryPool);

				static AllocType alloc;


				return alloc;
			}
		};

	} // end of IOCP

} // end of DataStructrue












#endif