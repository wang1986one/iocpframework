#ifndef __MAP_FILE_BUFFER_HPP
#define __MAP_FILE_BUFFER_HPP



#include "../MMap/MapFile.hpp"


namespace DataStructure
{
	template<typename CharT>
	class MapFileBufT
	{
	public:
		typedef size_t					size_type;
		typedef CharT					char_type;
		typedef CharT*					iterator;
		typedef const CharT*			const_iterator;

	private:
		const_iterator m_data;
		size_type	m_fileSize;
		MapFileReadOnly m_mapFile;


	public:
		MapFileBufT()
			: m_data(NULL)
		{}
		template<typename T>
		MapFileBufT(T file)
			: m_data(NULL)
		{
			Read(file);
		}
		~MapFileBufT()
		{
			if( m_data != NULL )
			{
				MapFileReadOnly::Unmap((void *)m_data, m_fileSize);
				m_mapFile.Close();
			}
		}

	public:
		template<typename T>
		bool Read(T file)
		{
			if( Good() )
				return false;

			MapFileReadOnly::pos_type fileSize = 0;
			bool bSuc = m_mapFile.Open(file, &fileSize);
			if( !bSuc )
				return bSuc;

			if( fileSize > 0xFFFFFFFF )
				return false;

			m_fileSize  = static_cast<DWORD>(fileSize);
			m_data		= (const_iterator)m_mapFile.Map(0, m_fileSize);
			if( m_data != NULL )
				return true;

			m_mapFile.Close();
			return false;
		}

		void clear()
		{
			if( m_data != NULL )
			{
				MapFileReadOnly::Unmap(reinterpret_cast<void *>(m_data), m_fileSize);
				m_data = NULL;
				m_mapFile.Close();
			}
		}

		const_iterator data() const
		{
			return m_data;
		}

		const_iterator begin() const
		{
			return m_data;
		}

		const_iterator end() const
		{
			return m_data + m_fileSize;
		}

		bool Good() const
		{
			return m_data != NULL;
		}

		const size_type size() const
		{
			return m_fileSize;
		}
	};


	typedef MapFileBufT<char> MapFileBuf;

}







#endif