#ifndef TUT_MAIN_H
#define TUT_MAIN_H

#include "tut.hpp"
#include "tut_console_reporter.hpp"
#include "tut_cppunit_reporter.hpp"


namespace tut
{

	/** Helper function to make test binaries simpler.
	*
	* Example of basic usage follows.
	*
	* @code
	*  namespace tut { test_runner_singleton runner; }
	*
	*  int main(int argc, char **argv)
	*  {
	*      if( TutMain(argc, argv) )
	*          return 0;
	*      else
	*          return -1;
	*  }
	* @endcode
	*
	* It is also possible to do some generic initialization before
	* running any tests and cleanup before exiting application.
	* Note that TutMain can throw tut::no_such_group or tut::NoSuchTest.
	*
	* @code
	*  namespace tut { test_runner_singleton runner; }
	*
	*  int main(int argc, char **argv)
	*  {
	*      tut::xml_reporter reporter;
	*      tut::runner.get().insert_callback(&reporter);
	*
	*      MyInit();
	*      try
	*      {
	*          TutMain(argc, argv);
	*      }
	*      catch(const tut::tut_error &ex)
	*      {
	*          std::cerr << "TUT error: " << ex.what() << std::endl;
	*      }
	*      MyCleanup();
	*  }
	* @endcode
	*/
	inline bool TutMain(int argc, const char * const * const argv, std::ostream &os = std::cerr)
	{
		std::stringstream usage;
		usage << "Usage: " << argv[0] << " [group] [testcase]" << std::endl;

		GroupNames gr = runner.get().ListGroups();
		usage << "Available test groups:" << std::endl;

		for(GroupNames::const_iterator i = gr.begin(); i != gr.end(); ++i)
		{
			usage << "    " << *i << std::endl;
		}

		if(argc>1)
		{
			if(std::string(argv[1]) == "-h" ||
				std::string(argv[1]) == "--help" ||
				std::string(argv[1]) == "/?" ||
				argc > 3)
			{
				os << usage.rdbuf();
				return false;
			}
		}

		// Check command line options.
		switch(argc)
		{
		case 1:
			runner.get().RunTests();
			break;

		case 2:
			runner.get().RunTests(argv[1]);
			break;

		case 3:
			{
				char *end;
				int t = strtol(argv[2], &end, 10);
				if(end != argv[2] + strlen(argv[2]))
				{
					throw NoSuchTest("`" + std::string(argv[2]) + "` should be a number");
				}

				TestResult tr;
				if(!runner.get().RunTest(argv[1], t, tr) || tr.result_ == TestResult::dummy)
				{
					throw NoSuchTest("No testcase `" + std::string(argv[2]) + "` in group `" + argv[1] + "`");
				}
			}
			break;
		}

		return true;
	} // TutMain()

}

#endif
