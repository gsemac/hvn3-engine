#include "hvn3/core/ManagerBase.h"
#include "hvn3/core/ManagerRegistry.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace hvn3;

namespace hvn3test {

	TEST_CLASS(ManagerRegistryTests) {

		class TestManagerInterface {

		public:
			virtual int foo() = 0;

		};

		class TestManager :
			public ManagerBase<TestManagerInterface> {

		public:
			static const int FOO_VALUE;

			int foo() override { return FOO_VALUE; }

		};

public:
	TEST_METHOD(TestManagerIsRegistered) {

		ManagerRegistry registry;

		registry.Register<TestManager>();

		Assert::IsTrue(registry.IsRegistered<TestManager>());

	}
	TEST_METHOD(TestManagerIsNotRegistered) {

		ManagerRegistry registry;

		Assert::IsFalse(registry.IsRegistered<TestManager>());

	}
	TEST_METHOD(TestManagerCallMethod) {

		ManagerRegistry registry;

		registry.Register<TestManager>();

		Assert::IsTrue(registry.GetManager<TestManager>()->foo() == TestManager::FOO_VALUE);

	}
	TEST_METHOD(TestRemoveManagerManagerRemoved) {

		ManagerRegistry registry;

		registry.Register<TestManager>();

		registry.Deregister<TestManager>();

		Assert::IsFalse(registry.IsRegistered<TestManager>());
		Assert::IsFalse(registry.IsRegistered<TestManagerInterface>());

	}
	TEST_METHOD(TestRemoveManagerInterfaceRemoved) {

		ManagerRegistry registry;

		registry.Register<TestManager>();

		registry.Deregister<TestManager>();

		Assert::IsFalse(registry.IsRegistered<TestManagerInterface>());

	}

	TEST_METHOD(TestManagerIsRegisteredByInterface) {

		ManagerRegistry registry;

		registry.Register<TestManager>();

		Assert::IsTrue(registry.IsRegistered<TestManagerInterface>());

	}
	TEST_METHOD(TestManagerIsNotRegisteredByInterface) {

		ManagerRegistry registry;

		Assert::IsFalse(registry.IsRegistered<TestManagerInterface>());

	}
	TEST_METHOD(TestManagerCallMethodByInterface) {

		ManagerRegistry registry;

		registry.Register<TestManager>();

		Assert::IsTrue(registry.GetManager<TestManagerInterface>()->foo() == TestManager::FOO_VALUE);

	}

	};

	const int ManagerRegistryTests::TestManager::FOO_VALUE = 5;

}