#ifndef TUT_RUNNER_H_GUARD
#define TUT_RUNNER_H_GUARD


#include "tut_exception.hpp"

namespace tut
{

	/**
	* Interface.
	* Test group operations.
	*/
	struct GroupBase
	{
		virtual ~GroupBase()
		{
		}

		// execute tests iteratively
		virtual void Rewind() = 0;
		virtual bool RunNext(TestResult &) = 0;

		// execute one test
		virtual bool RunTest(int n, TestResult &tr) = 0;
	};


	/**
	* Test runner Callback interface.
	* Can be implemented by caller to update
	* tests results in real-time. User can implement
	* any of Callback methods, and leave unused
	* in default implementation.
	*/
	struct Callback
	{
		/**
		* Default constructor.
		*/
		Callback()
		{}

		/**
		* Virtual destructor is a must for subclassed types.
		*/
		virtual ~Callback()
		{}

		/**
		* Called when new test run started.
		*/
		virtual void RunStart()
		{}

		/**
		* Called when a group started
		* @param name Name of the group
		*/
		virtual void GroupStart(const std::string&)
		{}

		/**
		* Called when a test finished.
		* @param tr Test results.
		*/
		virtual void TestCompleted(const TestResult&)
		{}

		/**
		* Called when a group is completed
		* @param name Name of the group
		*/
		virtual void GroupCompleted(const std::string&)
		{}

		/**
		* Called when all tests in run completed.
		*/
		virtual void RunCompleted()
		{}

		virtual bool AllOK() const
		{
			return true;
		}

	private:
		Callback(const Callback &);
		Callback &operator=(const Callback&);
	};



	/**
	* Typedef for runner::ListGroups()
	*/
	typedef std::vector<std::string> GroupNames;
	typedef std::set<Callback*> Callbacks;


	/**
	* Test runner.
	*/
	class TestRunner
	{
	protected:

		typedef std::map<std::string, GroupBase*>	GroupsType;
		typedef GroupsType::iterator				iterator;
		typedef GroupsType::const_iterator			const_iterator;

		GroupsType groups_;
		Callbacks callbacks_;

	public:
		TestRunner()
			: groups_()
			, callbacks_()
		{}

		/**
		* Stores another group for getting by name.
		* @param name new group object
		* @param gr new Callback object
		*/
		void RegisterGroup(const std::string& name, GroupBase* gr)
		{
			if( gr == 0)
			{
				throw TutError("group shall be non-null");
			}

			if( groups_.find(name) != groups_.end())
			{
				std::string msg("attempt to add already existent group " + name);
				throw TutError(msg);
			}

			groups_.insert( std::make_pair(name, gr) );
		}

		/**
		* Stores one Callback object.
		* @param cb new Callback object
		*/
		void SetCallback(Callback *cb)
		{
			ClearCallbacks();
			InsertCallback(cb);
		}

		/**
		* Add Callback object.
		* @param cb new Callback object
		*/
		void InsertCallback(Callback* cb)
		{
			if( cb != NULL )
			{
				callbacks_.insert(cb);
			}
		}

		/**
		* Remove Callback object.
		* @param cb Callback to remove
		*/
		void EraseCallback(Callback* cb)
		{
			callbacks_.erase(cb);
		}

		/**
		* Remove all Callback objects.
		*/
		void ClearCallbacks()
		{
			callbacks_.clear();
		}

		/**
		* Returns Callback list.
		* @return     Callback list
		*/
		const Callbacks &GetCallbacks() const
		{
			return callbacks_;
		}

		/**
		* Set Callback list.
		* @param cb new Callback list
		*/
		void SetCallbacks(const Callbacks &cb)
		{
			callbacks_ = cb;
		}

		/**
		* Returns list of known test GroupsType.
		* @return     GroupsType list
		*/
		const GroupNames ListGroups() const
		{
			GroupNames ret;
			for(const_iterator i = groups_.begin(); i != groups_.end(); ++i)
			{
				ret.push_back(i->first);
			}

			return ret;
		}

		/**
		* Runs all tests in all GroupsType.
		*/
		void RunTests() const
		{
			_RunStart();

			const_iterator i = groups_.begin();
			const_iterator e = groups_.end();
			while (i != e)
			{
				_GroupStart(i->first);
				_RunAllTests(i);
				_GroupCompleted(i->first);

				++i;
			}

			_RunCompleted();
		}

		/**
		* Runs all tests in specified group.
		* @param group_name group to test
		*/
		void RunTests(const std::string& group_name) const
		{
			_RunStart();

			const_iterator i = groups_.find(group_name);
			if( i == groups_.end())
			{
				_RunCompleted();
				throw NoSuchGroup(group_name);
			}

			_GroupStart(group_name);
			_RunAllTests(i);
			_GroupCompleted(group_name);
			_RunCompleted();
		}

		/**
		* Runs one test in specified group.
		* @param group_name group to test
		* @param n run case in test
		* @param tr result_ of this case
		* @return  true if test is ok, otherwise false
		*/
		bool RunTest(const std::string& group_name, int n, TestResult &tr) const
		{
			_RunStart();

			const_iterator i = groups_.find(group_name);
			if( i == groups_.end())
			{
				_RunCompleted();
				throw NoSuchGroup(group_name);
			}

			_GroupStart(group_name);

			bool t = i->second->RunTest(n, tr);

			if(t && tr.result_ != TestResult::dummy)
			{
				_TestCompleted(tr);
			}

			_GroupCompleted(group_name);
			_RunCompleted();

			return t;
		}

	

	private:
		friend class RestatableWrapper;

		void _RunStart() const
		{
			for(Callbacks::const_iterator i = callbacks_.begin(); i != callbacks_.end(); ++i)
			{
				(*i)->RunStart();
			}
		}

		void _RunCompleted() const
		{
			for(Callbacks::const_iterator i = callbacks_.begin(); i != callbacks_.end(); ++i)
			{
				(*i)->RunCompleted();
			}
		}

		void _GroupStart(const std::string &group_name) const
		{
			for(Callbacks::const_iterator i = callbacks_.begin(); i != callbacks_.end(); ++i)
			{
				(*i)->GroupStart(group_name);
			}
		}

		void _GroupCompleted(const std::string &group_name) const
		{
			for(Callbacks::const_iterator i = callbacks_.begin(); i != callbacks_.end(); ++i)
			{
				(*i)->GroupCompleted(group_name);
			}
		}

		void _TestCompleted(const TestResult &tr) const
		{
			for(Callbacks::const_iterator i = callbacks_.begin(); i != callbacks_.end(); ++i)
			{
				(*i)->TestCompleted(tr);
			}
		}

		void _RunAllTests(const_iterator i) const
		{
			i->second->Rewind();

			TestResult tr;
			while(i->second->RunNext(tr))
			{
				if(tr.result_ != TestResult::dummy)
				{
					_TestCompleted(tr);
				}

				if( tr.result_ == TestResult::ex_ctor)
				{
					// test object ctor failed, skip whole group
					break;
				}
			}
		}
	};

	/**
	* Singleton for TestRunner implementation.
	* Instance with name runner_singleton shall be implemented
	* by user.
	*/
	class TestRunnerSingleton
	{
	public:

		static TestRunner& get()
		{
			static TestRunner tr;
			return tr;
		}
	};

	extern TestRunnerSingleton runner;

}

#endif // TUT_RUNNER_H_GUARD

