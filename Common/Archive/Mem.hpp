#ifndef __ARCHIVE_MEM_HPP
#define __ARCHIVE_MEM_HPP


#include "MemoryArchive.hpp"
#include <deque>

namespace DataStructure
{

	// -------------------------------------------------------------------------
	// class MemReader

	typedef MemReadArchive<const char*>						MemReadeArchive;


	// -------------------------------------------------------------------------
	// class VectorReader/VectorWriter

	typedef std::vector<char>								CharVector;
	typedef MemWriteArchive<CharVector>						VectorWriteArchive;
	typedef MemReadArchive<CharVector::const_iterator>		VectorReadeArchive;


	// -------------------------------------------------------------------------
	// class DequeReader/DequeWriter, TextPoolReader/TextPoolWriter

	typedef std::deque<char>								CharDeque;
	typedef MemWriteArchive<CharDeque>						DequeWriteArchive;
	typedef MemReadArchive<CharDeque::const_iterator>		DequeReadeArchive;


	// -------------------------------------------------------------------------
	// class StlStringReader/StlStringWriter

	typedef MemWriteArchive<std::string>					StringWriteArchive;
	typedef MemReadArchive<std::string::const_iterator>		StringReadeArchive;

}



#endif