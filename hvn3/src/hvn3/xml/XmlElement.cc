#include "hvn3/exceptions/Exception.h"
#include "hvn3/xml/XmlElement.h"
#include <cassert>

namespace hvn3 {
	namespace Xml {

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

			assert(iter != _attributes.end());

			return iter->first;

		}
		bool XmlElement::HasAttribute(const std::string& attribute) const {
			return _attributes.count(attribute) > 0;
		}
		XmlElement::attributes_collection_type::iterator XmlElement::AttributesBegin() {
			return _attributes.begin();
		}
		XmlElement::attributes_collection_type::iterator XmlElement::AttributesEnd() {
			return _attributes.end();
		}
		XmlElement::attributes_collection_type::const_iterator XmlElement::AttributesBegin() const {
			return _attributes.begin();
		}
		XmlElement::attributes_collection_type::const_iterator XmlElement::AttributesEnd() const {
			return _attributes.end();
		}
		XmlElement* XmlElement::AddChild(const std::string& tag) {

			_child_nodes.push_back(std::move(std::make_unique<XmlElement>(tag)));

			return _child_nodes.back().get();

		}
		bool XmlElement::HasChildren() const {
			return _child_nodes.size() > 0;
		}
		XmlElement::child_collection_type::iterator XmlElement::ChildrenBegin() {
			return _child_nodes.begin();
		}
		XmlElement::child_collection_type::iterator XmlElement::ChildrenEnd() {
			return _child_nodes.end();
		}
		XmlElement::child_collection_type::const_iterator XmlElement::ChildrenBegin() const {
			return _child_nodes.begin();
		}
		XmlElement::child_collection_type::const_iterator XmlElement::ChildrenEnd() const {
			return _child_nodes.end();
		}
		std::string& XmlElement::operator[](const std::string& attribute) {
			return _attributes[attribute];
		}
		const std::string& XmlElement::operator[](const std::string& attribute) const {
			return GetAttribute(attribute);
		}

	}
}