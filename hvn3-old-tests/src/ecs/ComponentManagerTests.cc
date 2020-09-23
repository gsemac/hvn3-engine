#include "hvn3/ecs/ComponentManager.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace hvn3::ecs;

namespace hvn3test {

	TEST_CLASS(ComponentManagerTests) {

public:
	TEST_METHOD(TestAddComponent) {

		ComponentManager m;
		Entity entity(1);

		m.AddComponent(entity, 5);

		Assert::IsTrue(m.Count() == 1);

	}
	TEST_METHOD(TestGetComponent) {

		ComponentManager m;
		Entity entity(1);

		m.AddComponent(entity, 5);

		auto c = m.GetComponent<int>(entity);

		Assert::IsTrue(*c == 5);

	}
	TEST_METHOD(TestRemoveComponent) {

		ComponentManager m;
		Entity entity(1);

		m.AddComponent(entity, 5);
		m.RemoveComponent<int>(entity);

		auto c = m.GetComponent<int>(entity);

		Assert::IsTrue(!c);

	}

	};

}