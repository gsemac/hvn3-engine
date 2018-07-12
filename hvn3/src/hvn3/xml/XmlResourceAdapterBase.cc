#pragma once
#include "hvn3/backgrounds/Background.h"
#include "hvn3/objects/Object.h"
#include "hvn3/tilesets/TileManager.h"
#include "hvn3/utility/StringUtils.h"
#include "hvn3/views/View.h"
#include "hvn3/xml/XmlResourceAdapterBase.h"
#include "hvn3/xml/XmlElement.h"

namespace hvn3 {
	namespace Xml {

		void drawBadTexture(const PointF& pos, Graphics::Graphics& canvas, Color color1 = Color::Fuchsia, Color color2 = Color::Black) {

			canvas.DrawSolidRectangle(pos.x, pos.y, 16.0f, 16.0f, color1);
			canvas.DrawSolidRectangle(pos.x + 16.0f, pos.y, 16.0f, 16.0f, color2);
			canvas.DrawSolidRectangle(pos.x, pos.y + 16.0f, 16.0f, 16.0f, color2);
			canvas.DrawSolidRectangle(pos.x + 16.0f, pos.y + 16.0f, 16.0f, 16.0f, color1);

		}

		class BadObject :
			public Object {
		public:
			BadObject() : Object(NoOne) {}
			void OnDraw(DrawEventArgs& e) override {
				drawBadTexture(Position(), e.Graphics());
			}
		};



		void XmlResourceAdapterBase::ExportObject(const IObject& data, Xml::XmlElement& node) const {
			WriteDefaultProperties(data, node);
		}
		void XmlResourceAdapterBase::ExportBackground(const Background& data, Xml::XmlElement& node) const {
			WriteDefaultProperties(data, node);
		}
		void XmlResourceAdapterBase::ExportTiles(const TileManager& data, Xml::XmlElement& node) const {
			WriteDefaultProperties(data, node);
		}
		void XmlResourceAdapterBase::ExportView(const View& data, Xml::XmlElement& node) const {
			WriteDefaultProperties(data, node);
		}

		IObject* XmlResourceAdapterBase::ImportObject(const Xml::XmlElement& node) const {

			// The user needs to override this function for proper behavior.

			IObject* ptr = new BadObject;

			ReadDefaultProperties(*ptr, node);

			return ptr;

		}
		Background XmlResourceAdapterBase::ImportBackground(const Xml::XmlElement& node) const {

			// The user needs to override this function for proper behavior.

			Graphics::Bitmap bmp(32, 32);
			Graphics::Graphics gfx(bmp);

			drawBadTexture(PointF(0.0f, 0.0f), gfx, Color::Silver, Color::White);

			Background background(bmp);

			ReadDefaultProperties(background, node);

			return background;

		}
		void XmlResourceAdapterBase::ImportTiles(TileManager& out, const Xml::XmlElement& node) const {
			ReadDefaultProperties(out, node);
		}
		View XmlResourceAdapterBase::ImportView(const Xml::XmlElement& node) const {

			float x = StringUtils::ParseString<float>(node["x"]);
			float y = StringUtils::ParseString<float>(node["y"]);
			float width = StringUtils::ParseString<float>(node["width"]);
			float height = StringUtils::ParseString<float>(node["height"]);

			float port_x = StringUtils::ParseString<float>(node["port_x"]);
			float port_y = StringUtils::ParseString<float>(node["port_y"]);
			float port_width = StringUtils::ParseString<float>(node["port_width"]);
			float port_height = StringUtils::ParseString<float>(node["port_height"]);

			View view(x, y, width, height, port_x, port_y, port_width, port_height);

			XmlResourceAdapterBase::ReadDefaultProperties(view, node);

			return view;

		}


		void XmlResourceAdapterBase::WriteDefaultProperties(const IObject& data, Xml::XmlElement& node) const {

			node.SetAttribute("id", data.Id());
			node.SetAttribute("x", data.X());
			node.SetAttribute("y", data.Y());

		}
		void XmlResourceAdapterBase::WriteDefaultProperties(const Background& data, Xml::XmlElement& node) const {

			// We will only export the properties that differ from the defaults.

			if (!data.Offset().IsEmpty()) {
				node.SetAttribute("x", data.Offset().x);
				node.SetAttribute("y", data.Offset().y);
			}

			if (data.Scale().XScale() != 1.0f || data.Scale().YScale() != 1.0f) {
				node.SetAttribute("scale_x", data.Scale().XScale());
				node.SetAttribute("scale_y", data.Scale().YScale());
			}

			if (data.Velocity().X() != 0.0f || data.Velocity().Y() != 0.0f) {
				node.SetAttribute("velocity_x", data.Velocity().X());
				node.SetAttribute("velocity_y", data.Velocity().Y());
			}

			if (data.IsForeground())
				node.SetAttribute("foreground", true);

			if (data.IsTiledHorizontally())
				node.SetAttribute("tiled_x", true);

			if (data.IsTiledVertically())
				node.SetAttribute("tiled_y", true);

			node.SetAttribute("visible", data.Visible());

			if (data.Fixed())
				node.SetAttribute("fixed", true);

		}
		void XmlResourceAdapterBase::WriteDefaultProperties(const TileManager& data, Xml::XmlElement& node) const {

			std::stringstream buf;

			node.SetAttribute("layers", data.LayerCount());

			// Write each layer as a separate node.
			for (auto i = data.LayersBegin(); i != data.LayersEnd(); ++i) {

				Xml::XmlElement* layer_node = node.AddChild("layer");
				layer_node->SetAttribute("depth", i->first);

				for (int j = 0; j < data.Count(); ++j)
					buf << data.AtIndex(j, i->first).id << ',';

				layer_node->SetText(buf.str());

				buf.clear();

			}

		}
		void XmlResourceAdapterBase::WriteDefaultProperties(const View& data, Xml::XmlElement& node) const {

			node.SetAttribute("x", data.X());
			node.SetAttribute("y", data.Y());
			node.SetAttribute("width", data.Size().width);
			node.SetAttribute("height", data.Size().height);

			node.SetAttribute("port_x", data.Port().X());
			node.SetAttribute("port_y", data.Port().Y());
			node.SetAttribute("port_width", data.Port().Width());
			node.SetAttribute("port_height", data.Port().Height());

			node.SetAttribute("hbor", data.HorizontalBorder());
			node.SetAttribute("vbor", data.VerticalBorder());

			node.SetAttribute("angle", data.Angle());

			if (data.GetFollowing() != nullptr)
				node.SetAttribute("following_id", data.GetFollowing()->Id());

			node.SetAttribute("enabled", data.Enabled());
			node.SetAttribute("mouse_tracking", data.MouseTrackingEnabled());

		}

		void XmlResourceAdapterBase::ReadDefaultProperties(IObject& data, const Xml::XmlElement& node) const {

			data.SetX(StringUtils::ParseString<float>(node["x"]));
			data.SetY(StringUtils::ParseString<float>(node["y"]));

		}
		void XmlResourceAdapterBase::ReadDefaultProperties(Background& data, const Xml::XmlElement& node) const {

			if (node.HasAttribute("x") && node.HasAttribute("y")) {

				float x = StringUtils::ParseString<float>(node["x"]);
				float y = StringUtils::ParseString<float>(node["y"]);

				data.SetOffset(x, y);

			}

			if (node.HasAttribute("scale_x") && node.HasAttribute("scale_y")) {

				float x = StringUtils::ParseString<float>(node["scale_x"]);
				float y = StringUtils::ParseString<float>(node["scale_y"]);

				data.SetScale(x, y);

			}

			if (node.HasAttribute("velocity_x") && node.HasAttribute("velocity_y")) {

				float x = StringUtils::ParseString<float>(node["velocity_x"]);
				float y = StringUtils::ParseString<float>(node["velocity_y"]);

				data.SetVelocity(Vector2d(x, y));

			}

			if (node.HasAttribute("foreground"))
				data.SetForeground(StringUtils::ParseString<bool>(node["foreground"]));

			if (node.HasAttribute("tiled_x"))
				data.SetTiledHorizontally(StringUtils::ParseString<bool>(node["tiled_x"]));

			if (node.HasAttribute("tiled_y"))
				data.SetTiledVertically(StringUtils::ParseString<bool>(node["tiled_y"]));

			if (node.HasAttribute("visible"))
				data.SetVisible(StringUtils::ParseString<bool>(node["visible"]));

			if (node.HasAttribute("fixed"))
				data.SetFixed(StringUtils::ParseString<bool>(node["fixed"]));

		}
		void XmlResourceAdapterBase::ReadDefaultProperties(TileManager& data, const Xml::XmlElement& node) const {

			for (auto i = node.ChildrenBegin(); i != node.ChildrenEnd(); ++i) {

				TileManager::layer_id depth = StringUtils::ParseString<TileManager::layer_id>(i->get()->GetAttribute("depth"));

				data.AddLayer(depth);

				std::stringstream ss;
				std::string token;
				int x = 0;
				int y = 0;

				ss << i->get()->Text();

				while (std::getline(ss, token, ',')) {

					TileManager::tile_id tile_id = StringUtils::ParseString<TileManager::tile_id>(token);
					data.SetTile(x, y, tile_id, depth);

					if (++x > data.Width()) {
						x = 0;
						++y;
					}

				}


			}

		}
		void XmlResourceAdapterBase::ReadDefaultProperties(View& data, const Xml::XmlElement& node) const {

			bool enabled = StringUtils::ParseString<bool>(node["enabled"]);
			bool mouse_tracking = StringUtils::ParseString<bool>(node["mouse_tracking"]);
			
			float hbor = StringUtils::ParseString<float>(node["hbor"]);
			float vbor = StringUtils::ParseString<float>(node["vbor"]);
			//ObjectId following_id = StringUtils::ParseString<ObjectId>(node["following_id"]);

			data.SetEnabled(enabled);
			data.SetMouseTrackingEnabled(mouse_tracking);

			// The object being followed is set later by the room importer.
			data.SetFollowing(nullptr, hbor, vbor);

		}

	}
}