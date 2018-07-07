#pragma once
#include "hvn3/objects/IObject.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/tilesets/TileManager.h"
#include "hvn3/xml/XmlWriter.h"
#include <string>
#include <sstream>

namespace hvn3 {

	class RoomWriter {

	public:
		RoomWriter(const IRoom& room) :
			_room(&room) {
		}

		bool Save(const std::string& file_path) const {

			std::stringstream buf;

			Xml::XmlWriter writer("map");
			writer.Root().SetAttribute("version", "1.0");

			// Write tiles.
			if (_room->Tiles().LayerCount() > 0) {

				Xml::XmlNode* tiles_node = writer.Root().AddChild("tiles");
				tiles_node->SetAttribute("layers", _room->Tiles().LayerCount());

				// Write each layer as a separate node.
				for (auto i = _room->Tiles().LayersBegin(); i != _room->Tiles().LayersEnd(); ++i) {
					Xml::XmlNode* layer_node = writer.Root().AddChild("layer");
					layer_node->SetAttribute("depth", i->first);
					for (int j = 0; j < _room->Tiles().Count(); ++j)
						buf << _room->Tiles().AtIndex(j, i->first).id << ',';
					layer_node->SetText(buf.str());
					buf.clear();
				}

			}

			// Write objects.
			if (_room->Objects().Count() > 0) {

				Xml::XmlNode* objects_node = writer.Root().AddChild("objects");

				_room->Objects().ForEach([&](const IObject* obj) {
					Xml::XmlNode* object_node = objects_node->AddChild("object");
					object_node->SetAttribute("id", obj->Id());
					object_node->SetAttribute("x", obj->X());
					object_node->SetAttribute("y", obj->Y());
				});

			}

			return writer.Save(file_path);

		}

	private:
		const IRoom* _room;

	};

}