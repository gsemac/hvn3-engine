#pragma once
#include <memory>

namespace hvn3 {
	namespace System {

		template <typename ptr_type>
		class SharedCreatableBase {

		public:
			template<typename T, typename ... Args>
			static std::shared_ptr<ptr_type> Create(Args &&... args) {

				return std::make_shared<T>(std::forward<Args>(args)...);

			}
			template<typename T>
			static std::shared_ptr<ptr_type> Create(T* ptr) {

				return std::shared_ptr<T>(ptr);

			}

		};

	}
}