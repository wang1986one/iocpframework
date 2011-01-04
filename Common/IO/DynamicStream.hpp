#ifndef __IO_DYNAMIC_STREAM_HPP
#define __IO_DYNAMIC_STREAM_HPP




namespace DataStructure
{

	//--------------------------------------------------------------------------
	// class WriterBase

	// Writer»ùÀà
	template<typename CharT>
	class WriterBase
	{
	public:
		typedef CharT		char_type;

	public:
		virtual ~WriterBase() 
		{}

		virtual void Write(char_type c) = 0;

		template<typename IterT>
		void Write(IterT b, IterT e)
		{
			while(b != e)
			{
				Write(*b++);
			}
		}

		virtual void Write(const char_type *b, const char_type *e) = 0;

	public:
		virtual bool EndOfStream()	const = 0;
		virtual bool DeviceError()	const = 0;
		virtual bool Good()			const = 0;
	};

	//--------------------------------------------------------------------------
	// class DynamicWriter

	// used for dispatching via WriterBase
	template<typename CharT>
	class DynamicWriter
	{
	public:
		typedef CharT					char_type;
		typedef WriterBase<char_type>	WriterBaseType;

	public:
		explicit DynamicWriter(WriterBaseType *w) 
			: writer_(w) 
		{}

		void Write(char_type c) 
		{ writer_->Write(c); }

		void Write(const char_type *b, const char_type *e) 
		{ writer_->Write(b, e); }

	public:
		bool EndOfStream() 
		{ return writer_->EndOfStream(); }

		bool DeviceError() 
		{ return writer_->DeviceError(); }

		bool Good() const 
		{ return writer_->Good(); }

	private:
		WriterBaseType *writer_;
	};


	//---------------------------------------------------------------------------------
	// class DynamicWriterAdapter

	// used to provide the writer_base interface
	// to arbitrary (in particular, non-polymorphic) writer
	template<typename WriterT>
	class DynamicWriterAdapter 
		: public WriterBase<typename WriterT::char_type>
	{
	public:
		typedef WriterT								WriterType;
		typedef typename WriterType::char_type		char_type;

	public:
		explicit DynamicWriterAdapter(WriterType &w) 
			: writer_(w) 
		{}

	private:
		DynamicWriterAdapter(const DynamicWriterAdapter &);
		DynamicWriterAdapter &operator=(const DynamicWriterAdapter &);

	public:
		virtual void Write(char_type c) 
		{ writer_.Write(c); }
		virtual void Write(const char_type *b, const char_type *e) 
		{ writer_.Write(b, e); }

	public:
		virtual bool EndOfStream() const 
		{ return writer_.EndOfStream(); }

		virtual bool DeviceError() const 
		{ return writer_.DeviceError(); }

		virtual bool Good() const
		{ return writer_.Good(); }

	private:
		WriterType &writer_;
	};



	//--------------------------------------------------------------
	// class ReaderBase
	template<typename CharT>
	class ReaderBase
	{
	public:
		typedef CharT		char_type;

	public:
		virtual ~ReaderBase() 
		{}

		virtual bool Read(char_type &c) = 0;

		template<typename IterT>
		size_t Read(IterT b, size_t elems)
		{
			size_t to_read = elems;
			for (; elems > 0; --elems)
			{
				char_type c;
				if (Read(c))
				{
					*b++ = c;
				}
				else
				{
					break;
				}
			}

			return to_read - elems;
		}

		// additional functionality
		// (this means that only rewindable readers
		// can be adapted by this adapter)
		virtual void GoBack(int s) = 0;

		// stream state and error detection
		virtual bool EndOfStream() const = 0;
		virtual bool DeviceError() const = 0;
		virtual void SetBadFormat(bool b) = 0;
		virtual bool BadFormat() const = 0;
		virtual bool Good() const = 0;
	};

	//---------------------------------------------------------------------------
	// class DynamicReader
	// 
	// used for dispatching via reader_base

	template<typename CharT>
	class DynamicReader
	{
	public:
		typedef CharT						char_type;
		typedef ReaderBase<char_type>		ReaderBaseType;

	public:
		explicit DynamicReader(ReaderBaseType *r) 
			: reader_(r) 
		{}

	public:
		bool Read(char_type &c) 
		{ return reader_->Read(c); }

	public:
		void GoBack(int s) 
		{ reader_->GoBack(s); }

		bool EndOfStream() const 
		{ return reader_->EndOfStream(); }

		bool DeviceError() const 
		{ return reader_->DeviceError(); }

		void SetBadFormat(bool b) 
		{ reader_->SetBadFormat(b); }

		bool BadFormat() const 
		{ return reader_->BadFormat(); }

		bool Good() const 
		{ return reader_->Good(); }

	private:
		ReaderBaseType *reader_;
	};


	//-----------------------------------------------------------------------
	// class DynamicReaderAdapter

	template<typename ReaderT>
	class DynamicReaderAdapter 
		: public ReaderBase<typename ReaderT::char_type>
	{
	public:
		typedef typename ReaderT::char_type		char_type;
		typedef ReaderT							ReaderType;


	public:
		explicit DynamicReaderAdapter(ReaderType &r) 
			: reader_(r) 
		{}

	private:
		DynamicReaderAdapter(const DynamicReaderAdapter &);
		DynamicReaderAdapter &operator=(const DynamicReaderAdapter &);

	public:
		virtual bool Read(char_type &c) 
		{ return reader_.Read(c); }

	public:
		virtual void GoBack(int s) 
		{ reader_.GoBack(s); }

		virtual bool EndOfStream() const 
		{ return reader_.EndOfStream(); }

		virtual bool DeviceError() const 
		{ return reader_.DeviceError(); }

		virtual void SetBadFormat(bool b) 
		{ reader_.SetBadFormat(b); }

		virtual bool BadFormat() const 
		{ return reader_.BadFormat(); }

		virtual bool Good() const 
		{ return reader_.Good(); }

	private:
		ReaderType &reader_;
	};
}








#endif