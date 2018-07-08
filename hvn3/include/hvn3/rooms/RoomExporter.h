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
		typedef std::function<void(const IObject*, Xml::XmlElement*)> object_exporter_callback_type;

		RoomExporter(const IRoom& room) :
			_room(&room) {
		}

		void SetObjectExporterCallback(object_exporter_callback_type&& callback) {
			_object_exporter_callback = std::move(callback);
		}
		bool Save(const std::string& file_path) const {

			std::stringstream buf;

			Xml::XmlDocument writer("map");
			writer.Root().SetAttribute("version", "1.0");

			// Write tiles.
			if (_room->Tiles().LayerCount() > 0) {

				Xml::XmlElement* tiles_node = writer.Root().AddChild("tiles");
				tiles_node->SetAttribute("layers", _room->Tiles().LayerCount());

				// Write each layer as a separate node.
				for (auto i = _room->Tiles().LayersBegin(); i != _room->Tiles().LayersEnd(); ++i) {
					Xml::XmlElement* layer_node = writer.Root().AddChild("layer");
					layer_node->SetAttribute("depth", i->first);
					for (int j = 0; j < _room->Tiles().Count(); ++j)
						buf << _room->Tiles().AtIndex(j, i->first).id << ',';
					layer_node->SetText(buf.str());
					buf.clear();
				}

			}

			// Write objects.
			if (_room->Objects().Count() > 0) {

				Xml::XmlElement* objects_node = writer.Root().AddChild("objects");

				_room->Objects().ForEach([&](const IObject* obj) {
					Xml::XmlElement* object_node = objects_node->AddChild("object");
					_exportObject(obj, object_node);
				});

			}

			return writer.Save(file_path);

		}

	private:
		const IRoom* _room;
		object_exporter_callback_type _object_exporter_callback;

		void _exportObject(const IObject* object, Xml::XmlElement* node) const {

			if (_object_exporter_callback)
				_object_exporter_callback(object, node);
			else {
				node->SetAttribute("id", object->Id());
				node->SetAttribute("x", object->X());
				node->SetAttribute("y", object->Y());
			}

		}

	};

}