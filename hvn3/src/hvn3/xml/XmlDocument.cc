#include "hvn3/xml/XmlDocument.h"

#include "hvn3/io/IOException.h"
#include "hvn3/xml/XmlLexer.h"
#include "hvn3/xml/XmlUtils.h"

#include <fstream>
#include <sstream>
#include <stack>

namespace hvn3 {
	namespace Xml {

		// Public members

		XmlDocument::XmlDocument() :
			XmlDocument("") {}
		XmlDocument::XmlDocument(const std::string& rootTag) :
			_root(rootTag) {
		}
		XmlElement& XmlDocument::Root() {
			return _root;
		}
		const XmlElement& XmlDocument::Root() const {
			return _root;
		}
		bool XmlDocument::Save(const std::string& filePath) const {

			std::ofstream outputStream(filePath.c_str());

			if (!outputStream.is_open())
				return false;

			writeDocument(outputStream);

			return true;

		}
		std::string XmlDocument::ToString() const {

			std::ostringstream outputStream;

			writeDocument(outputStream);

			return outputStream.str();

		}

		XmlDocument XmlDocument::Open(const std::string& filePath) {

			std::ifstream buf(filePath.c_str());

			if (!buf.is_open())
				throw IO::FileNotFoundException(filePath);

			XmlDocument document;

			document.readDocument(buf);

			return document;

		}
		XmlDocument XmlDocument::Parse(const std::string& xmlString) {

			std::istringstream inputStream(xmlString);

			XmlDocument document;

			document.readDocument(inputStream);

			return document;

		}

		// Private members

		void XmlDocument::writeDocument(std::ostream& outputStream) const {

			writeProlog(outputStream);
			writeNode(outputStream, _root, 0);

		}
		void XmlDocument::writeProlog(std::ostream& outputStream) const {

			outputStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

		}
		void XmlDocument::writeNode(std::ostream& outputStream, const XmlElement& node, int depth) const {

			// Write the opening tag and attributes.

			writeIndent(outputStream, depth);

			outputStream << "<" << node.Tag();

			for (auto i = node.Attributes().begin(); i != node.Attributes().end(); ++i)
				outputStream << " " << i->first << "=\"" << Xml::EscapeXmlString(i->second) << "\"";

			outputStream << '>';

			depth += 1;

			// Write text.

			if (node.HasText()) {

				if (node.HasChildren())
					writeNewline(outputStream, depth);

				outputStream << Xml::EscapeXmlString(node.Text());

			}

			// Write all child nodes.

			if (node.HasChildren()) {

				outputStream << '\n';

				for (auto i = node.Children().begin(); i != node.Children().end(); ++i) {

					writeNode(outputStream, *i->get(), depth);

					if (std::next(i) != node.Children().end())
						outputStream << '\n';

				}

			}

			depth -= 1;

			if (node.HasChildren())
				writeNewline(outputStream, depth);

			outputStream << "</" << node.Tag() << '>';

		}

		void XmlDocument::writeIndent(std::ostream& outputStream, int depth) const {

			for (int i = 0; i < depth; ++i)
				outputStream << '\t';

		}
		void XmlDocument::writeNewline(std::ostream& outputStream, int depth) const {

			outputStream << '\n';

			writeIndent(outputStream, depth);

		}

		void XmlDocument::readDocument(std::istream& inputStream) {

			std::stack<XmlElement*> nodes;
			std::string current_attribute;
			bool parsing_error = false;

			XmlLexerToken token;
			XmlLexer lexer(inputStream);

			while (!parsing_error && lexer >> token) {

				switch (token.type) {

				case XmlLexerTokenType::OpenTag:
					if (nodes.size() <= 0) {
						nodes.push(&Root());
						nodes.top()->SetTag(token.value);
					}
					else
						nodes.push(&(nodes.top()->AddChild(token.value)));
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