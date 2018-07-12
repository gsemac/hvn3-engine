#pragma once
#include "hvn3/backgrounds/Background.h"

namespace hvn3 {

	class Background;
	class IObject;
	class TileManager;

	namespace Xml {

		class XmlElement;

		class IXmlResourceAdapter {

		public:
			virtual void ExportBackground(const Background& in, Xml::XmlElement& node) const = 0;
			virtual Background ImportBackground(const Xml::XmlElement& node) const = 0;
			virtual void ExportObject(const IObject& in, Xml::XmlElement& node) const = 0;
			virtual IObject* ImportObject(const Xml::XmlElement& node) const = 0;
			virtual void ExportTiles(const TileManager& in, Xml::XmlElement& node) const = 0;
			virtual void ImportTiles(TileManager& out, const Xml::XmlElement& node) const = 0;

		};

	}
}