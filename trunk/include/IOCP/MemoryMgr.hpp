#ifndef __IOCP_MEMORY_MANAGER_HPP
#define __IOCP_MEMORY_MANAGER_HPP

#include "../Memory/SGIMemoryPool.hpp"
#include "../Memory/FixedMemoryPool.hpp"
#include "../Memory/MemAllocator.hpp"

//#include "../Memory/MemoryLeak.h"

namespace async
{


	namespace iocp
	{

		// -------------------------------------------------------
		// class MemoryMgr 
		// �ڴ����

		class MemoryMgr
		{
		public:
			// С����ʹ��--Socket,File, Object...
			typedef async::memory::SGIMTMemoryPool SmallObjectMemoryPool;
			typedef async::memory::MemAllocator<char, SmallObjectMemoryPool>	AllocType;

			//typedef std::allocator<char> AllocType;


			//  Socket��File�ڴ��
			static const size_t DEFAULT_SOCKET_SIZE = 4 * 1024;		
			static const size_t DEFAULT_FILE_SIZE	= 4 * 1024;

			typedef async::memory::SGIMemoryPool<true, DEFAULT_SOCKET_SIZE>		SocketMemoryPool;
			typedef async::memory::SGIMemoryPool<true, DEFAULT_FILE_SIZE>		FileMemoryPool;

			//typedef std::allocator<char> SocketMemoryPool;
			//typedef std::allocator<char> FileMemoryPool;

		public:
			// С����ʹ��
			static AllocType &GetMemory()
			{
				static SmallObjectMemoryPool memoryPool;
				static AllocType alloc(memoryPool);

				//static AllocType alloc;


				return alloc;
			}
		};

	} // end of iocp

} // end of DataStructrue












#endif