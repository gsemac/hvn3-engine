#pragma once
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

			Xml::XmlNode* tiles_node = writer.Root().AddChild("tiles");
			for (int i = 0; i < _room->Tiles().Count(); ++i)
				buf << _room->Tiles().AtIndex(i).id << ',';
			tiles_node->SetText(buf.str());
			buf.clear();

			return writer.Save(file_path);
			
		}

	private:
		const IRoom* _room;

	};

}