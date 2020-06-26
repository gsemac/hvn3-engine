#include "CppUnitTest.h"

#include <math/point.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hvn3::math::tests {

	TEST_CLASS(PointTests) {

public:

	TEST_METHOD(TestGetX) {

		Point2i point(1, 2);

		Assert::AreEqual(1, point.X());

	}
	TEST_METHOD(TestSetX) {

		Point2i point(1, 2);

		point.SetX(3);

		Assert::AreEqual(3, point.X());

	}
	TEST_METHOD(TestGetY) {

		Point2i point(1, 2);

		Assert::AreEqual(2, point.Y());

	}
	TEST_METHOD(TestSetY) {

		Point2i point(1, 2);

		point.SetY(3);

		Assert::AreEqual(3, point.Y());

	}

	TEST_METHOD(TestTo) {

		Point2i point1(1, 2);
		Point2i point2(4, 6);

		Vector2i result = point1.To(point2);

		Assert::IsTrue(Vector2i(3, 4) == result);

	}
	TEST_METHOD(TestTranslateXY) {

		Point2i point(1, 2);

		Point2i result = point.Translate(3, 4);

		Assert::IsTrue(Point2i(4, 6) == result);

	}

	};

}