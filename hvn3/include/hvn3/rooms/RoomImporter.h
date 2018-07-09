#pragma once
#include "hvn3/objects/IObject.h"
#include "hvn3/xml/XmlDocument.h"
#include <functional>
#include <string>

namespace hvn3 {

	class RoomImporter {

	public:
		typedef std::function<IObject*(const Xml::XmlElement*)> object_import_callback_type;
		
		RoomImporter() {}

		void SetObjectExportCallback(object_import_callback_type&& callback) {
			_object_import_callback = std::move(callback);
		}
		bool Load(const IRoom& room, const std::string& file_path) const {

			Xml::XmlDocument doc = Xml::XmlDocument::Open(file_path);

			for (auto i = doc.Root().ChildrenBegin(); i != doc.Root().ChildrenEnd(); ++i) {


			}

			return true;

		}

	private:
		object_import_callback_type _object_import_callback;

		IObject* _importObject(const Xml::XmlElement* node) const {

			if (_object_import_callback)
				return _object_import_callback(node);
			else {
				// #todo Return a default object?
				return nullptr;
			}

		}

	};

}