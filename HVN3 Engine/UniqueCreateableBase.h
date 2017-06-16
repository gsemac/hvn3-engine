#pragma once
#include <memory>

namespace hvn3 {

	template <typename ptr_type, typename my_type>
	class UniqueCreateableBase {

	public:
		typedef ptr_type PtrType;

		template<typename ... Args>
		static std::unique_ptr<ptr_type> Create(Args &&... args) {

			return std::make_unique<my_type>(std::forward<Args>(args)...);

		}
		static std::unique_ptr<ptr_type> Create(my_type* ptr) {

			return std::unique_ptr<ptr_type>(ptr);

		}

	};

}