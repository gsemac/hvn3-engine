#include "hvn3/xml/XmlElementCollection.h"

#include "hvn3/exceptions/Exception.h"

namespace hvn3 {
	namespace Xml {

		XmlElementCollection::XmlElementCollection() {



		}

		void XmlElementCollection::Add(const std::string& tag) {

			throw NotImplementedException();

		}

		XmlElement& XmlElementCollection::GetElementByTag(const std::string& tag) {

			throw NotImplementedException();

		}
		XmlElementCollection::XmlElementCollectionView XmlElementCollection::GetElementsByTag(const std::string& tag) {

			throw NotImplementedException();

		}

		XmlElement& XmlElementCollection::operator[](const std::string& tag) {

			throw NotImplementedException();

		}
		const XmlElement& XmlElementCollection::operator[](const std::string& tag) const {


			throw NotImplementedException();

		}

	}
}