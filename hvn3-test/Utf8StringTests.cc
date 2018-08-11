#include "CppUnitTest.h"
#include "hvn3/core/Framework.h"
#include "hvn3/utility/Utf8String.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace hvn3;

namespace hvn3test {

	TEST_CLASS(UnitTest1)
	{

		TEST_CLASS_INITIALIZE(initialization)
		{
			System::Framework::Initialize();
		}
		TEST_CLASS_CLEANUP(cleanup)
		{
			System::Framework::Shutdown();
		}

	public:
		TEST_METHOD(TestMethod1)
		{

			Assert::Fail();

		}

	};

}