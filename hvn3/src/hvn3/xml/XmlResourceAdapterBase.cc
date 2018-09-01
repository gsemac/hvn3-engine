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



		void XmlResourceAdapterBase::ExportObject(const IObject* data, Xml::XmlElement& node) const {
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

			ReadDefaultProperties(ptr, node);

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

			float x = StringUtils::Parse<float>(node["x"]);
			float y = StringUtils::Parse<float>(node["y"]);
			float width = StringUtils::Parse<float>(node["width"]);
			float height = StringUtils::Parse<float>(node["height"]);

			float port_x = StringUtils::Parse<float>(node["port_x"]);
			float port_y = StringUtils::Parse<float>(node["port_y"]);
			float port_width = StringUtils::Parse<float>(node["port_width"]);
			float port_height = StringUtils::Parse<float>(node["port_height"]);

			View view(x, y, width, height, port_x, port_y, port_width, port_height);

			XmlResourceAdapterBase::ReadDefaultProperties(view, node);

			return view;

		}


		void XmlResourceAdapterBase::WriteDefaultProperties(const IObject* data, Xml::XmlElement& node) {

			node.SetAttribute("id", data->Id());
			node.SetAttribute("x", data->X());
			node.SetAttribute("y", data->Y());

		}
		void XmlResourceAdapterBase::WriteDefaultProperties(const Background& data, Xml::XmlElement& node) {

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
		void XmlResourceAdapterBase::WriteDefaultProperties(const TileManager& data, Xml::XmlElement& node) {

			std::stringstream buf;

			node.SetAttribute("layers", data.LayerCount());
			node.SetAttribute("tile_w", data.TileSize().width);
			node.SetAttribute("tile_h", data.TileSize().height);

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
		void XmlResourceAdapterBase::WriteDefaultProperties(const View& data, Xml::XmlElement& node) {

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

		void XmlResourceAdapterBase::ReadDefaultProperties(IObject* data, const Xml::XmlElement& node) {

			if (data == nullptr)
				return;

			data->SetX(StringUtils::Parse<float>(node["x"]));
			data->SetY(StringUtils::Parse<float>(node["y"]));

		}
		void XmlResourceAdapterBase::ReadDefaultProperties(Background& data, const Xml::XmlElement& node) {

			if (node.HasAttribute("x") && node.HasAttribute("y")) {

				float x = StringUtils::Parse<float>(node["x"]);
				float y = StringUtils::Parse<float>(node["y"]);

				data.SetOffset(x, y);

			}

			if (node.HasAttribute("scale_x") && node.HasAttribute("scale_y")) {

				float x = StringUtils::Parse<float>(node["scale_x"]);
				float y = StringUtils::Parse<float>(node["scale_y"]);

				data.SetScale(x, y);

			}

			if (node.HasAttribute("velocity_x") && node.HasAttribute("velocity_y")) {

				float x = StringUtils::Parse<float>(node["velocity_x"]);
				float y = StringUtils::Parse<float>(node["velocity_y"]);

				data.SetVelocity(Vector2d(x, y));

			}

			if (node.HasAttribute("foreground"))
				data.SetForeground(StringUtils::Parse<bool>(node["foreground"]));

			if (node.HasAttribute("tiled_x"))
				data.SetTiledHorizontally(StringUtils::Parse<bool>(node["tiled_x"]));

			if (node.HasAttribute("tiled_y"))
				data.SetTiledVertically(StringUtils::Parse<bool>(node["tiled_y"]));

			if (node.HasAttribute("visible"))
				data.SetVisible(StringUtils::Parse<bool>(node["visible"]));

			if (node.HasAttribute("fixed"))
				data.SetFixed(StringUtils::Parse<bool>(node["fixed"]));

		}
		void XmlResourceAdapterBase::ReadDefaultProperties(TileManager& data, const Xml::XmlElement& node) {

			int tile_w = StringUtils::Parse<int>(node.GetAttribute("tile_w"));
			int tile_h = StringUtils::Parse<int>(node.GetAttribute("tile_h"));

			data.SetTileSize(SizeI(tile_w, tile_h));

			for (auto i = node.ChildrenBegin(); i != node.ChildrenEnd(); ++i) {

				TileManager::layer_id depth = StringUtils::Parse<TileManager::layer_id>(i->get()->GetAttribute("depth"));

				data.AddLayer(depth);

				std::stringstream ss;
				std::string token;
				int x = 0;
				int y = 0;

				ss << i->get()->Text();

				while (std::getline(ss, token, ',')) {

					TileManager::tile_id tile_id = StringUtils::Parse<TileManager::tile_id>(token);
					data.SetTile(x, y, tile_id, depth);

					if (++x > data.Columns()) {
						x = 0;
						++y;
					}

				}


			}

		}
		void XmlResourceAdapterBase::ReadDefaultProperties(View& data, const Xml::XmlElement& node) {

			bool enabled = StringUtils::Parse<bool>(node["enabled"]);
			bool mouse_tracking = StringUtils::Parse<bool>(node["mouse_tracking"]);
			
			float hbor = StringUtils::Parse<float>(node["hbor"]);
			float vbor = StringUtils::Parse<float>(node["vbor"]);
			//ObjectId following_id = StringUtils::Parse<ObjectId>(node["following_id"]);

			data.SetEnabled(enabled);
			data.SetMouseTrackingEnabled(mouse_tracking);

			// The object being followed is set later by the room importer.
			data.SetFollowing(nullptr, hbor, vbor);

		}

	}
}