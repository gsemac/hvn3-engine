#include "hvn3/xml/xml_attribute.h"

#include <cassert>

namespace hvn3 {
	namespace xml {

		// Public members

		XmlAttribute::XmlAttribute() {

			this->namePtr = nullptr;
			this->valuePtr = nullptr;
			this->readOnly = true;

		}
		XmlAttribute::XmlAttribute(const string_type* name, string_type* value) :
			namePtr(name),
			valuePtr(value) {

			assert(namePtr != nullptr);
			assert(valuePtr != nullptr);

			readOnly = false;

		}
		XmlAttribute::XmlAttribute(const string_type* name, const string_type* value) :
			namePtr(name),
			valuePtr(value) {

			assert(namePtr != nullptr);
			assert(valuePtr != nullptr);

			readOnly = true;

		}

		const XmlAttribute::string_type& XmlAttribute::Name() const {

			assert(namePtr != nullptr);

			return *namePtr;

		}
		const XmlAttribute::string_type& XmlAttribute::Value() const {

			assert(valuePtr != nullptr);

			return *valuePtr;

		}
		void XmlAttribute::SetValue(const string_type& value) {

			assert(!readOnly);
			assert(valuePtr != nullptr);

			*const_cast<string_type*>(valuePtr) = value;

		}

	}
}