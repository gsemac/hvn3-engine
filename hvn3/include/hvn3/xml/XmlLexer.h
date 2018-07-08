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
				_inside_tag(false) {
			}

			bool operator>>(XmlLexerToken& rhs) {

				_readNextToken(rhs);

				return rhs.type != XmlLexerTokenType::Eof;

			}

		private:
			std::istream& _stream;
			bool _inside_tag;

			void _readNextToken(XmlLexerToken& rhs) {
				
				rhs.type = XmlLexerTokenType::Undefined;
				rhs.value.clear();

				char c;

				if (!_readNextChar(c, true)) {
					rhs.type = XmlLexerTokenType::Eof;
					return;
				}

				switch (c) {

				case '<':
					// Reached either a tag (open or closed) or processor instruction.
					_readNextTagOrProcessorInstruction(rhs);
					break;
				case '>':
					// Reached the end of a tag (that had attributes).
					_inside_tag = false;
					_readNextToken(rhs);
					return;
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
					if (!_inside_tag)
						_readNextText(rhs);
					else
						_readNextAttributeName(rhs);
					break;

				}

			}
			bool _readNextChar(char& c, bool skipws = false) {

				while (_stream >> std::noskipws >> c)
					if (!skipws || !std::isspace(c))
						return true;

				return false;

			}
			char _peekNextChar() {
				return _stream.peek();
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

				// Determine whether the tag is opened or closed.
				if (c == '/') {
					rhs.type = XmlLexerTokenType::CloseTag;
					// Skip whitespace following the symbol.
					_readNextChar(c, true);
					// Check if the tag is closed right away (no tag name).
					if (c == '>')
						return;
				}
				else if (c == '?') {
					rhs.type = XmlLexerTokenType::OpenProcessorInstruction;
					_readNextChar(c, true);
				}
				else
					rhs.type = XmlLexerTokenType::OpenTag;
								
				_inside_tag = true;
				rhs.value.push_back(c);

				// Read until we reach the end of the tag name.
				while (_readNextChar(c)) {
					if (std::isspace(c))
						break;
					else if (c == '>') {
						// The tag ended right away (does not have any attributes).
						_inside_tag = false;
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
				while (_peekNextChar() != '<') {
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