#pragma once
#include <cstddef>
#include <iterator>
#include <memory>

namespace hvn3 {

	enum class AbstractIteratorComparisonMode {
		CompareByValue,
		CompareByAddress
	};

	template <typename ValueType, AbstractIteratorComparisonMode ComparisonMode = AbstractIteratorComparisonMode::CompareByAddress>
	class AbstractIterator {

		template<typename Iter, AbstractIteratorComparisonMode ComparisonMode>
		friend AbstractIterator<typename Iter::value_type, ComparisonMode> MakeAbstractIterator(Iter iterator);

		typedef AbstractIterator<ValueType, ComparisonMode> this_type;

	public:
		typedef ptrdiff_t difference_type;
		typedef typename ValueType value_type;
		typedef typename ValueType& reference;
		typedef typename ValueType* pointer;

		AbstractIterator(const this_type& other) {
			ptr = std::move(other.ptr->Clone());
		}

		bool operator==(const this_type& rhs) const {
			return ptr->Equal(rhs);
		}
		bool operator!=(const this_type& rhs) const {
			return !ptr->Equal(rhs);
		}

		this_type& operator++() {
			ptr->Increment();
			return *this;
		}
		this_type operator++(int) {
			this_type iter(*this);
			ptr->Increment();
			return iter;
		}
		this_type& operator--() {
			ptr->Decrement();
			return *this;
		}
		this_type operator--(int) {
			this_type iter(*this);
			ptr->Decrement();
			return iter;
		}
		this_type operator+(difference_type rhs) const {
			this_type iter(*this);
			iter += rhs;
			return iter;
		}
		this_type& operator+=(difference_type rhs) {
			ptr->Advance(rhs);
			return *this;
		}
		this_type operator-(difference_type rhs) const {
			this_type iter(*this);
			iter -= rhs;
			return iter;
		}
		this_type& operator-=(difference_type rhs) {
			ptr->Advance(-rhs);
			return *this;
		}

		pointer operator->() {
			return &ptr->Dereference();
		}
		reference operator*() {
			return ptr->Dereference();
		}
		const reference operator*() const {
			return ptr->Dereference();
		}
		reference operator[](difference_type index) {
			return *(*this + index);
		}

	private:
		AbstractIterator() {}

		class IteratorInterface {
		public:
			virtual void Increment() = 0;
			virtual void Decrement() = 0;
			virtual reference Dereference() = 0;
			virtual const reference Dereference() const = 0;
			virtual bool Equal(const IteratorInterface& other) const = 0;
			virtual void Advance(difference_type n) = 0;
			virtual std::unique_ptr<IteratorInterface> Clone() const = 0;
		};

		template <typename IteratorType>
		class IteratorWrapper : public IteratorInterface {

			typedef IteratorWrapper<IteratorType> this_type;

		public:
			typedef IteratorType iterator_type;

			IteratorWrapper(iterator_type iterator) :
				_iter(iterator) {
			}
			virtual void Increment() override {
				++_iter;
			}
			virtual void Decrement() override {
				--_iter;
			}
			virtual reference Dereference() override {
				return *_iter;
			}
			virtual const reference Dereference() const override {
				return *_iter;
			}
			virtual bool Equal(const IteratorInterface& other) const override {
				return _equal(other);
			}
			virtual void Advance(difference_type n) override {
				std::advance(_iter, n);
			}
			virtual std::unique_ptr<IteratorInterface> Clone() const override {
				return std::unique_ptr<IteratorInterface>(new this_type(_iter));
			}

		private:
			iterator_type _iter;

			template<AbstractIteratorComparisonMode mode = ComparisonMode>
			typename std::enable_if<mode == AbstractIteratorComparisonMode::CompareByValue, bool>::type _equal(const IteratorInterface& other) const {
				return Dereference() == other.Dereference();
			}
			template<AbstractIteratorComparisonMode mode = ComparisonMode>
			typename std::enable_if<mode == AbstractIteratorComparisonMode::CompareByAddress, bool>::type _equal(const IteratorInterface& other) const {
				return &Dereference() == &other.Dereference();
			}

		};

		std::unique_ptr<IteratorInterface> ptr;

	};

	template<typename Iter, AbstractIteratorComparisonMode ComparisonMode = AbstractIteratorComparisonMode::CompareByAddress>
	AbstractIterator<typename Iter::value_type, ComparisonMode> MakeAbstractIterator(Iter iterator) {

		using abstract_iterator_type = AbstractIterator<typename Iter::value_type, ComparisonMode>;
		using interface_type = typename abstract_iterator_type::IteratorInterface;
		using interface_ptr_type = std::unique_ptr<interface_type>;
		using wrapper_type = typename abstract_iterator_type::IteratorWrapper<Iter>;

		abstract_iterator_type iter;

		interface_ptr_type wrapper_ptr(new wrapper_type(iterator));
		iter.ptr = std::move(wrapper_ptr);

		return iter;
	}

}