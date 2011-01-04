#ifndef __SERIALIZE_CONTAINER_HPP
#define __SERIALIZE_CONTAINER_HPP



#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>


#include <fstream>


namespace DataStructure
{
	namespace Serialize
	{


		// 不保证对数据里的空格、tab、换行等做处理

		// -----------------------------------------------------------------
		// class Writer

		class Writer
			: public std::ofstream
		{
		public:
			Writer(const std::string& filePath)
				: std::ofstream(filePath.c_str())
			{}
		};


		// -----------------------------------------------------------------
		// class Reader

		class Reader
			: public std::ifstream
		{
		public:
			Reader(const std::string& filePath)
				:std::ifstream(filePath.c_str())
			{}
		};


		template<typename WriterTraitT , typename ReaderTraitT>
		class SerializeTrait
		{
		public:
			typedef WriterTraitT WriterType;
			typedef ReaderTraitT ReaderType;
		};



		// 分隔符
		const char DELIM = '\n';

		// 内置类型序列化类

		// -----------------------------------------------------------------
		// class ValueTraits

		template<typename ValTraitsT, typename SerializeTraitsT = SerializeTrait<Writer, Reader> >
		class ValueTrait
		{
		public:
			typedef ValTraitsT								ValTraitsType;
			typedef typename SerializeTraitsT::WriterType	WriterType;
			typedef typename SerializeTraitsT::ReaderType	ReaderType;

		public:
			void Out(const ValTraitsType& val, WriterType &io)
			{
				io << val << DELIM;
			}

			void In(ValTraitsT &val, ReaderType &io)
			{
				io >> val;
			}
		};



		// 关联容器 -- map, multimap, set, multiset

		// -----------------------------------------------------------------
		// class AssociativeTypeTrait

		template<typename AssocTypeT, typename ValTraitT>
		class AssociativeTypeTrait
		{
		public:
			typedef typename ValTraitT::WriterType		WriterType;
			typedef typename ValTraitT::ReaderType		ReaderType;

			typedef typename AssocTypeT::size_type		size_type;
			typedef typename AssocTypeT::key_type		key_type;
			typedef typename AssocTypeT::mapped_type	data_type;

		public:
			void Out(AssocTypeT &val, WriterType &io)
			{
				ValueTrait<size_type, ValTraitT> sizeTrait;
				sizeTrait.Out(val.size(), io);

				for(AssocTypeT::iterator i = val.begin(); i != val.end(); ++i)
				{
					ValueTrait<key_type, ValTraitT>	keyTrait;
					ValueTrait<data_type, ValTraitT> dataTrait;

					keyTrait.Out(i->first, io);
					dataTrait.Out(i->second, io);
				}
			}

			void In(AssocTypeT& val, ReaderType &io )
			{
				ValueTrait<size_type, ValTraitT> sizeTrait;
				size_type valSize = 0;

				sizeTrait.In(valSize, io);

				for(; valSize > 0; valSize--)
				{
					key_type key;
					ValueTrait<key_type, ValTraitT> keyTrait;
					keyTrait.In(key, io);

					data_type data;
					ValueTrait<data_type, ValTraitT> dataTrait;
					dataTrait.In(data, io);

					val.insert(std::pair<key_type, data_type>(key, data));
				}
			}
		};


		// 线性容器--list, vector, stack, queue, deque, priority_queue

		// -----------------------------------------------------------------
		// class SequceTrait

		template<typename SequenceTypeT, typename ValTraitT>
		class SequenceTrait
		{
		public:
			typedef typename ValTraitT::WriterType		WriterType;
			typedef typename ValTraitT::ReaderType		ReaderType;

			typedef typename SequenceTypeT::size_type	size_type;	
			typedef typename SequenceTypeT::value_type	value_type;

		public:
			void Out(SequenceTypeT& val, WriterType &io )
			{
				ValueTrait<size_type, ValTraitT> sizeTrait;
				sizeTrait.Out(val.size(), io);

				for(SequenceTypeT::iterator i = val.begin(); i != val.end(); ++i)
				{
					ValueTrait<value_type, ValTraitT> valTrait;
					valTrait.Out(*i, io);
				}
			}

			void In(SequenceTypeT& val, ReaderType &io)
			{
				ValueTrait<size_type, ValTraitT> sizeTrait;
				size_type val_size = 0;
				sizeTrait.In(val_size, io);

				ValueTrait<value_type, ValTraitT> valTrait;
				for(; val_size > 0; --val_size)
				{
					value_type element;
					valTrait.In(element, io);

					val.push_back(element);
				}
			}
		};


		// STL map datatype serializer class.

		template<typename KeyTraitsT, typename DataTraitsT, typename ValTraitsT> 
		class ValueTrait<std::map<KeyTraitsT, DataTraitsT> , ValTraitsT> 
		{
		public:

			typedef typename ValTraitsT::WriterType		WriteType;
			typedef typename ValTraitsT::ReaderType		ReaderType;

			typedef std::map<KeyTraitsT, DataTraitsT>	MapTraitType;


		public:
			void Out(MapTraitType &val, WriteType &io)
			{
				AssociativeTypeTrait<MapTraitType, ValTraitsT> assocTrait;

				assocTrait.Out(val, io);
			}

			void In(MapTraitType& val, ReaderType &io)
			{
				AssociativeTypeTrait<MapTraitType, ValTraitsT> assocTrait;

				assocTrait.In(val, io);
			}
		};


		// STL multimap datatype serializer class.

		template<typename KeyTraitsT, typename DataTraitsT, typename ValTraitsT> 
		class ValueTrait<std::multimap<KeyTraitsT, DataTraitsT>, ValTraitsT> 
		{
		public:

			typedef typename ValTraitsT::WriterType WriteType;
			typedef typename ValTraitsT::ReaderType ReaderType;

			typedef std::multimap<KeyTraitsT, DataTraitsT> MultimapTraitType;


		public:
			void Out(MultimapTraitType& val, WriteType &io)
			{
				AssociativeTypeTrait<MultimapTraitType, ValTraitsT> assocTrait;

				assocTrait.Out(val, io);
			}

			void In(MultimapTraitType& val, ReaderType &io )
			{
				AssociativeTypeTrait<MultimapTraitType, ValTraitsT> assocTrait;

				assocTrait.In(val, io);
			}
		};



		// STL set datatype serializer class.

		template<typename KeyTraitsT, typename DataTraitsT, typename ValTraitsT> 
		class ValueTrait<std::set<KeyTraitsT, DataTraitsT> , ValTraitsT> 
		{
		public:
			typedef typename ValTraitsT::WriterType		WriteType;
			typedef typename ValTraitsT::ReaderType		ReaderType;

			typedef std::set<KeyTraitsT, DataTraitsT>	SetTratiType;


		public:
			void Out(SetTratiType& val, WriteType &io)
			{
				AssociativeTypeTrait<SetTratiType, ValTraitsT> assocTrait;

				assocTrait.Out(val, io);
			}

			void In(SetTratiType& val, ReaderType &io)
			{
				AssociativeTypeTrait<SetTratiType, ValTraitsT> assocTrait;

				assocTrait.In(val, io);
			}
		};

		// STL multiset datatype serializer class.

		template<typename KeyTraitsT, typename DataTraitsT, typename ValTraitsT> 
		class ValueTrait<std::multiset<KeyTraitsT, DataTraitsT>, ValTraitsT> 
		{
		public:
			typedef typename ValTraitsT::WriterType WriteType;
			typedef typename ValTraitsT::ReaderType ReaderType;

			typedef std::multiset<KeyTraitsT, DataTraitsT> MultisetTraitType;

		public:
			void Out(MultisetTraitType& val, WriteType &io)
			{

				AssociativeTypeTrait<MultisetTraitType, ValTraitsT> assocTrait;

				assocTrait.Out(val, io);
			}

			void In(MultisetTraitType& val, ReaderType &io)
			{
				AssociativeTypeTrait<MultisetTraitType, ValTraitsT> assocTrait;

				assocTrait.In(val, io);
			}
		};


		// STL list datatype serializer class.

		template<typename KeyTraitsT, typename DataTraitsT, typename ValTraitsT> 
		class ValueTrait<std::list<KeyTraitsT, DataTraitsT>, ValTraitsT>
		{
		public:
			typedef typename ValTraitsT::WriterType WriteType;
			typedef typename ValTraitsT::ReaderType ReaderType;

			typedef std::list<KeyTraitsT, DataTraitsT> ListTraitType;


		public:
			void Out(ListTraitType& val, WriteType &io)
			{
				SequenceTrait<ListTraitType, ValTraitsT> seqTrait;

				seqTrait.Out(val, io);
			}

			void In(ListTraitType& val, ReaderType &io)
			{
				SequenceTrait<ListTraitType, ValTraitsT> seqTrait;

				seqTrait.In(val, io);
			}
		};

		//STL vector datatype serializer class.

		template<typename KeyTraitsT, typename DataTraitsT, typename ValTraitsT> 
		class ValueTrait<std::vector<KeyTraitsT, DataTraitsT>, ValTraitsT>
		{
		public:
			typedef typename ValTraitsT::WriterType WriteType;
			typedef typename ValTraitsT::ReaderType ReaderType;

			typedef std::vector<KeyTraitsT, DataTraitsT> VectorTraitType;


		public:
			void Out(VectorTraitType& val, WriteType &io)
			{
				SequenceTrait<VectorTraitType, ValTraitsT> seqTrait;

				seqTrait.Out(val, io);
			}

			void In(VectorTraitType& val, ReaderType &io)
			{
				SequenceTrait<VectorTraitType, ValTraitsT> seqTrait;

				seqTrait.In(val, io);
			}
		};

		// STL stack datatype serializer class.

		template<typename KeyTraitsT, typename DataTraitsT, typename ValTraitsT>
		class ValueTrait<std::stack<KeyTraitsT, DataTraitsT>, ValTraitsT>
		{
		public:
			typedef typename ValTraitsT::WriterType WriteType;
			typedef typename ValTraitsT::ReaderType ReaderType;

			typedef std::stack<KeyTraitsT, DataTraitsT> StackTraitType;


		public:
			void Out(StackTraitType& val, WriteType &io)
			{
				SequenceTrait<StackTraitType, ValTraitsT> seqTrait;

				seqTrait.Out(val, io);
			}

			void In(StackTraitType& val, ReaderType &io)
			{
				SequenceTrait<StackTraitType, ValTraitsT> seqTrait;

				seqTrait.In(val, io);
			}
		};



		// STL queue datatype serializer class.

		template<typename KeyTraitsT, typename DataTraitsT, typename ValTraitsT>
		class ValueTrait<std::queue<KeyTraitsT, DataTraitsT>, ValTraitsT>
		{
		public:
			typedef typename ValTraitsT::WriterType WriteType;
			typedef typename ValTraitsT::ReaderType ReaderType;

			typedef std::queue<KeyTraitsT, DataTraitsT> QueueTraitType;


		public:
			void Out(QueueTraitType& val, WriteType &io)
			{
				SequenceTrait<QueueTraitType, ValTraitsT> seqTrait;

				seqTrait.Out(val, io);
			}

			void In(QueueTraitType& val, ReaderType &io)
			{

				SequenceTrait<QueueTraitType, ValTraitsT> seqTrait;

				seqTrait.In(val, io);
			}
		};



		// STL deque datatype serializer class.

		template<typename KeyTraitsT, typename DataTraitsT, typename ValTraitsT>
		class ValueTrait<std::deque<KeyTraitsT, DataTraitsT>, ValTraitsT>
		{
		public:

			typedef typename ValTraitsT::WriterType WriteType;
			typedef typename ValTraitsT::ReaderType ReaderType;

			typedef std::deque<KeyTraitsT, DataTraitsT> DequeTraitType;


		public:
			void Out(DequeTraitType& val, WriteType &io)
			{
				SequenceTrait<DequeTraitType, ValTraitsT> seqTrait;

				seqTrait.Out(val, io);
			}

			void In(DequeTraitType& val, ReaderType &io)
			{
				SequenceTrait<DequeTraitType, ValTraitsT> seqTrait;

				seqTrait.In(val, io);
			}
		};



		// STL priority_queue datatype serializer class.

		template<typename KeyTraitsT, typename DataTraitsT, typename ValTraitsT>
		class ValueTrait<std::priority_queue<KeyTraitsT, DataTraitsT>, ValTraitsT>
		{
		public:

			typedef typename ValTraitsT::WriterType WriteType;
			typedef typename ValTraitsT::ReaderType ReaderType;

			typedef std::priority_queue<KeyTraitsT, DataTraitsT> PriorityQueueTraitType;


		public:
			void Out(PriorityQueueTraitType& val, WriteType &io)
			{
				SequenceTrait<PriorityQueueTraitType, ValTraitsT> seqTrait;

				seqTrait.Out(val, io);
			}

			void In(PriorityQueueTraitType& val, ReaderType &io)
			{
				SequenceTrait<PriorityQueueTraitType, ValTraitsT> seqTrait;

				seqTrait.In(val, io);
			}
		};


	}
}






#endif