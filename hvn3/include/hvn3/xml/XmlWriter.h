#pragma once
#include "hvn3/xml/XmlNode.h"
#include <iostream>

namespace hvn3 {
	namespace Xml {

		class XmlWriter {

		public:
			XmlWriter(const std::string& root);

			XmlNode& Root();
			const XmlNode& Root() const;

			bool Save(const std::string& file_path) const;
			std::string ToString() const;

		private:
			XmlNode _root;
			mutable unsigned int _write_depth;

			void _write(std::ostream& buf) const;
			void _writeProlog(std::ostream& buf) const;
			void _writeNode(std::ostream& buf, const XmlNode& node) const;
			void _writeIndent(std::ostream& buf) const;
			void _writeNewLine(std::ostream& buf) const;

		};

	}
}