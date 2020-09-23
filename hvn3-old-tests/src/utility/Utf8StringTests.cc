#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/utility/Utf8String.h"

#include "CppUnitTest.h"

#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace hvn3;

namespace hvn3test {

	TEST_CLASS(Utf8StringTests)
	{

	public:
		TEST_METHOD(TestConstructorDefault) {

			String str;

			Assert::IsTrue(String::IsNullOrEmpty(str));

		}
		TEST_METHOD(TestMemoryPointsToSameLocationAfterCopyAssignment) {

			String str1 = "hello";
			String str2 = str1;

			Assert::IsTrue(System::AllegroAdapter::ToUStr(str1) == System::AllegroAdapter::ToUStr(str2));

		}
		TEST_METHOD(TestMemoryPointsToDifferentLocationAfterCopyAssignmentFollowedByWrite) {

			String str1 = "hello";
			String str2 = str1;

			str2.Append('w');

			Assert::IsTrue(System::AllegroAdapter::ToUStr(str1) != System::AllegroAdapter::ToUStr(str2));

		}
		TEST_METHOD(TestModifyingStringAfterCopyAssignmentDoesNotModifyOriginalString) {

			String str1 = "hello";
			String str2 = str1;

			str2.Append('w');

			Assert::IsTrue(str1 == "hello");
			Assert::IsTrue(str2 == "hellow");

		}
		TEST_METHOD(TestForwardIteration) {

			String str("hello");
			std::string copy;

			for (auto i = str.begin(); i != str.end(); ++i)
				copy.push_back(*i);

			Assert::IsTrue(copy == "hello");

		}
		TEST_METHOD(TestBackwardIteration) {

			String str("hello");
			std::string copy;

			for (auto i = str.rbegin(); i != str.rend(); ++i)
				copy.push_back(*i);

			Assert::IsTrue(copy == "olleh");

		}
		TEST_METHOD(TestSubstringStartingAtBeginningOfString) {

			String str("hello, world!");

			Assert::IsTrue(str.SubString(5) == "hello");

		}

	};

}