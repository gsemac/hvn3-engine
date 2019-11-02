#pragma once
#include "hvn3/backgrounds/Background.h"
#include "hvn3/backgrounds/IBackgroundManager.h"
#include "hvn3/objects/IObjectManager.h"
#include "hvn3/objects/Object.h"
#include "hvn3/rooms/Room.h"
#include "hvn3/tiles/TileManager.h"
#include "hvn3/utility/StringUtils.h"
#include "hvn3/views/IViewManager.h"
#include "hvn3/views/View.h"
#include "hvn3/xml/XmlElement.h"
#include "hvn3/xml/IXmlResourceAdapter.h"

namespace hvn3 {
	namespace Xml {

		template<typename RoomType = Room>
		class XmlResourceAdapterBase :
			public IXmlResourceAdapter {

		private:
			static void _drawBadTexture(const PointF& pos, Graphics::Graphics& canvas, Color color1 = Color::Fuchsia, Color color2 = Color::Black) {

				canvas.DrawSolidRectangle(pos.x, pos.y, 16.0f, 16.0f, color1);
				canvas.DrawSolidRectangle(pos.x + 16.0f, pos.y, 16.0f, 16.0f, color2);
				canvas.DrawSolidRectangle(pos.x, pos.y + 16.0f, 16.0f, 16.0f, color2);
				canvas.DrawSolidRectangle(pos.x + 16.0f, pos.y + 16.0f, 16.0f, 16.0f, color1);

			}

			class BadObject :
				public Object {

			public:
				BadObject() :
					Object(NoOne) {}

				void OnDraw(DrawEventArgs& e) override {
					_drawBadTexture(Position(), e.Graphics());
				}

			};

		public:
			void WriteDefaultProperties(const IRoomPtr& data, Xml::XmlElement& node) const {

				node.SetTag("map");
				node.SetAttribute("version", "1.0");
				node.SetAttribute("width", data->Width());
				node.SetAttribute("height", data->Height());

				Xml::XmlElement* backgrounds_node = node.AddChild("backgrounds");
				Xml::XmlElement* tiles_node = node.AddChild("tiles");
				Xml::XmlElement* objects_node = node.AddChild("objects");
				Xml::XmlElement* views_node = node.AddChild("views");

				ExportBackgrounds(data->Backgrounds(), *backgrounds_node);
				ExportObjects(data->Objects(), *objects_node);
				ExportTiles(data->Tiles(), *tiles_node);
				ExportViews(data->Views(), *views_node);

			}
			static void WriteDefaultProperties(const IObjectPtr& data, Xml::XmlElement& node) {

				node.SetAttribute("id", data->Id());
				node.SetAttribute("x", data->X());
				node.SetAttribute("y", data->Y());

			}
			static void WriteDefaultProperties(const Background& data, Xml::XmlElement& node) {

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
			static void WriteDefaultProperties(const TileManager& data, Xml::XmlElement& node) {

				std::stringstream buf;

				node.SetAttribute("tile_w", data.TileSize().width);
				node.SetAttribute("tile_h", data.TileSize().height);

				Xml::XmlElement* layers_node = node.AddChild("layers");
				layers_node->SetAttribute("layers", data.LayerCount());

				// Write each layer as a separate node.
				for (auto i = data.LayersBegin(); i != data.LayersEnd(); ++i) {

					Xml::XmlElement* layer_node = layers_node->AddChild("layer");
					layer_node->SetAttribute("depth", i->first);

					for (int j = 0; j < data.Count(); ++j)
						buf << data.AtIndex(j, i->first).id << ',';

					layer_node->SetText(buf.str());

					buf.clear();

				}

			}
			static void WriteDefaultProperties(const View& data, Xml::XmlElement& node) {

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

			void ReadDefaultProperties(IRoomPtr& data, const Xml::XmlElement& node) const {

				if (!data)
					return;

				const Xml::XmlElement* backgrounds_node = node.GetChild("backgrounds");
				const Xml::XmlElement* tiles_node = node.GetChild("tiles");
				const Xml::XmlElement* objects_node = node.GetChild("objects");
				const Xml::XmlElement* views_node = node.GetChild("views");

				if (backgrounds_node != nullptr)
					ImportBackgrounds(data->Backgrounds(), *backgrounds_node);

				if (tiles_node != nullptr)
					ImportTiles(data->Tiles(), *tiles_node);

				if (objects_node != nullptr)
					ImportObjects(data->Objects(), *objects_node);

				if (views_node != nullptr)
					ImportViews(data->Views(), *views_node);

			}
			static void ReadDefaultProperties(IObjectPtr& data, const Xml::XmlElement& node) {

				if (!data)
					return;

				data->SetX(StringUtils::Parse<float>(node["x"]));
				data->SetY(StringUtils::Parse<float>(node["y"]));

			}
			static void ReadDefaultProperties(Background& data, const Xml::XmlElement& node) {

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
			static void ReadDefaultProperties(TileManager& data, const Xml::XmlElement& node) {

				int tile_w = StringUtils::Parse<int>(node.GetAttribute("tile_w"));
				int tile_h = StringUtils::Parse<int>(node.GetAttribute("tile_h"));

				data.SetTileSize(SizeI(tile_w, tile_h));

				const Xml::XmlElement* layers_node = node.GetChild("layers");

				for (auto i = layers_node->ChildrenBegin(); i != layers_node->ChildrenEnd(); ++i) {

					TileManager::layer_id depth = StringUtils::Parse<TileManager::layer_id>(i->get()->GetAttribute("depth"));

					data.AddLayer(depth);

					std::stringstream ss;
					TileManager::tile_id id;
					int x = 0;
					int y = 0;

					ss << i->get()->Text();

					while (ss >> id) {

						data.SetTile(x, y, id, depth);

						if (++x >= data.Columns()) {
							x = 0;
							++y;
						}

						// Ignore the comma following the tile id.
						ss.ignore(1);

					}

				}

			}
			static void ReadDefaultProperties(View& data, const Xml::XmlElement& node) {

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

			void ExportRoom(const IRoomPtr& data, Xml::XmlElement& node) const override {
				WriteDefaultProperties(data, node);
			}
			void ExportBackground(const Background& data, Xml::XmlElement& node) const override {
				WriteDefaultProperties(data, node);
			}
			void ExportBackgrounds(const IBackgroundManager& data, Xml::XmlElement& node) const override {

				if (data.Count() <= 0)
					return;

				for (size_t i = 0; i < data.Count(); ++i) {

					const Background& bg = data.At(i);

					Xml::XmlElement* background_node = node.AddChild("background");

					ExportBackground(bg, *background_node);

				}

			}
			void ExportObject(const IObjectPtr& data, Xml::XmlElement& node) const override {
				WriteDefaultProperties(data, node);
			}
			void ExportObjects(const IObjectManager& data, Xml::XmlElement& node) const override {

				if (data.Count() <= 0)
					return;

				data.ForEach([&](const IObjectPtr& obj) {

					Xml::XmlElement* object_node = node.AddChild("object");

					ExportObject(obj, *object_node);

				});

			}
			void ExportTiles(const TileManager& data, Xml::XmlElement& node) const override {
				WriteDefaultProperties(data, node);
			}
			void ExportView(const View& data, Xml::XmlElement& node) const override {
				WriteDefaultProperties(data, node);
			}
			void ExportViews(const IViewManager& data, Xml::XmlElement& node) const override {

				if (data.Count() <= 0)
					return;

				data.ForEach([&](const View& view) {

					Xml::XmlElement* view_node = node.AddChild("view");

					ExportView(view, *view_node);

				});

			}

			IRoomPtr ImportRoom(const Xml::XmlElement& node) const override {

				int width, height;
				width = StringUtils::Parse<int>(node["width"]);
				height = StringUtils::Parse<int>(node["height"]);

				IRoomPtr room(new RoomType(SizeI(width, height)));

				ReadDefaultProperties(room, node);

				return room;

			}
			Background ImportBackground(const Xml::XmlElement& node) const override {

				// The user needs to override this function for proper behavior.

				Graphics::Bitmap bmp(32, 32);
				Graphics::Graphics gfx(bmp);

				_drawBadTexture(PointF(0.0f, 0.0f), gfx, Color::Silver, Color::White);

				Background background(bmp);

				ReadDefaultProperties(background, node);

				return background;

			}
			void ImportBackgrounds(IBackgroundManager& data, const Xml::XmlElement& node) const override {

				for (auto i = node.ChildrenBegin(); i != node.ChildrenEnd(); ++i) {

					Background background = ImportBackground(*i->get());

					data.Add(background);

				}

			}
			IObjectPtr ImportObject(const Xml::XmlElement& node) const override {

				// The user needs to override this function for proper behavior.

				IObjectPtr ptr = hvn3::make_object<BadObject>();

				ReadDefaultProperties(ptr, node);

				return ptr;

			}
			void ImportObjects(IObjectManager& data, const Xml::XmlElement& node) const override {

				for (auto i = node.ChildrenBegin(); i != node.ChildrenEnd(); ++i) {

					IObjectPtr object = ImportObject(*i->get());

					if (object)
						data.Add(object);

				}

			}
			void ImportTiles(TileManager& out, const Xml::XmlElement& node) const override {
				ReadDefaultProperties(out, node);
			}
			View ImportView(const Xml::XmlElement& node) const override {

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
			void ImportViews(IViewManager& data, const Xml::XmlElement& node) const {

				for (auto i = node.ChildrenBegin(); i != node.ChildrenEnd(); ++i) {

					View view = ImportView(*i->get());

					// #todo Assign following object to imported views.

					/*if ((*i)->HasAttribute("following_id")) {

						ObjectId id = StringUtils::Parse<ObjectId>((*i)->GetAttribute("following_id"));
						IObject* object = room.GetObjects().Find(id);

						if (object != nullptr)
							view.SetFollowing(object);

					}*/

					data.Add(view);

				}

			}

		};

	}
}