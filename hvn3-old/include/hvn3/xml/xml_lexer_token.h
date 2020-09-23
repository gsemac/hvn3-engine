#pragma once

#include <string>

namespace hvn3 {
	namespace xml {

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

			XmlLexerToken();

			XmlLexerTokenType type;
			std::string value;

		};

	}
}