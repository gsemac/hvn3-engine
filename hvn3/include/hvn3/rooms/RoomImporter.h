#pragma once
#include "hvn3/backgrounds/IBackgroundManager.h"
#include "hvn3/objects/IObject.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/tilesets/TileManager.h"
#include "hvn3/utility/StringUtils.h"
#include "hvn3/views/IViewManager.h"
#include "hvn3/xml/IXmlResourceAdapter.h"
#include "hvn3/xml/XmlDocument.h"
#include <functional>
#include <memory>
#include <string>

namespace hvn3 {

	class Room;

	template <typename RoomT = Room>
	class RoomImporter {

	public:
		RoomImporter(std::unique_ptr<Xml::IXmlResourceAdapter>& adapter) {
			_adapter = std::move(adapter);
		}

		RoomPtr Import(const std::string& file_path) const {

			Xml::XmlDocument doc = Xml::XmlDocument::Open(file_path);

			int width, height;
			width = StringUtils::Parse<int>(doc.Root()["width"]);
			height = StringUtils::Parse<int>(doc.Root()["height"]);

			RoomPtr room(new RoomT(SizeI(width, height)));

			Xml::XmlElement* backgrounds_node = doc.Root().GetChild("backgrounds");
			Xml::XmlElement* tiles_node = doc.Root().GetChild("tiles");
			Xml::XmlElement* objects_node = doc.Root().GetChild("objects");
			Xml::XmlElement* views_node = doc.Root().GetChild("views");

			_readBackgrounds(*room, backgrounds_node);
			_readTiles(*room, tiles_node);
			_readObjects(*room, objects_node);
			_readViews(*room, views_node);

			return room;

		}

	private:
		std::unique_ptr<Xml::IXmlResourceAdapter> _adapter;

		void _readBackgrounds(IRoom& room, const Xml::XmlElement* node) const {

			if (node == nullptr)
				return;

			for (auto i = node->ChildrenBegin(); i != node->ChildrenEnd(); ++i) {

				Background background = _adapter->ImportBackground(*i->get());

				room.GetBackgrounds().Add(background);

			}

		}
		void _readTiles(IRoom& room, const Xml::XmlElement* node) const {

			if (node == nullptr)
				return;

			_adapter->ImportTiles(room.GetTiles(), *node);

		}
		void _readObjects(IRoom& room, const Xml::XmlElement* node) const {

			if (node == nullptr)
				return;

			for (auto i = node->ChildrenBegin(); i != node->ChildrenEnd(); ++i) {

				IObject* object = _adapter->ImportObject(*i->get());

				if (object != nullptr)
					room.GetObjects().Add(object);

			}

		}
		void _readViews(IRoom& room, const Xml::XmlElement* node) const {

			if (node == nullptr)
				return;

			for (auto i = node->ChildrenBegin(); i != node->ChildrenEnd(); ++i) {

				View view = _adapter->ImportView(*i->get());

				if ((*i)->HasAttribute("following_id")) {

					ObjectId id = StringUtils::Parse<ObjectId>((*i)->GetAttribute("following_id"));

					IObject* object = room.GetObjects().Find(id);

					if (object != nullptr)
						view.SetFollowing(object);

				}
				
				room.GetViews().Add(view);

			}

		}

	};

}