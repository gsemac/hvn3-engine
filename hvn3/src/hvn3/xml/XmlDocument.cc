#include "hvn3/xml/XmlDocument.h"

#include "hvn3/io/IOException.h"
#include "hvn3/xml/XmlLexer.h"
#include "hvn3/xml/XmlUtils.h"

#include <fstream>
#include <sstream>
#include <stack>

namespace hvn3 {
	namespace Xml {

		XmlDocument::XmlDocument() :
			XmlDocument("") {}
		XmlDocument::XmlDocument(const std::string& root_tag) :
			_root(root_tag) {
		}
		XmlElement& XmlDocument::Root() {
			return _root;
		}
		const XmlElement& XmlDocument::Root() const {
			return _root;
		}
		bool XmlDocument::Save(const std::string& file_path) const {

			std::ofstream buf(file_path.c_str());

			if (!buf.is_open())
				return false;

			_write(buf);

			return true;

		}
		std::string XmlDocument::ToString() const {

			_write_depth = 0;

			std::stringstream buf;

			_write(buf);

			return buf.str();

		}

		XmlDocument XmlDocument::Open(const std::string& filePath) {

			std::ifstream buf(filePath.c_str());

			if (!buf.is_open())
				throw IO::IOException(filePath);

			XmlDocument doc("");
			doc._read(buf);

			return doc;

		}
		XmlDocument XmlDocument::Parse(const std::string& xml) {

			std::stringstream buf(xml);

			XmlDocument doc("");
			doc._read(buf);

			return doc;

		}

		void XmlDocument::_write(std::ostream& buf) const {

			_write_depth = 0;

			_writeProlog(buf);
			_writeNode(buf, _root);

		}
		void XmlDocument::_writeProlog(std::ostream& buf) const {
			buf << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		}
		void XmlDocument::_writeNode(std::ostream& buf, const XmlElement& node) const {

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
		void XmlDocument::_writeIndent(std::ostream& buf) const {

			for (unsigned int i = 0; i < _write_depth; ++i)
				buf << '\t';

		}
		void XmlDocument::_writeNewLine(std::ostream& buf) const {

			buf << '\n';
			_writeIndent(buf);

		}
		void XmlDocument::_read(std::istream& buf) {

			std::stack<XmlElement*> nodes;
			std::string current_attribute;
			bool parsing_error = false;

			XmlLexerToken token;
			XmlLexer lexer(buf);

			while (!parsing_error && lexer >> token) {

				switch (token.type) {

				case XmlLexerTokenType::OpenTag:
					if (nodes.size() <= 0) {
						nodes.push(&Root());
						nodes.top()->SetTag(token.value);
					}
					else
						nodes.push(nodes.top()->AddChild(token.value));
					break;

				case XmlLexerTokenType::CloseTag:
					nodes.pop();
					break;

				case XmlLexerTokenType::AttributeName:
					current_attribute = token.value;
					break;

				case XmlLexerTokenType::AttributeValue:
					if (current_attribute.length() <= 0)
						parsing_error = true;
					else if (nodes.size() > 0) {
						nodes.top()->SetAttribute(current_attribute, token.value);
						current_attribute.clear();
					}
					break;

				case XmlLexerTokenType::Text:
					if (nodes.size() <= 0)
						parsing_error = true;
					else
						nodes.top()->SetText(token.value);
					break;

				}

			}

		}

	}
}