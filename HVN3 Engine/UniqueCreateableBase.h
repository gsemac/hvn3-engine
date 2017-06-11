#pragma once
#include <memory>

namespace hvn3 {

	template <typename ptr_type>
	class UniqueCreateableBase {

	public:
		template<typename ... Args>
		static std::unique_ptr<ptr_type> Create(Args &&... args) {

			return std::make_unique<ptr_type>(std::forward<Args>(args)...);

		}
		static std::unique_ptr<ptr_type> Create(ptr_type* ptr) {

			return std::unique_ptr<ptr_type>(ptr);

		}

	};

}