#pragma once

#include <string>

namespace hvn3 {
	namespace xml {

		class XmlAttribute {

			using string_type = std::string;

		public:
			XmlAttribute();
			XmlAttribute(const string_type* name, string_type* value);
			XmlAttribute(const string_type* name, const string_type* value);

			const string_type& Name() const;
			const string_type& Value() const;
			void SetValue(const string_type& value);

		private:
			const string_type* namePtr;
			const string_type* valuePtr;
			bool readOnly;

		};

	}
}