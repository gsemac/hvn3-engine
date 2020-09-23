#include "hvn3/utility/StringUtils.h"
#include "hvn3/utility/Utf8String.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace hvn3;

namespace hvn3test {

	TEST_CLASS(StringUtilsTests) {

public:
	TEST_METHOD(TestStartsWithWithStringTypeAndConstCharReturnsTrue) {

		String str = "hello world";

		Assert::IsTrue(StringUtils::StartsWith(str, "hello"));

	}
	TEST_METHOD(TestStartsWithWithStringTypeAndConstCharReturnsTrueWithEqualString) {

		String str = "hello world";

		Assert::IsTrue(StringUtils::StartsWith(str, "hello world"));

	}
	TEST_METHOD(TestStartsWithWithStringTypeAndConstCharReturnsFalse) {

		String str = "hello world";

		Assert::IsFalse(StringUtils::StartsWith(str, "world"));
		Assert::IsFalse(StringUtils::StartsWith(str, "hello world hello"));

	}
	TEST_METHOD(TestStartsWithWithStringTypeAndConstCharOfLength1ReturnsFalse) {

		String str = "hello world";

		Assert::IsFalse(StringUtils::StartsWith(str, "."));

	}
	TEST_METHOD(TestEndsWithWithStringTypeAndConstCharReturnsTrue) {

		String str = "hello world";

		Assert::IsTrue(StringUtils::EndsWith(str, "world"));

	}
	TEST_METHOD(TestEndsWithWithStringTypeAndConstCharReturnsTrueWithEqualString) {

		String str = "hello world";

		Assert::IsTrue(StringUtils::EndsWith(str, "hello world"));

	}
	TEST_METHOD(TestEndsWithWithStringTypeAndConstCharReturnsFalse) {

		String str = "hello world";

		Assert::IsFalse(StringUtils::EndsWith(str, "hello"));
		Assert::IsFalse(StringUtils::EndsWith(str, "hello world hello"));

	}
	TEST_METHOD(TestEndsWithWithStringTypeAndConstCharOfLength1ReturnsFalse) {

		String str = "hello world";

		Assert::IsFalse(StringUtils::EndsWith(str, "."));

	}
	TEST_METHOD(TestEndsWithWithStringTypeAndStringTypeOfLesserLength) {

		String str = "hello world";
		String suffix = "world";

		Assert::IsTrue(StringUtils::EndsWith(str, suffix));

	}

	};

}