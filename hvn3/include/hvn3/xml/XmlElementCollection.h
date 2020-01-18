#pragma once

#include "hvn3/xml/XmlElement.h"

#include <string>
#include <vector>

namespace hvn3 {
	namespace Xml {

		class XmlElementCollection {

			using container_type = std::vector<std::shared_ptr<XmlElement>>;

		public:
			template<typename IteratorType>
			class XmlElementCollectionIterator {

			public:
				XmlElementCollectionIterator(IteratorType begin, IteratorType end) {

					_iter = iterator;

				}

				XmlElementCollectionIterator& operator++() {

					++_iter;

					return *this;

				}
				XmlElementCollectionIterator& operator++(int) {

					XmlElementCollectionIterator copy = *this;

					++_iter;

					return copy;

				}

				XmlElement& operator*() {

					return *_iter;

				}
				XmlElement* operator->() {

					return &(*_iter);

				}

			private:
				IteratorType _iter;

			};

			class XmlElementCollectionView {};

			XmlElementCollection();

			void Add(const std::string& tag);

			XmlElement& GetElementByTag(const std::string& tag);
			XmlElementCollectionView GetElementsByTag(const std::string& tag);

			XmlElement& operator[](const std::string& tag);
			const XmlElement& operator[](const std::string& tag) const;

		private:
			container_type _items;

		};

	}
}