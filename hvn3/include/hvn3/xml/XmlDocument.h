#pragma once
#include "hvn3/xml/XmlElement.h"
#include <iostream>

namespace hvn3 {
	namespace Xml {

		class XmlDocument {

		public:
			XmlDocument();
			XmlDocument(const std::string& root_tag);

			XmlElement& Root();
			const XmlElement& Root() const;

			bool Save(const std::string& filePath) const;
			std::string ToString() const;

			static XmlDocument Open(const std::string& filePath);
			static XmlDocument Parse(const std::string& xml);

		private:
			XmlElement _root;
			mutable unsigned int _write_depth;

			void _write(std::ostream& buf) const;
			void _writeProlog(std::ostream& buf) const;
			void _writeNode(std::ostream& buf, const XmlElement& node) const;
			void _writeIndent(std::ostream& buf) const;
			void _writeNewLine(std::ostream& buf) const;

			void _read(std::istream& buf);

		};

	}
}