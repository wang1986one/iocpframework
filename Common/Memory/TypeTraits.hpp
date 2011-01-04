#ifndef __MEMORY_TYPE_TRAITS_HPP
#define __MEMORY_TYPE_TRAITS_HPP







namespace DataStructure
{


	// =========================================================================
	// class ConstructorTraits

	template<typename T>
	struct ConstructorTraits
	{
		static void Construct(void* data)
		{
			new(data) T;
		}

		static void ConstructArray(T* pArray, size_t count)
		{
			for (size_t i = 0; i < count; ++i)
				new(pArray + i) T;
		}
	};




	// =========================================================================
	// class DestructorTraits

	typedef void FnDestructor(void* data);
	typedef FnDestructor* DestructorT;


	template<typename T>
	struct DestructorTraits
	{
		enum { hasDestructor = 1 };

		typedef DestructorT DestructorType;

		static void Destruct(void* data)
		{
			(reinterpret_cast<T *>(data))->~T();
		}

		static void DestructArray(T* pArray, size_t count)
		{
			for(size_t i = 0; i < count; ++i)
				pArray[i].~T();
		}
	};

	template<typename T>
	inline void DestroyArray(T *pArray, size_t count)
	{
		DestructorTraits<T>::DestructArray(pArray, count);
	}


	// =========================================================================
	// class ArrayFactory
	template<typename T>
	struct ArrayFactory
	{
		struct DestructorHeader
		{
			size_t count;
		};

		static size_t AllocSize(size_t count)
		{
			return sizeof(DestructorHeader) + sizeof(T) * count;
		}

		static void Destruct(void *pData)
		{
			DestructorHeader *pHdr = reinterpret_cast<DestructorHeader *>(pData);
			DestructorTraits<T>::DestructArray(reinterpret_cast<T *>(pHdr + 1), pHdr->count);
		}

		template<typename U>
		static T *Create(U &alloc, size_t count)
		{
			DestructorHeader *pHdr = reinterpret_cast<DestructorHeader *>(alloc.Allocate(sizeof(DestructorHeader) + 
				sizeof(T) * count, Destruct));

			T *pArray = reinterpret_cast<T *>(pHdr + 1);
			pHdr->count = count;

			ConstructorTraits<T>::ConstructArray(pArray, count);

			return pArray;
		}

		static char *Buffer(T *pArray)
		{
			return static_cast<char *>(pArray) - sizeof(DestructorHeader);
		}

		static size_t size(T *pArray)
		{
			return (reinterpret_cast<DestructorHeader *>(pArray) - 1)->count;
		}
	};



}





#endif