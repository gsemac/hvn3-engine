#pragma once
#include <cctype>
#include <iostream>
#include <string>

namespace hvn3 {
	namespace Xml {

		enum class XmlLexerTokenType {
			OpenProcessorInstruction,
			CloseProcessorInstruction,
			OpenTag,
			CloseTag,
			AttributeName,
			AttributeValue,
			Text,
			Eof,
			Undefined
		};

		struct XmlLexerToken {
			XmlLexerTokenType type;
			std::string value;
		};

		class XmlLexer {

		public:
			XmlLexer(std::istream& stream) :
				_stream(stream),
				_insideTag(false),
				_betweenTags(false) {
			}

			bool operator>>(XmlLexerToken& rhs) {

				_readNextToken(rhs);

				return rhs.type != XmlLexerTokenType::Eof;

			}

		private:
			std::istream& _stream;
			bool _insideTag;
			bool _betweenTags;
			std::string _lastTagName;

			void _readNextToken(XmlLexerToken& rhs) {

				rhs.type = XmlLexerTokenType::Undefined;
				rhs.value.clear();

				char c;

				if (_readNextChar(c, true)) {

					switch (c) {

					case '<':

						// Reached either a tag (open or closed) or processor instruction.

						_readNextTagOrProcessorInstruction(rhs);

						_lastTagName = rhs.value;

						break;

					case '/':

						// At the end of a self-closing tag (e.g. "<tag/>").
						// We'll treat this as if there was a separate closing tag.

						rhs.type = XmlLexerTokenType::CloseTag;
						rhs.value = _lastTagName;

						break;

					case '>':

						// Reached the end of a tag (that had attributes).

						_insideTag = false;

						_readNextToken(rhs);

						break;

					case '?':

						// Reached the end of a processor instruction.

						_readNextChar(c, true); // Consume ">"

						rhs.type = XmlLexerTokenType::CloseProcessorInstruction;

						break;

					case '"':

						// Reached an attribute value.

						_readNextAttributeValue(rhs);

						break;

					default:

						rhs.value.push_back(c);

						// Reached either text or an attribute name.
						// We'll also end up here for text outside of tags.

						if (_insideTag)
							_readNextAttributeName(rhs);
						else {

							_readNextText(rhs);

							// If we read text outside of tags, this isn't a valid text token.

							if (!_betweenTags)
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
					_betweenTags = true;
				else if (rhs.type == XmlLexerTokenType::CloseTag)
					_betweenTags = false;

			}
			bool _readNextChar(char& c, bool skipws = false) {

				while (_stream >> std::noskipws >> c)
					if (!skipws || !std::isspace(c))
						return true;

				return false;

			}
			bool _peekNextChar(char& out) {

				bool good = _stream.good();

				if (good)
					out = _stream.peek();

				return good;

			}
			void _readNextTagOrProcessorInstruction(XmlLexerToken& rhs) {

				char c;

				// Skip whitespace following the symbol.

				_readNextChar(c, true);

				// Check if the tag is closed right way (no tag name).

				if (c == '>') {

					rhs.type = XmlLexerTokenType::OpenTag;

					return;

				}

				// Determine whether the tag is an open or close tag.

				if (c == '/') {

					rhs.type = XmlLexerTokenType::CloseTag;

					// Skip whitespace following the symbol.

					_readNextChar(c, true);

					// Check if the tag is closed right away (i.e. "</>").

					if (c == '>')
						return;
				}
				else if (c == '?') {

					rhs.type = XmlLexerTokenType::OpenProcessorInstruction;

					_readNextChar(c, true);

				}
				else
					rhs.type = XmlLexerTokenType::OpenTag;

				_insideTag = true;

				rhs.value.push_back(c);

				// Read the tag name.

				while (_peekNextChar(c) && c != '/') {

					_readNextChar(c);

					if (std::isspace(c))
						break;

					else if (c == '>') {

						// The tag ended right away (does not have any attributes) (e.g. "<tag>").

						_insideTag = false;

						break;

					}

					rhs.value.push_back(c);

				}

			}
			void _readNextAttributeValue(XmlLexerToken& rhs) {

				rhs.type = XmlLexerTokenType::AttributeValue;

				char c;

				// Read until we reach the closing quote.
				while (_readNextChar(c)) {
					if (c == '"')
						break;
					rhs.value.push_back(c);
				}

			}
			void _readNextText(XmlLexerToken& rhs) {

				rhs.type = XmlLexerTokenType::Text;

				char c;

				// Read until we reach the end of the text.

				while (_peekNextChar(c) && c != '<') {

					_readNextChar(c);

					rhs.value.push_back(c);

				}

				// Remove trailing whitespace.
				while (std::isspace(rhs.value.back()))
					rhs.value.pop_back();

			}
			void _readNextAttributeName(XmlLexerToken& rhs) {

				rhs.type = XmlLexerTokenType::AttributeName;

				char c;

				// Read until we reach the end of the attribute name.
				while (_readNextChar(c, true)) {
					if (c == '=')
						break;
					rhs.value.push_back(c);
				}

			}

		};

	}
}