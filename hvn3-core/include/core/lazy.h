#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <mutex>
#include <type_traits>
#include <utility>

namespace hvn3::core {

	template<typename DataType, typename FactoryFunc>
	class Lazy {

	public:
		Lazy(FactoryFunc factoryFunc) :
			factoryFunc(factoryFunc),
			state(State::Uninitialized),
			data{} {
		}
		Lazy(const Lazy& other) :
			Lazy(other.factoryFunc) {

			Copy(other);

		}
		Lazy(Lazy&& other) noexcept :
			Lazy(std::move(other.factoryFunc)) {

			Move(std::move(other));

		}

		Lazy& operator=(const Lazy& other) {

			Copy(other);

		}
		Lazy& operator=(Lazy&& other) {

			Move(std::move(other));

		}

		~Lazy() {

			if (IsValueCreated())
				Value().~DataType();

		}

		bool IsValueCreated() const {

			return state == State::Initialized;

		}
		DataType& Value() {

			Initialize();

			assert(state == State::Initialized);

			return *reinterpret_cast<DataType*>(&data[0]);

		}
		const DataType& Value() const {

			Initialize();

			assert(state == State::Initialized);

			return *reinterpret_cast<DataType*>(&data[0]);

		}

		DataType* operator->() {

			return &Value();

		}
		const DataType* operator->() const {

			return &Value();

		}

		explicit operator bool() const {

			return IsValueCreated();

		}

	private:
		enum class State :
			std::uint8_t {
			Uninitialized,
			Initialized,
			Moved
		};

		alignas(DataType) mutable std::byte data[sizeof(DataType)];
		FactoryFunc factoryFunc;
		mutable State state;
		mutable std::once_flag flag;

		void Initialize() const {

			if (state == State::Uninitialized) {

				std::call_once(flag,
					[this]() {

						new (reinterpret_cast<DataType*>(&data[0])) DataType(factoryFunc());

						state = State::Initialized;

					});

			}

		}

		template<typename U = DataType, std::enable_if_t<std::is_copy_constructible_v<U>, int> = 0>
		void Copy(const Lazy& other) {

			state = other.state;

			if (other.IsValueCreated()) {

				DestroyData();

				new (reinterpret_cast<DataType*>(&data[0])) DataType(other.Value());

			}

		}
		template<typename U = DataType, std::enable_if_t<!std::is_copy_constructible_v<U>, int> = 0>
		void Copy(const Lazy& other) {

			assert(!other.IsValueCreated());

			state = other.state;

		}
		void Move(Lazy&& other) {

			state = other.state;

			if (other.IsValueCreated()) {

				DestroyData();

				new (reinterpret_cast<DataType*>(&data[0])) DataType(std::move(other.Value()));

			}

			other.state = State::Moved;

		}
		void DestroyData() {

			if (IsValueCreated())
				Value().~DataType();

		}

	};

	template<typename FactoryFunc>
	auto make_lazy(FactoryFunc factoryFunc) -> Lazy<decltype(factoryFunc()), FactoryFunc> {

		return Lazy<decltype(factoryFunc()), FactoryFunc>(std::forward<FactoryFunc>(factoryFunc));

	}

}