#ifndef TUT_H_GUARD
#define TUT_H_GUARD


#include "tut_assert.hpp"
#include "tut_runner.hpp"




namespace tut
{

	template<typename, int>
	class TestGroup;

	/**
	* Test ObjectType. Contains data Test run upon and default Test method
	* implementation. Inherited from Data to allow TestsType to
	* access Test data as members.
	*/
	template< typename Data >
	class TestObject 
		: public Data
	{

		template<typename D, int M>
		friend class TestGroup;


	private:
		/**
		* The flag is set to true by default (dummy) testNum_.
		* Used to detect usused testNum_ numbers and avoid unnecessary
		* testNum_ ObjectType creation which may be time-consuming depending
		* on operations described in Data::Data() and Data::~Data().
		*/
		bool called_method_was_a_dummy_test_;

		int             current_test_id_;
		std::string     current_test_name_;
		std::string     current_test_group_;

	public:
		TestObject()
			: called_method_was_a_dummy_test_(false)
			, current_test_id_(0)
			, current_test_name_()
			, current_test_group_()
		{}
		virtual ~TestObject()
		{}

	public:
		void SetTestGroup(const char *group)
		{
			current_test_group_ = group;
		}

		void SetTestID(int current_test_id)
		{
			current_test_id_ = current_test_id;
		}

		void SetTestName(const std::string& current_test_name)
		{
			current_test_name_ = current_test_name;
		}

		const std::string& GetTestName() const
		{
			return current_test_name_;
		}

		const std::string& GetTestGroup() const
		{
			return current_test_group_;
		}

		int GetTestID() const
		{
			return current_test_id_;
		}

		/**
		* Default do-nothing Test.
		*/
		template< int n >
		void Test()
		{
			called_method_was_a_dummy_test_ = true;
		}
	};



	/**
	* Walks through testNum_ tree and stores address of each
	* testNum_ method in group. Instantiation stops at 0.
	*/
	template< typename Test, typename Group, int n >
	struct TestsRegister
	{
		static void Register(Group& group)
		{
			group.Register(n, &Test::Test<n>);
			TestsRegister<Test, Group, n - 1>::Register(group);
		}
	};

	template< typename Test, typename Group >
	struct TestsRegister<Test, Group, 0>
	{
		static void Register(Group&)
		{
		}
	};


	/**
	* Test group; used to recreate testNum ObjectType instance for
	* each new testNum since we have to have reinitialized
	* Data base class.
	*/
	template< typename Data, int MaxTestsInGroup = 50 >
	class TestGroup 
		: public GroupBase
	{
		typedef void (TestObject<Data>::*TestMethodFuncPtr)();
		typedef std::map<int, TestMethodFuncPtr>			TestsType;
		typedef typename TestsType::iterator				tests_iterator;
		typedef typename TestsType::const_iterator			tests_const_iterator;
		typedef typename TestsType::const_reverse_iterator	tests_const_reverse_iterator;
		typedef typename TestsType::size_type				size_type;


	private:
		const char* name_;

		TestsType tests_;
		tests_iterator current_test_;

		enum SEHResult
		{
			SEH_OK,
			SEH_DUMMY
		};

		/**
		* Exception-in-destructor-safe smart-pointer class.
		*/
		template< typename T>
		class safe_holder
		{
		private:
			T* p_;
			bool permit_throw_in_dtor;


		public:
			safe_holder()
				: p_(0)
				, permit_throw_in_dtor(false)
			{}

			~safe_holder()
			{
				release();
			}

		private:
			safe_holder(const safe_holder&);
			safe_holder& operator=(const safe_holder&);


		public:
			T* operator->() const
			{
				return p_;
			}

			T* get() const
			{
				return p_;
			}

			/**
			* Tell ptr it can throw from destructor. Right way is to
			* use std::uncaught_exception(), but some compilers lack
			* correct implementation of the function.
			*/
			void permit_throw()
			{
				permit_throw_in_dtor = true;
			}

			/**
			* Specially treats exceptions in testNum_ ObjectType destructor;
			* if testNum_ itself failed, exceptions in destructor
			* are ignored; if testNum_ was successful and destructor failed,
			* Warning exception throwed.
			*/
			void release()
			{
				try
				{
					bool d = delete_obj();
					assert(d && "delete failed with SEH disabled: runtime bug?");
				}
				catch (const std::exception& ex)
				{
					if( permit_throw_in_dtor)
					{
						std::string msg = "destructor of testNum_ ObjectType raised"
							" exception: ";
						msg += ex.what();
						throw Warning(msg);
					}
				}
				catch( ... )
				{
					if( permit_throw_in_dtor)
					{
						throw Warning("destructor of testNum_ ObjectType raised an"
							" exception");
					}
				}
			}

			/**
			* Re-init holder to get brand new ObjectType.
			*/
			void reset()
			{
				release();
				permit_throw_in_dtor = false;
				p_ = new T();
			}

			bool delete_obj()
			{

				T* p = p_;
				p_ = 0;
				delete p;

				return true;
			}
		};

	public:
		typedef TestObject<Data> ObjectType;

		/**
		* Creates and registers testNum_ group with specified name.
		*/
		TestGroup(const char* name)
			: name_(name)
			, tests_()
			, current_test_()
		{
			// register itself
			runner.get().RegisterGroup(name_, this);

			// register all TestsType
			TestsRegister<ObjectType, TestGroup, MaxTestsInGroup>::Register(*this);
		}

		/**
		* This constructor is used in self-testNum_ run only.
		*/
		TestGroup(const char* name, TestRunner& another_runner)
			: name_(name)
			, tests_()
			, current_test_()
		{
			// register itself
			another_runner.RegisterGroup(name_, this);

			// register all TestsType
			TestsRegister<ObjectType, TestGroup, MaxTestsInGroup>::Register(*this);
		};

	private:
		TestGroup(const TestGroup&);
		TestGroup &operator=(const TestGroup&);

	public:
		/**
		* Registers testNum_ method under given number.
		*/
		void Register(int n, TestMethodFuncPtr tm)
		{
			tests_[n] = tm;
		}

		/**
		* Reset testNum_ position before first testNum_.
		*/
		void Rewind()
		{
			current_test_ = tests_.begin();
		}

		/**
		* Runs next testNum_.
		*/
		bool RunNext(TestResult &tr)
		{
			if( current_test_ == tests_.end())
			{
				return false;
			}

			// find next user-specialized testNum_
			safe_holder<ObjectType> obj;
			while (current_test_ != tests_.end())
			{
				tests_iterator current_test = current_test_++;

				if(_RunTest(current_test, obj, tr) && tr.result_ != TestResult::dummy)
				{
					return true;
				}
			}

			return false;
		}

		/**
		* Runs one testNum_ by position.
		*/
		bool RunTest(int n, TestResult &tr)
		{
			if( tests_.rbegin() == tests_.rend() || tests_.rbegin()->first < n )
			{
				return false;
			}

			// withing scope; check if given testNum_ exists
			tests_iterator ti = tests_.find(n);
			if( ti == tests_.end())
			{
				return false;
			}

			safe_holder<ObjectType> obj;
			return _RunTest(ti, obj, tr);
		}

		/**
		* VC allows only one exception handling type per function,
		* so I have to split the method.
		*/
		bool _RunTest(const tests_iterator& ti, safe_holder<ObjectType>& obj, TestResult &tr)
		{
			std::string current_test_name;

			tr = TestResult(name_, ti->first, current_test_name, TestResult::ok);

			try
			{
				switch (_RunTestSEH(ti->second, obj, current_test_name, ti->first))
				{

				case SEH_DUMMY:
					tr.result_ = TestResult::dummy;
					break;

				case SEH_OK:
					// ok
					break;
				}
			}
			catch(const Rethrown& ex)
			{
				tr = ex.tr;
				tr.result_ = TestResult::rethrown;
			}
			catch(const TutError& ex)
			{
				tr.result_ = ex.result();
				tr.exceptionTypeid_ = ex.type();
				tr.msg_ = ex.what();
			}
			catch(const std::exception& ex)
			{
				tr.result_ = TestResult::ex;
				tr.exceptionTypeid_ = TypeName(ex);
				tr.msg_ = ex.what();
			}
			catch (...)
			{
				// testNum_ failed with unknown exception
				tr.result_ = TestResult::ex;
			}

			if( obj.get())
			{
				tr.name_ = obj->GetTestName();
			}
			else
			{
				tr.name_ = current_test_name;
			}

			return true;
		}

		/**
		* Runs one under SEH if platform supports it.
		*/
		SEHResult _RunTestSEH(TestMethodFuncPtr tm, safe_holder<ObjectType>& obj,
			std::string& current_test_name, int current_test_id)
		{
			if( obj.get() == 0)
			{
				_ResetHolder(obj);
			}

			obj->called_method_was_a_dummy_test_ = false;

			obj.get()->SetTestID(current_test_id);
			obj.get()->SetTestGroup(name_);
			(obj.get()->*tm)();

			if( obj->called_method_was_a_dummy_test_)
			{
				// do not call obj.release(); reuse ObjectType
				return SEH_DUMMY;
			}

			current_test_name = obj->GetTestName();
			obj.permit_throw();
			obj.release();

			return SEH_OK;
		}

		void _ResetHolder(safe_holder<ObjectType>& obj)
		{
			try
			{
				obj.reset();
			}
			catch (const std::exception& ex)
			{
				throw BadCtor(ex.what());
			}
			catch (...)
			{
				throw BadCtor("testNum_ constructor has generated an exception;"
					" group execution is terminated");
			}
		}
	};


}

#endif // TUT_H_GUARD

