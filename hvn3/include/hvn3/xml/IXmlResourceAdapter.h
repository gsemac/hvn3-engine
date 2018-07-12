#pragma once
#include "hvn3/backgrounds/Background.h"

namespace hvn3 {

	class Background;
	class IObject;
	class TileManager;
	class View;

	namespace Xml {

		class XmlElement;

		class IXmlResourceAdapter {

		public:
			virtual void ExportObject(const IObject* data, Xml::XmlElement& node) const = 0;
			virtual void ExportBackground(const Background& data, Xml::XmlElement& node) const = 0;
			virtual void ExportTiles(const TileManager& data, Xml::XmlElement& node) const = 0;
			virtual void ExportView(const View& data, Xml::XmlElement& node) const = 0;

			virtual Background ImportBackground(const Xml::XmlElement& node) const = 0;			
			virtual IObject* ImportObject(const Xml::XmlElement& node) const = 0;			
			virtual void ImportTiles(TileManager& data, const Xml::XmlElement& node) const = 0;
			virtual View ImportView(const Xml::XmlElement& node) const = 0;

		};

	}
}