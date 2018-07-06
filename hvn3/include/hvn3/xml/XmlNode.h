#pragma once
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace hvn3 {
	namespace Xml {

		class XmlNode {

			typedef std::unordered_map<std::string, std::string> attributes_collection_type;
			typedef std::vector<std::unique_ptr<XmlNode>> child_collection_type;

		public:
			XmlNode(const std::string& tag);

			const std::string& Tag() const;
			void SetTag(const std::string& value);
			const std::string& Text() const;
			void SetText(const std::string& value);
			bool HasText() const;
			const std::string& GetAttribute(const std::string& attribute) const;
			template<typename T>
			void SetAttribute(const std::string& attribute, const T& value) {

				std::stringstream buf;
				buf << value;

				_attributes[attribute] = buf.str();
			}
			template <>
			void SetAttribute(const std::string& attribute, const std::string& value) {
				_attributes[attribute] = value;
			}
			template<std::size_t N>
			void SetAttribute(const std::string& attribute, const char(&value)[N]) {
				_attributes[attribute] = value;
			}
			bool HasAttribute(const std::string& attribute) const;
			attributes_collection_type::iterator AttributesBegin();
			attributes_collection_type::iterator AttributesEnd();
			attributes_collection_type::const_iterator AttributesBegin() const;
			attributes_collection_type::const_iterator AttributesEnd() const;
			
			XmlNode* AddChild(const std::string& tag);
			bool HasChildren() const;
			child_collection_type::iterator ChildrenBegin();
			child_collection_type::iterator ChildrenEnd();
			child_collection_type::const_iterator ChildrenBegin() const;
			child_collection_type::const_iterator ChildrenEnd() const;

			std::string& operator[](const std::string& attribute);
			const std::string& operator[](const std::string& attribute) const;

		private:
			std::string _tag;
			std::string _text;
			attributes_collection_type _attributes;
			std::vector<std::unique_ptr<XmlNode>> _child_nodes;

		};

	}
}