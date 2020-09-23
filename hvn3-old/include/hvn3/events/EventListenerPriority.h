#pragma once

#include <limits>

namespace hvn3 {

	class EventListenerPriority {

	public:
		typedef int value_type;

		value_type value;

		enum : value_type {

			HIGH_PRIORITY = std::numeric_limits<value_type>::max(),

			COLLISIONS_UPDATE_PRIORITY = 0,
			OBJECTS_UPDATE_PRIORITY,
			PHYSICS_UPDATE_PRIORITY,

			NORMAL_PRIORITY,

			LOW_PRIORITY = std::numeric_limits<value_type>::min()

		};

		EventListenerPriority() :
			EventListenerPriority(NORMAL_PRIORITY) {
		}
		EventListenerPriority(value_type value) :
			value(value) {
		}
		EventListenerPriority(const EventListenerPriority& other) :
			EventListenerPriority(other.value) {
		}

		EventListenerPriority& operator=(value_type value) {

			this->value = value;

			return *this;

		}
		EventListenerPriority& operator=(const EventListenerPriority& rhs) {
			return *this = rhs.value;
		}

		bool operator==(value_type value) const {
			return this->value == value;
		}
		bool operator!=(value_type value) const {
			return !(*this == value);
		}
		bool operator<(value_type value) const {
			return this->value < value;
		}
		bool operator>(value_type value) const {
			return this->value > value;
		}
		bool operator==(const EventListenerPriority& rhs) const {
			return *this == rhs.value;
		}
		bool operator!=(const EventListenerPriority& rhs) const {
			return !(*this == rhs);
		}
		bool operator<(const EventListenerPriority& rhs) const {
			return *this < rhs.value;
		}
		bool operator>(const EventListenerPriority& rhs) const {
			return *this > rhs.value;
		}

		explicit operator value_type() const {
			return value;
		}

	};

}