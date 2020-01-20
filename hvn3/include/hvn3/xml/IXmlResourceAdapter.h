#pragma once
#include "hvn3/backgrounds/Background.h"
#include "hvn3/objects/ObjectDefs.h"
#include "hvn3/rooms/RoomDefs.h"

namespace hvn3 {

	class Background;
	class IBackgroundManager;
	class IObjectManager;
	class IViewManager;
	class TileManager;
	class View;

	namespace xml {

		class XmlElement;

		class IXmlResourceAdapter {

		public:
			virtual void ExportRoom(const IRoomPtr& data, Xml::XmlElement& node) const = 0;
			virtual void ExportBackground(const Background& data, Xml::XmlElement& node) const = 0;
			virtual void ExportBackgrounds(const IBackgroundManager& data, Xml::XmlElement& node) const = 0;
			virtual void ExportObject(const IObjectPtr& data, Xml::XmlElement& node) const = 0;
			virtual void ExportObjects(const IObjectManager& data, Xml::XmlElement& node) const = 0;
			virtual void ExportTiles(const TileManager& data, Xml::XmlElement& node) const = 0;
			virtual void ExportView(const View& data, Xml::XmlElement& node) const = 0;
			virtual void ExportViews(const IViewManager& data, Xml::XmlElement& node) const = 0;

			virtual IRoomPtr ImportRoom(const Xml::XmlElement& node) const = 0;
			virtual Background ImportBackground(const Xml::XmlElement& node) const = 0;
			virtual void ImportBackgrounds(IBackgroundManager& data, const Xml::XmlElement& node) const = 0;
			virtual IObjectPtr ImportObject(const Xml::XmlElement& node) const = 0;
			virtual void ImportObjects(IObjectManager& data, const Xml::XmlElement& node) const = 0;
			virtual void ImportTiles(TileManager& data, const Xml::XmlElement& node) const = 0;
			virtual View ImportView(const Xml::XmlElement& node) const = 0;
			virtual void ImportViews(IViewManager& data, const Xml::XmlElement& node) const = 0;

		};

	}
}