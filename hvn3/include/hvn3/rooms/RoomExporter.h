#pragma once
#include "hvn3/objects/IObject.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/tilesets/TileManager.h"
#include "hvn3/xml/XmlDocument.h"
#include <functional>
#include <string>
#include <sstream>

namespace hvn3 {

	class RoomExporter {

	public:
		typedef std::function<void(const IObject*, Xml::XmlElement*)> object_export_callback_type;

		RoomExporter() {}

		void SetObjectExportCallback(object_export_callback_type&& callback) {
			_object_export_callback = std::move(callback);
		}
		bool Save(const IRoom& room, const std::string& file_path) const {

			std::stringstream buf;

			Xml::XmlDocument writer("map");
			writer.Root().SetAttribute("version", "1.0");

			// Write tiles.
			if (room.Tiles().LayerCount() > 0) {

				Xml::XmlElement* tiles_node = writer.Root().AddChild("tiles");
				tiles_node->SetAttribute("layers", room.Tiles().LayerCount());

				// Write each layer as a separate node.
				for (auto i = room.Tiles().LayersBegin(); i != room.Tiles().LayersEnd(); ++i) {
					Xml::XmlElement* layer_node = writer.Root().AddChild("layer");
					layer_node->SetAttribute("depth", i->first);
					for (int j = 0; j < room.Tiles().Count(); ++j)
						buf << room.Tiles().AtIndex(j, i->first).id << ',';
					layer_node->SetText(buf.str());
					buf.clear();
				}

			}

			// Write objects.
			if (room.Objects().Count() > 0) {

				Xml::XmlElement* objects_node = writer.Root().AddChild("objects");

				room.Objects().ForEach([&](const IObject* obj) {
					Xml::XmlElement* object_node = objects_node->AddChild("object");
					_exportObject(obj, object_node);
				});

			}

			return writer.Save(file_path);

		}

	private:
		object_export_callback_type _object_export_callback;

		void _exportObject(const IObject* object, Xml::XmlElement* node) const {

			if (_object_export_callback)
				_object_export_callback(object, node);
			else {
				node->SetAttribute("id", object->Id());
				node->SetAttribute("x", object->X());
				node->SetAttribute("y", object->Y());
			}

		}

	};

}