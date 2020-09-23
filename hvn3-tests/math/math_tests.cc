#include "CppUnitTest.h"

#include <math/math.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hvn3::math::tests {

	TEST_CLASS(MathTests) {

public:

	TEST_METHOD(TestWrapWithZeroMinAndPositiveMax) {

		Assert::AreEqual(0.9f, Wrap(1.9f, 0.0f, 1.0f));

	}
	TEST_METHOD(TestWrapWithZeroMinAndPositiveMaxWithValueEvenlyDivisibleByMax) {

		Assert::AreEqual(0.0f, Wrap(2.0f, 0.0f, 1.0f));

	}
	TEST_METHOD(TestWrapWithZeroMinAndPositiveMaxWithValueEqualToMax) {

		Assert::AreEqual(0.0f, Wrap(1.0f, 0.0f, 1.0f));

	}
	TEST_METHOD(TestWrapWithPositiveMinAndPositiveMax) {

		Assert::AreEqual(3, Wrap(10, 1, 5));

	}

	};

}