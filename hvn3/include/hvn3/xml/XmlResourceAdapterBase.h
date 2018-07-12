#pragma once
#include "hvn3/xml/IXmlResourceAdapter.h"

namespace hvn3 {
	namespace Xml {

		class XmlResourceAdapterBase :
			public IXmlResourceAdapter {

		public:
			void ExportObject(const IObject& in, Xml::XmlElement& node) const override;
			void ExportBackground(const Background& in, Xml::XmlElement& node) const override;
			void ExportTiles(const TileManager& in, Xml::XmlElement& node) const override;
			void ExportView(const View& data, Xml::XmlElement& node) const override;

			IObject* ImportObject(const Xml::XmlElement& node) const override;
			Background ImportBackground(const Xml::XmlElement& node) const override;
			void ImportTiles(TileManager& out, const Xml::XmlElement& node) const override;
			View ImportView(const Xml::XmlElement& node) const override;

		protected:
			void WriteDefaultProperties(const IObject& data, Xml::XmlElement& node) const;
			void WriteDefaultProperties(const Background& data, Xml::XmlElement& node) const;
			void WriteDefaultProperties(const TileManager& data, Xml::XmlElement& node) const;
			void WriteDefaultProperties(const View& data, Xml::XmlElement& node) const;

			void ReadDefaultProperties(IObject& data, const Xml::XmlElement& node) const;
			void ReadDefaultProperties(Background& data, const Xml::XmlElement& node) const;
			void ReadDefaultProperties(TileManager& data, const Xml::XmlElement& node) const;
			void ReadDefaultProperties(View& data, const Xml::XmlElement& node) const;
			
		};

	}
}