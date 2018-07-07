#pragma once
#include <cstddef>

namespace hvn3 {

	/*
	Base class to simplify the creation of custom iterators.
	The interface is inspired by: https://www.boost.org/doc/libs/1_65_0/libs/iterator/doc/iterator_facade.html
	*/

	template <typename DerivedType, typename ValueType, typename DifferenceType = ptrdiff_t>
	class IteratorTemplate {

	protected:
		typedef IteratorTemplate<DerivedType, ValueType> base_type;

	public:
		typedef typename DifferenceType difference_type;
		typedef typename ValueType value_type;
		typedef typename ValueType& reference;
		typedef typename ValueType* pointer;

		bool operator==(const DerivedType& rhs) const {
			return static_cast<const DerivedType*>(this)->Equal(rhs);
		}
		bool operator!=(const DerivedType& rhs) const {
			return !(static_cast<const DerivedType*>(this)->Equal(rhs));
		}

		DerivedType& operator++() {
			
			_derivedPtr()->Increment();
			
			return _derivedRef();

		}
		DerivedType operator++(int) {
			
			DerivedType iter(*this);
			
			_derivedPtr()->Increment();
			
			return iter;

		}
		DerivedType& operator--() {
			
			_derivedPtr()->Decrement();
			
			return _derivedRef();

		}
		DerivedType operator--(int) {
			
			DerivedType iter(*this);
			
			_derivedPtr()->Decrement();
			
			return iter;

		}
		DerivedType operator+(difference_type rhs) const {

			DerivedType iter = _derivedConstRef();
			iter += rhs;

			return iter;

		}
		DerivedType& operator+=(difference_type rhs) {

			_derivedPtr()->Advance(rhs);

			return _derivedRef();

		}
		DerivedType operator-(difference_type rhs) const {

			DerivedType iter = _derivedConstRef();
			iter -= rhs;

			return iter;

		}
		DerivedType& operator-=(difference_type rhs) {

			_derivedPtr()->Advance(-rhs);

			return _derivedRef();

		}
		
		pointer operator->() {
			return &static_cast<DerivedType*>(this)->Dereference();
		}
		reference operator*() {
			return static_cast<DerivedType*>(this)->Dereference();
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