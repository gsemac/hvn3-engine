#pragma once

#include "hvn3/xml/XmlElement.h"

#include <iostream>

namespace hvn3 {
	namespace Xml {

		class XmlDocument {

		public:
			XmlDocument();
			XmlDocument(const std::string& rootTag);

			XmlElement& Root();
			const XmlElement& Root() const;

			bool Save(const std::string& filePath) const;
			std::string ToString() const;

			static XmlDocument Open(const std::string& filePath);
			static XmlDocument Parse(const std::string& xmlString);

		private:
			XmlElement _root;

			void WriteDocument(std::ostream& outputStream) const;
			void WriteProlog(std::ostream& outputStream) const;
			void WriteNode(std::ostream& outputStream, const XmlElement& node, int depth) const;

			void WriteIndent(std::ostream& outputStream, int depth) const;
			void WriteNewline(std::ostream& outputStream, int depth) const;

			void ReadDocument(std::istream& inputStream);

		};

	}
}