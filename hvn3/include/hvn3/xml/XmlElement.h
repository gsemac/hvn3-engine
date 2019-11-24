#pragma once

#include "hvn3/utility/Convert.h"

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace hvn3 {
	namespace Xml {

		class XmlElement {

			typedef std::unordered_map<std::string, std::string> attributes_collection_type;
			typedef std::vector<std::shared_ptr<XmlElement>> child_collection_type;

		public:
			typedef child_collection_type::iterator child_iterator;
			typedef child_collection_type::const_iterator const_child_iterator;
			typedef attributes_collection_type::iterator attribute_iterator;
			typedef attributes_collection_type::const_iterator const_attribute_iterator;

			XmlElement();
			explicit XmlElement(const std::string& tag);

			const std::string& Tag() const;
			void SetTag(const std::string& value);
			const std::string& Text() const;
			void SetText(const std::string& value);
			bool HasText() const;

			const std::string& GetAttribute(const std::string& attribute) const;
			template<typename T>
			T GetAttribute(const std::string& attribute) const {

				return Convert::To<T>(GetAttribute(attribute));

			}
			bool TryGetAttribute(const std::string& attribute, std::string& out) const;
			template<typename T>
			bool TryGetAttribute(const std::string& attribute, T& out) const {

				auto iter = _attributes.find(attribute);

				if (iter == _attributes.end())
					return false;


				out = Convert::To<T>(iter->second);

				return true;

			}

			template<typename T>
			void SetAttribute(const std::string& attribute, const T& value) {

				_attributes[attribute] = Convert::ToString(value);

			}
			template <>
			void SetAttribute(const std::string& attribute, const std::string& value) {

				_attributes[attribute] = value;

			}
			template<std::size_t N>
			void SetAttribute(const std::string& attribute, const char(&value)[N]) {

				SetAttribute(attribute, std::string(value));

			}

			bool HasAttribute(const std::string& attribute) const;
			bool HasAttributes() const;

			const attributes_collection_type& Attributes() const;

			XmlElement& AddChild(const std::string& tag);
			XmlElement& GetChild(const std::string& tag);
			bool GetChild(const std::string& tag, XmlElement*& result);
			const XmlElement& GetChild(const std::string& tag) const;
			bool GetChild(const std::string& tag, const XmlElement*& result) const;

			bool HasChildren() const;

			const child_collection_type& Children() const;

			std::string& operator[](const std::string& attribute);
			const std::string& operator[](const std::string& attribute) const;

		private:
			std::string _tag;
			std::string _text;
			attributes_collection_type _attributes;
			child_collection_type _childNodes;

		};

	}
}