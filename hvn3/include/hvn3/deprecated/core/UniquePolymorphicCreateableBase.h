#pragma once
#include <memory>

namespace hvn3 {
	namespace System {

		template <typename ptr_type>
		class UniquePolymorphicCreateableBase {

		public:
			template<typename T, typename ... Args>
			static std::unique_ptr<ptr_type> Create(Args &&... args) {

				return std::make_unique<T>(std::forward<Args>(args)...);

			}
			template<typename T>
			static std::unique_ptr<ptr_type> Create(T* ptr) {

				return std::unique_ptr<T>(ptr);

			}

		};

	}
}