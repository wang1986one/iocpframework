#ifndef __MEMORY_SCOPED_ALLOC_HPP
#define __MEMORY_SCOPED_ALLOC_HPP




namespace DataStructure
{



	// -------------------------------------------------------------------------
	// class ScopedAlloc �����ڴ���������ڴ�������

	// ͨ�����κ�����£�������ScopedAlloc�������ڴ������ȡ���õ����ܻر�

	typedef RegionAlloc<Config::Scoped> ScopedAlloc;

	// -------------------------------------------------------------------------

}




#endif