#include "hvn3/xml/xml_lexer.h"

namespace hvn3 {
	namespace xml {

		// Public members

		XmlLexer::XmlLexer(std::istream& stream) :
			stream(stream),
			insideTag(false),
			betweenTags(false) {
		}

		bool XmlLexer::operator>>(XmlLexerToken& rhs) {

			ReadNextToken(rhs);

			return rhs.type != XmlLexerTokenType::Eof;

		}

		// Private members

		void XmlLexer::ReadNextToken(XmlLexerToken& rhs) {

			rhs.type = XmlLexerTokenType::Undefined;
			rhs.value.clear();

			char c;

			if (ReadNextChar(c, true)) {

				switch (c) {

				case '<':

					// Reached either a tag (open or closed) or processor instruction.

					ReadNextTagOrProcessorInstruction(rhs);

					lastTagName = rhs.value;

					break;

				case '/':

					// At the end of a self-closing tag (e.g. "<tag/>").
					// We'll treat this as if there was a separate closing tag.

					rhs.type = XmlLexerTokenType::CloseTag;
					rhs.value = lastTagName;

					break;

				case '>':

					// Reached the end of a tag (that had attributes).

					insideTag = false;

					ReadNextToken(rhs);

					break;

				case '?':

					// Reached the end of a processor instruction.

					ReadNextChar(c, true); // Consume ">"

					rhs.type = XmlLexerTokenType::CloseProcessorInstruction;

					break;

				case '"':

					// Reached an attribute value.

					ReadNextAttributeValue(rhs);

					break;

				default:

					rhs.value.push_back(c);

					// Reached either text or an attribute name.
					// We'll also end up here for text outside of tags.

					if (insideTag)
						ReadNextAttributeName(rhs);
					else {

						ReadNextText(rhs);

						// If we read text outside of tags, this isn't a valid text token.

						if (!betweenTags)
							rhs.type = XmlLexerTokenType::Undefined;

					}

					break;

				}

			}
			else {

				// There were no more characters to read.

				rhs.type = XmlLexerTokenType::Eof;

			}

			if (rhs.type == XmlLexerTokenType::OpenTag)
				betweenTags = true;
			else if (rhs.type == XmlLexerTokenType::CloseTag)
				betweenTags = false;

		}
		bool XmlLexer::ReadNextChar(char& c, bool skipws) {

			while (stream >> std::noskipws >> c)
				if (!skipws || !std::isspace(c))
					return true;

			return false;

		}
		bool XmlLexer::PeekNextChar(char& out) {

			bool good = stream.good();

			if (good)
				out = stream.peek();

			return good;

		}
		void XmlLexer::ReadNextTagOrProcessorInstruction(XmlLexerToken& rhs) {

			char c;

			// Skip whitespace following the symbol.

			ReadNextChar(c, true);

			// Check if the tag is closed right way (no tag name).

			if (c == '>') {

				rhs.type = XmlLexerTokenType::OpenTag;

				return;

			}

			// Determine whether the tag is an open or close tag.

			if (c == '/') {

				rhs.type = XmlLexerTokenType::CloseTag;

				// Skip whitespace following the symbol.

				ReadNextChar(c, true);

				// Check if the tag is closed right away (i.e. "</>").

				if (c == '>')
					return;
			}
			else if (c == '?') {

				rhs.type = XmlLexerTokenType::OpenProcessorInstruction;

				ReadNextChar(c, true);

			}
			else
				rhs.type = XmlLexerTokenType::OpenTag;

			insideTag = true;

			rhs.value.push_back(c);

			// Read the tag name.

			while (PeekNextChar(c) && c != '/') {

				ReadNextChar(c);

				if (std::isspace(c))
					break;

				else if (c == '>') {

					// The tag ended right away (does not have any attributes) (e.g. "<tag>").

					insideTag = false;

					break;

				}

				rhs.value.push_back(c);

			}

		}
		void XmlLexer::ReadNextAttributeValue(XmlLexerToken& rhs) {

			rhs.type = XmlLexerTokenType::AttributeValue;

			char c;

			// Read until we reach the closing quote.
			while (ReadNextChar(c)) {
				if (c == '"')
					break;
				rhs.value.push_back(c);
			}

		}
		void XmlLexer::ReadNextText(XmlLexerToken& rhs) {

			rhs.type = XmlLexerTokenType::Text;

			char c;

			// Read until we reach the end of the text.

			while (PeekNextChar(c) && c != '<') {

				ReadNextChar(c);

				rhs.value.push_back(c);

			}

			// Remove trailing whitespace.
			while (std::isspace(rhs.value.back()))
				rhs.value.pop_back();

		}
		void XmlLexer::ReadNextAttributeName(XmlLexerToken& rhs) {

			rhs.type = XmlLexerTokenType::AttributeName;

			char c;

			// Read until we reach the end of the attribute name.
			while (ReadNextChar(c, true)) {
				if (c == '=')
					break;
				rhs.value.push_back(c);
			}

		}

	}
}