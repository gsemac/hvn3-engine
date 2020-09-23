#pragma once

#include "hvn3/xml/xml_lexer_token.h"

#include <cctype>
#include <iostream>
#include <string>

namespace hvn3 {
	namespace xml {

		class XmlLexer {

		public:
			XmlLexer(std::istream& stream);

			bool operator>>(XmlLexerToken& rhs);

		private:
			std::istream& stream;
			bool insideTag;
			bool betweenTags;
			std::string lastTagName;

			void ReadNextToken(XmlLexerToken& rhs);
			bool ReadNextChar(char& c, bool skipws = false);
			bool PeekNextChar(char& out);
			void ReadNextTagOrProcessorInstruction(XmlLexerToken& rhs);
			void ReadNextAttributeValue(XmlLexerToken& rhs);
			void ReadNextText(XmlLexerToken& rhs);
			void ReadNextAttributeName(XmlLexerToken& rhs);

		};

	}
}