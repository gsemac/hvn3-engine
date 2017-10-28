#pragma once
#include "rooms/Room.h"
#include "gui/GuiManager.h"
#include "tilesets/Tileset.h"

namespace hvn3 {
	namespace editor {

		class LevelEditor : public Room {

		public:
			LevelEditor(const SizeI& size, const SizeI& display_size);
			virtual ~LevelEditor() = default;

			Gui::GuiManager& GuiManager();

			virtual void OnUpdate(UpdateEventArgs& e) override;
			virtual void OnDraw(DrawEventArgs& e) override;
			virtual void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) override;

			void ToggleGrid(bool grid_visible);

		protected:
			virtual void OnSetUp() override;
			virtual void OnReset() override;
			virtual void OnRender(DrawEventArgs& e) override;
			virtual void _renderGrid(DrawEventArgs& e);

		private:
			bool _grid_visible;
			SizeF _grid_space_size;
			RectangleF _visible_region;
			Tileset _tileset;

			Gui::GuiManager _gui_manager;

		};

	}
}