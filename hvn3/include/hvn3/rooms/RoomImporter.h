#pragma once
#include "hvn3/backgrounds/IBackgroundManager.h"
#include "hvn3/objects/IObject.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/tilesets/TileManager.h"
#include "hvn3/utility/StringUtils.h"
#include "hvn3/xml/XmlDocument.h"
#include <functional>
#include <string>

namespace hvn3 {

	class Room;

	template <typename ResourceAdapterT, typename RoomT = Room>
	class RoomImporter {

	public:
		RoomImporter() {}

		RoomPtr Import(const std::string& file_path) const {

			Xml::XmlDocument doc = Xml::XmlDocument::Open(file_path);

			int width, height;
			width = StringUtils::ParseString<int>(doc.Root()["width"]);
			height = StringUtils::ParseString<int>(doc.Root()["height"]);
			
			RoomPtr room(new RoomT(SizeI(width, height)));

			Xml::XmlElement* backgrounds_node = doc.Root().GetChild("backgrounds");
			Xml::XmlElement* tiles_node = doc.Root().GetChild("tiles");
			Xml::XmlElement* objects_node = doc.Root().GetChild("objects");

			_readBackgrounds(*room, backgrounds_node);
			_readTiles(*room, tiles_node);
			_readObjects(*room, objects_node);

			return room;

		}

	private:
		ResourceAdapterT adapter;

		void _readBackgrounds(IRoom& room, const Xml::XmlElement* node) const {

			if (node == nullptr)
				return;

			for (auto i = node->ChildrenBegin(); i != node->ChildrenEnd(); ++i) {

				Background background = adapter.ImportBackground(*i->get());

				room.GetBackgrounds().Add(background);

			}

		}
		void _readTiles(IRoom& room, const Xml::XmlElement* node) const {

			if (node == nullptr)
				return;

			adapter.ImportTiles(room.GetTiles(), *node);

		}
		void _readObjects(IRoom& room, const Xml::XmlElement* node) const {

			if (node == nullptr)
				return;

			for (auto i = node->ChildrenBegin(); i != node->ChildrenEnd(); ++i) {

				IObject* object = adapter.ImportObject(*i->get());

				if (object != nullptr)
					room.GetObjects().Add(object);

			}

		}

	};

}