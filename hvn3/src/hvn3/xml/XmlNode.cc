#include "hvn3/exceptions/Exception.h"
#include "hvn3/xml/XmlNode.h"
#include <cassert>

namespace hvn3 {
	namespace Xml {

		XmlNode::XmlNode(const std::string& tag) :
			_tag(tag) {
		}
		const std::string& XmlNode::Tag() const {
			return _tag;
		}
		void XmlNode::SetTag(const std::string& value) {
			_tag = value;
		}
		const std::string& XmlNode::Text() const {
			return _text;
		}
		void XmlNode::SetText(const std::string& value) {
			_text = value;
		}
		bool XmlNode::HasText() const {
			return _text.length() > 0;
		}
		const std::string& XmlNode::GetAttribute(const std::string& attribute) const {

			auto iter = _attributes.find(attribute);

			assert(iter != _attributes.end());

			return iter->first;

		}
		bool XmlNode::HasAttribute(const std::string& attribute) const {
			return _attributes.count(attribute) > 0;
		}
		XmlNode::attributes_collection_type::iterator XmlNode::AttributesBegin() {
			return _attributes.begin();
		}
		XmlNode::attributes_collection_type::iterator XmlNode::AttributesEnd() {
			return _attributes.end();
		}
		XmlNode::attributes_collection_type::const_iterator XmlNode::AttributesBegin() const {
			return _attributes.begin();
		}
		XmlNode::attributes_collection_type::const_iterator XmlNode::AttributesEnd() const {
			return _attributes.end();
		}
		XmlNode* XmlNode::AddChild(const std::string& tag) {

			_child_nodes.push_back(std::move(std::make_unique<XmlNode>(tag)));

			return _child_nodes.back().get();

		}
		bool XmlNode::HasChildren() const {
			return _child_nodes.size() > 0;
		}
		XmlNode::child_collection_type::iterator XmlNode::ChildrenBegin() {
			return _child_nodes.begin();
		}
		XmlNode::child_collection_type::iterator XmlNode::ChildrenEnd() {
			return _child_nodes.end();
		}
		XmlNode::child_collection_type::const_iterator XmlNode::ChildrenBegin() const {
			return _child_nodes.begin();
		}
		XmlNode::child_collection_type::const_iterator XmlNode::ChildrenEnd() const {
			return _child_nodes.end();
		}
		std::string& XmlNode::operator[](const std::string& attribute) {
			return _attributes[attribute];
		}
		const std::string& XmlNode::operator[](const std::string& attribute) const {
			return GetAttribute(attribute);
		}

	}
}