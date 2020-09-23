#pragma once

namespace hvn3 {
	namespace ecs {

		template<class ComponentType>
		class ComponentHandle {

		public:
			ComponentHandle() :
				ComponentHandle(nullptr) {}
			ComponentHandle(ComponentType* componentPtr) {

				_component = componentPtr;

			}

			ComponentType& operator*() {

				return *_component;

			}
			const ComponentType& operator*() const {

				return *_component;

			}
			ComponentType* operator->() {

				return _component;

			}
			const ComponentType* operator->() const {

				return _component;

			}

			explicit operator bool() const {

				return _component != nullptr;

			}

		private:
			ComponentType* _component;

		};

	}
}