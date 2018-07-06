#include "hvn3/xml/XmlUtils.h"
#include "hvn3/xml/XmlWriter.h"
#include <exception>
#include <fstream>
#include <sstream>

namespace hvn3 {
	namespace Xml {

		XmlWriter::XmlWriter(const std::string& root) :
			_root(root) {
		}
		XmlNode& XmlWriter::Root() {
			return _root;
		}
		const XmlNode& XmlWriter::Root() const {
			return _root;
		}
		bool XmlWriter::Save(const std::string& file_path) const {

			std::ofstream buf(file_path.c_str());

			if (!buf.is_open())
				return false;

			_write(buf);

			return true;

		}
		std::string XmlWriter::ToString() const {

			_write_depth = 0;

			std::stringstream buf;

			_write(buf);

			return buf.str();

		}

		void XmlWriter::_write(std::ostream& buf) const {

			_write_depth = 0;

			_writeProlog(buf);
			_writeNode(buf, _root);

		}
		void XmlWriter::_writeProlog(std::ostream& buf) const {
			buf << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		}
		void XmlWriter::_writeNode(std::ostream& buf, const XmlNode& node) const {

			// Write the opening tag and attributes.
			_writeIndent(buf);
			buf << "<" << node.Tag();
			for (auto i = node.AttributesBegin(); i != node.AttributesEnd(); ++i)
				buf << " " << i->first << "=\"" << Xml::EscapeXmlString(i->second) << "\"";
			buf << '>';

			_write_depth += 1;

			// Write text.
			if (node.HasText()) {
				if (node.HasChildren())
					_writeNewLine(buf);
				buf << Xml::EscapeXmlString(node.Text());
			}

			// Write all child nodes.
			if (node.HasChildren()) {

				buf << '\n';

				for (auto i = node.ChildrenBegin(); i != node.ChildrenEnd(); ++i) {

					_writeNode(buf, *i->get());

					if (std::next(i) != node.ChildrenEnd())
						buf << '\n';

				}

			}

			_write_depth -= 1;

			if (node.HasChildren())
				_writeNewLine(buf);

			buf << "</" << node.Tag() << '>';

		}
		void XmlWriter::_writeIndent(std::ostream& buf) const {

			for (unsigned int i = 0; i < _write_depth; ++i)
				buf << '\t';

		}
		void XmlWriter::_writeNewLine(std::ostream& buf) const {

			buf << '\n';
			_writeIndent(buf);

		}

	}
}