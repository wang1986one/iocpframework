#ifndef __BUFFER_STACK_BUFFER_HPP
#define __BUFFER_STACK_BUFFER_HPP

#include <type_traits>
#include <cassert>
#include <new>



namespace DataStructure
{
	
	//--------------------------------------------------------------------------
	// class StackBuffer
	
	// ����ջ�ڴ棨�Ѷ��룩
	template<typename T> 
	class StackBuffer 
	{ 
	public: 
		typedef T						value_type; 
		typedef const T					const_value_type; 
		typedef value_type&				reference; 
		typedef const_value_type&		const_reference; 
		typedef value_type*				pointer; 
		typedef const_value_type*		const_pointer;

	private:
		// aligned_storage<_Size, _Align> ��СΪ_Size,���뵽_Align����
		typedef std::tr1::aligned_storage<sizeof(T), std::tr1::alignment_of<T>::value> storage_type; 

	private:
		// members 
		pointer object_; 
		storage_type buffer_; 


	public:
		StackBuffer() 
			: object_(NULL)
		{} 

		~StackBuffer() 
		{ 
			Destroy(); 
		}

	private:
		StackBuffer(const StackBuffer &rhs); 
		StackBuffer& operator=(const StackBuffer &rhs); 

	public:
		// need to make multiple versions of this. 
		void Create() 
		{ 
			Destroy(); 
			object_ = new(buffer()) T; 
		} 

		template<typename T> 
		void Create(const T &val) 
		{ 
			Destroy(); 

			object_ = new(buffer()) T(val); 
		} 

		void Destroy() 
		{ 
			if( _Exists() ) 
			{ 
				object_->~T(); 
				object_ = 0; 
			} 
		} 

		// access 
		reference get() 
		{ 
			assert(_Exists()); 
			return *object_; 
		} 

		const_reference get() const 
		{ 
			assert(_Exists()); 
			return *object_; 
		} 

		pointer operator*() 
		{ 
			assert(_Exists()); 
			return object_; 
		} 

		const_pointer operator*() const 
		{ 
			assert(_Exists()); 
			return object_; 
		} 

		void* buffer() 
		{ 
			return &buffer_; 
		} 

	private:
		// query 
		const bool _Exists() const 
		{ 
			return object_ != 0; 
		}
	}; 


}





#endif