#include "hvn3/xml/XmlElement.h"

#include "hvn3/utility/StringUtils.h"
#include "hvn3/xml/XmlException.h"

#include <cassert>

namespace hvn3 {
	namespace Xml {

		// Public members

		XmlElement::XmlElement() {}
		XmlElement::XmlElement(const std::string& tag) :
			_tag(tag) {
		}

		const std::string& XmlElement::Tag() const {
			return _tag;
		}
		void XmlElement::SetTag(const std::string& value) {
			_tag = value;
		}
		const std::string& XmlElement::Text() const {
			return _text;
		}
		void XmlElement::SetText(const std::string& value) {
			_text = value;
		}
		bool XmlElement::HasText() const {
			return _text.length() > 0;
		}

		const std::string& XmlElement::GetAttribute(const std::string& attribute) const {

			auto iter = _attributes.find(attribute);

			if (iter == _attributes.end())
				throw XmlException(StringUtils::Format("The attribute \"{0}\" was not found.", attribute));

			return iter->second;

		}
		bool XmlElement::TryGetAttribute(const std::string& attribute, std::string& out) const {

			auto iter = _attributes.find(attribute);

			if (iter == _attributes.end())
				return false;

			out = iter->second;

			return false;

		}
		bool XmlElement::HasAttribute(const std::string& attribute) const {
			return _attributes.count(attribute) > 0;
		}
		bool XmlElement::HasAttributes() const {
			return _attributes.size() > 0;
		}

		const XmlElement::attributes_collection_type& XmlElement::Attributes() const {
			return _attributes;
		}

		XmlElement& XmlElement::AddChild(const std::string& tag) {

			_childNodes.push_back(std::move(std::make_unique<XmlElement>(tag)));

			return *_childNodes.back().get();

		}
		XmlElement& XmlElement::GetChild(const std::string& tag) {

			for (auto i = _childNodes.begin(); i != _childNodes.end(); ++i)
				if (i->get()->Tag() == tag)
					return *i->get();

			throw XmlException(StringUtils::Format("No child node exists with tag \"{0}\".", tag));

		}
		bool XmlElement::GetChild(const std::string& tag, XmlElement*& result) {

			result = nullptr;

			for (auto i = _childNodes.begin(); i != _childNodes.end(); ++i)
				if (i->get()->Tag() == tag) {

					result = i->get();

					break;

				}

			return result != nullptr;

		}
		const XmlElement& XmlElement::GetChild(const std::string& tag) const {

			for (auto i = _childNodes.begin(); i != _childNodes.end(); ++i)
				if (i->get()->Tag() == tag)
					return *i->get();

			throw XmlException(StringUtils::Format("No child node exists with tag \"{0}\".", tag));

		}
		bool XmlElement::GetChild(const std::string& tag, const XmlElement*& result) const {

			result = nullptr;

			for (auto i = _childNodes.begin(); i != _childNodes.end(); ++i)
				if (i->get()->Tag() == tag) {

					result = i->get();

					break;

				}

			return result != nullptr;

		}
		bool XmlElement::HasChildren() const {
			return _childNodes.size() > 0;
		}

		const XmlElement::child_collection_type& XmlElement::Children() const {
			return _childNodes;
		}

		std::string& XmlElement::operator[](const std::string& attribute) {
			return _attributes[attribute];
		}
		const std::string& XmlElement::operator[](const std::string& attribute) const {
			return GetAttribute(attribute);
		}

	}
}