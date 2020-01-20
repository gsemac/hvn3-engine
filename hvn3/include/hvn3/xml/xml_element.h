#pragma once

#include "hvn3/iterators/iterator_range.h"
#include "hvn3/iterators/stashing_transform_iterator.h"
#include "hvn3/utility/Convert.h"
#include "hvn3/xml/xml_attribute.h"

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace hvn3 {
	namespace xml {

		class XmlElement {

			using string_type = std::string;
			using attribute_container_type = std::unordered_map<string_type, string_type>;
			using child_container_type = std::vector<std::shared_ptr<XmlElement>>;

		public:
			XmlElement();
			explicit XmlElement(const string_type& tag);

			const string_type& Tag() const;
			void SetTag(const string_type& value);

			const string_type& Text() const;
			void SetText(const string_type& value);
			bool HasText() const;

			const string_type& GetAttribute(const string_type& name) const;
			bool GetAttribute(const string_type& name, string_type& value) const;

			template<typename OutputType>
			OutputType GetAttribute(const string_type& name) const;
			template<typename OutputType>
			bool GetAttribute(const string_type& name, OutputType& value) const;

			template<typename InputType>
			void SetAttribute(const string_type& name, const InputType& value);
			template <>
			void SetAttribute(const string_type& name, const string_type& value);
			template<std::size_t N>
			void SetAttribute(const string_type& name, const char(&value)[N]);

			bool HasAttribute(const string_type& name) const;
			bool HasAttributes() const;

			XmlElement& AddChild(const string_type& tag);
			XmlElement& GetChild(const string_type& tag);
			const XmlElement& GetChild(const string_type& tag) const;
			bool HasChildren() const;

			string_type& operator[](const string_type& attribute);
			const string_type& operator[](const string_type& attribute) const;

		private:
			string_type tag;
			string_type text;
			attribute_container_type attributes;
			child_container_type childNodes;

			static XmlAttribute AttributeTransformFunc(typename attribute_container_type::value_type& input);
			static XmlAttribute ConstAttributeTransformFunc(typename const attribute_container_type::value_type& input);
			static XmlElement& ChildTransformFunc(typename child_container_type::value_type& input);
			static const XmlElement& ConstChildTransformFunc(const typename child_container_type::value_type& input);

		public:
			using attribute_iterator = decltype(make_stashing_transform_iterator(attributes.begin(), AttributeTransformFunc));
			using const_attribute_iterator = decltype(make_stashing_transform_iterator(attributes.cbegin(), ConstAttributeTransformFunc));
			using child_iterator = decltype(make_transform_iterator(childNodes.begin(), ChildTransformFunc));
			using const_child_iterator = decltype(make_transform_iterator(childNodes.cbegin(), ConstChildTransformFunc));

			IteratorRange<attribute_iterator> Attributes();
			IteratorRange<const_attribute_iterator> Attributes() const;
			IteratorRange<child_iterator> Children();
			IteratorRange<const_child_iterator> Children() const;

		};

		template<typename OutputType>
		OutputType XmlElement::GetAttribute(const string_type& attribute) const {

			return Convert::To<OutputType>(GetAttribute(attribute));

		}
		template<typename OutputType>
		bool XmlElement::GetAttribute(const string_type& attribute, OutputType& out) const {

			auto iter = attributes.find(attribute);

			if (iter == attributes.end())
				return false;


			out = Convert::To<OutputType>(iter->second);

			return true;

		}

		template<typename InputType>
		void XmlElement::SetAttribute(const string_type& name, const InputType& value) {

			attributes[attribute] = Convert::ToString(value);

		}
		template <>
		void XmlElement::SetAttribute(const string_type& name, const string_type& value) {

			attributes[name] = value;

		}
		template<std::size_t N>
		void XmlElement::SetAttribute(const string_type& name, const char(&value)[N]) {

			SetAttribute(attribute, string_type(value));

		}


	}
}