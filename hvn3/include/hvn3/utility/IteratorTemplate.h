#pragma once
#include <cstddef>

namespace hvn3 {

	/*
	Base class to simplify the creation of custom iterators.
	The interface is inspired by: https://www.boost.org/doc/libs/1_65_0/libs/iterator/doc/iterator_facade.html

	Inherited classes must define:

	void increment() {}
	void decrement() {}
	reference dereference() {}
	bool equal(const derived_type& rhs) const {}
	void advance(difference_type n) {}

	*/

	template <typename DerivedType, typename ValueType, typename DifferenceType = ptrdiff_t>
	class IteratorTemplate {

	protected:
		typedef IteratorTemplate<DerivedType, ValueType> base_type;
		typedef DerivedType derived_type;

	public:
		typedef typename DifferenceType difference_type;
		typedef typename ValueType value_type;
		typedef typename ValueType& reference;
		typedef typename ValueType* pointer;

		bool operator==(const DerivedType& rhs) const {
			return static_cast<const DerivedType*>(this)->equal(rhs);
		}
		bool operator!=(const DerivedType& rhs) const {
			return !(static_cast<const DerivedType*>(this)->equal(rhs));
		}

		DerivedType& operator++() {

			_derivedPtr()->increment();

			return _derivedRef();

		}
		DerivedType operator++(int) {

			DerivedType iter(*this);

			_derivedPtr()->increment();

			return iter;

		}
		DerivedType& operator--() {

			_derivedPtr()->decrement();

			return _derivedRef();

		}
		DerivedType operator--(int) {

			DerivedType iter(*this);

			_derivedPtr()->decrement();

			return iter;

		}
		DerivedType operator+(difference_type rhs) const {

			DerivedType iter = _derivedConstRef();
			iter += rhs;

			return iter;

		}
		DerivedType& operator+=(difference_type rhs) {

			_derivedPtr()->advance(rhs);

			return _derivedRef();

		}
		DerivedType operator-(difference_type rhs) const {

			DerivedType iter = _derivedConstRef();
			iter -= rhs;

			return iter;

		}
		DerivedType& operator-=(difference_type rhs) {

			_derivedPtr()->advance(-rhs);

			return _derivedRef();

		}

		pointer operator->() {
			return &static_cast<DerivedType*>(this)->dereference();
		}
		reference operator*() {
			return static_cast<DerivedType*>(this)->dereference();
		}
		reference operator[](difference_type index) {
			return *(*this + index);
		}

	private:
		DerivedType& _derivedRef() {
			return static_cast<DerivedType&>(*this);
		}
		const DerivedType& _derivedConstRef() const {
			return static_cast<const DerivedType&>(*this);
		}
		DerivedType* _derivedPtr() {
			return static_cast<DerivedType*>(this);
		}

	};

}