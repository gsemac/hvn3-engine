#include "CppUnitTest.h"

#include "utilities/convert.h"

#include <cstdint>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hvn3::utilities::tests {

	TEST_CLASS(ConvertTests) {

public:
	TEST_METHOD(TestConvertHexStringWith0xPrefixToInt32) {

		uint32_t result = Convert::To<uint32_t>("0x123456");

		Assert::AreEqual(1193046u, result);

	}
	TEST_METHOD(TestConvertHexStringWithHashPrefixToInt32) {

		uint32_t result = Convert::To<uint32_t>("#123456");

		Assert::AreEqual(0x123456u, result);

	}
	TEST_METHOD(TestConvertShorthandHexStringWithHashPrefixToInt32) {

		uint32_t result = Convert::To<uint32_t>("#123");

		Assert::AreEqual(0x112233u, result);

	}

	};

}