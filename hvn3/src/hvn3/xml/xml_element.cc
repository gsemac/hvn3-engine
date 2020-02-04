#include "hvn3/xml/xml_element.h"

#include "hvn3/utility/StringUtils.h"
#include "hvn3/xml/xml_exception.h"

#include <cassert>

namespace hvn3 {
	namespace xml {

		// Public members

		XmlElement::XmlElement() {}
		XmlElement::XmlElement(const string_type& tag) :
			tag(tag) {
		}

		const XmlElement::string_type& XmlElement::Tag() const {
			return tag;
		}
		void XmlElement::SetTag(const string_type& value) {
			tag = value;
		}
		const XmlElement::string_type& XmlElement::Text() const {
			return text;
		}
		void XmlElement::SetText(const string_type& value) {
			text = value;
		}
		bool XmlElement::HasText() const {
			return text.length() > 0;
		}

		const XmlElement::string_type& XmlElement::GetAttribute(const string_type& attribute) const {

			auto iter = attributes.find(attribute);

			if (iter == attributes.end())
				throw XmlException(StringUtils::Format("The attribute \"{0}\" was not found.", attribute));

			return iter->second;

		}
		XmlElement::string_type XmlElement::GetAttribute(const string_type& attribute, const string_type& defaultValue) const {

			auto iter = attributes.find(attribute);

			if (iter == attributes.end())
				return defaultValue;

			return iter->second;

		}
		bool XmlElement::HasAttribute(const string_type& attribute) const {
			return attributes.count(attribute) > 0;
		}
		bool XmlElement::HasAttributes() const {
			return attributes.size() > 0;
		}

		XmlElement& XmlElement::AddChild(const string_type& tag) {

			childNodes.push_back(std::move(std::make_unique<XmlElement>(tag)));

			return *childNodes.back().get();

		}
		XmlElement& XmlElement::GetChild(const string_type& tag) {

			for (auto i = childNodes.begin(); i != childNodes.end(); ++i)
				if (i->get()->Tag() == tag)
					return *i->get();

			throw XmlException(StringUtils::Format("No child node exists with tag \"{0}\".", tag));

		}
		const XmlElement& XmlElement::GetChild(const string_type& tag) const {

			for (auto i = childNodes.begin(); i != childNodes.end(); ++i)
				if (i->get()->Tag() == tag)
					return *i->get();

			throw XmlException(StringUtils::Format("No child node exists with tag \"{0}\".", tag));

		}
		bool XmlElement::HasChildren() const {
			return childNodes.size() > 0;
		}

		XmlElement::string_type& XmlElement::operator[](const string_type& attribute) {
			return attributes[attribute];
		}
		const XmlElement::string_type& XmlElement::operator[](const string_type& attribute) const {
			return GetAttribute(attribute);
		}

		IterableRange<XmlElement::attribute_iterator> XmlElement::Attributes() {

			auto begin = make_stashing_transform_iterator(attributes.begin(), AttributeTransformFunc);
			auto end = make_stashing_transform_iterator(attributes.end(), AttributeTransformFunc);

			return make_iterable_range(begin, end);

		}
		IterableRange<XmlElement::const_attribute_iterator> XmlElement::Attributes() const {

			auto begin = make_stashing_transform_iterator(attributes.cbegin(), ConstAttributeTransformFunc);
			auto end = make_stashing_transform_iterator(attributes.cend(), ConstAttributeTransformFunc);

			return make_iterable_range(begin, end);

		}
		IterableRange<XmlElement::child_iterator> XmlElement::Children() {

			auto begin = make_transform_iterator(childNodes.begin(), ChildTransformFunc);
			auto end = make_transform_iterator(childNodes.end(), ChildTransformFunc);

			return make_iterable_range(begin, end);

		}
		IterableRange<XmlElement::const_child_iterator> XmlElement::Children() const {

			auto begin = make_transform_iterator(childNodes.cbegin(), ConstChildTransformFunc);
			auto end = make_transform_iterator(childNodes.cend(), ConstChildTransformFunc);

			return make_iterable_range(begin, end);

		}

		// Private members

		XmlAttribute XmlElement::AttributeTransformFunc(typename attribute_container_type::value_type& input) {

			return XmlAttribute(&input.first, &input.second);

		}
		XmlAttribute XmlElement::ConstAttributeTransformFunc(typename const attribute_container_type::value_type& input) {

			return XmlAttribute(&input.first, &input.second);

		}
		XmlElement& XmlElement::ChildTransformFunc(typename child_container_type::value_type& input) {

			return *input;

		}
		const XmlElement& XmlElement::ConstChildTransformFunc(const typename child_container_type::value_type& input) {

			return *input;

		}

	}
}