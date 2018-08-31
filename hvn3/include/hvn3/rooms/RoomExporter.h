#pragma once
#include "hvn3/backgrounds/IBackgroundManager.h"
#include "hvn3/objects/IObject.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/tilesets/TileManager.h"
#include "hvn3/views/IViewManager.h"
#include "hvn3/xml/IXmlResourceAdapter.h"
#include "hvn3/xml/XmlDocument.h"
#include <functional>
#include <memory>
#include <string>
#include <sstream>

namespace hvn3 {

	class RoomExporter {

	public:
		RoomExporter(std::unique_ptr<Xml::IXmlResourceAdapter>& adapter) {
			_adapter = std::move(adapter);
		}

		bool Export(const IRoom& room, const std::string& file_path) const {
			
			Xml::XmlDocument writer("map");
			writer.Root().SetAttribute("version", "1.0");
			writer.Root().SetAttribute("width", room.Width());
			writer.Root().SetAttribute("height", room.Height());

			_writeBackgrounds(room, &writer.Root());
			_writeTiles(room, &writer.Root());
			_writeObjects(room, &writer.Root());
			_writeViews(room, &writer.Root());

			return writer.Save(file_path);

		}

	private:
		std::unique_ptr<Xml::IXmlResourceAdapter> _adapter;

		void _writeBackgrounds(const IRoom& room, Xml::XmlElement* root) const {

			if (room.Backgrounds().Count() <= 0)
				return;

			Xml::XmlElement* backgrounds_node = root->AddChild("backgrounds");

			for (size_t i = 0; i < room.Backgrounds().Count(); ++i) {

				const Background& bg = room.Backgrounds().At(i);

				Xml::XmlElement* node = backgrounds_node->AddChild("background");
				
				_adapter->ExportBackground(bg, *node);

			}


		}
		void _writeTiles(const IRoom& room, Xml::XmlElement* root) const {

			if (room.Tiles().LayerCount() <= 0)
				return;

			Xml::XmlElement* tiles_node = root->AddChild("tiles");

			_adapter->ExportTiles(room.Tiles(), *tiles_node);

		}
		void _writeObjects(const IRoom& room, Xml::XmlElement* root) const {

			if (room.Objects().Count() <= 0)
				return;

			Xml::XmlElement* objects_node = root->AddChild("objects");

			room.Objects().ForEach([&](const IObject* obj) {

				Xml::XmlElement* object_node = objects_node->AddChild("object");

				_adapter->ExportObject(obj, *object_node);

			});

		}
		void _writeViews(const IRoom& room, Xml::XmlElement* root) const {

			if (room.Views().Count() <= 0)
				return;

			Xml::XmlElement* views_node = root->AddChild("views");

			room.Views().ForEach([=](const View& view) {

				Xml::XmlElement* view_node = views_node->AddChild("view");

				_adapter->ExportView(view, *view_node);

			});

		}

	};

}