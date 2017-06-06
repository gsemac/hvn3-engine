#pragma once
#include "Room.h"
#include "gui/GuiManager.h"

namespace hvn3 {
	namespace editor {

		class LevelEditor : public Room {

		public:
			LevelEditor(const Size& size);
			LevelEditor(unsigned int width, unsigned int height);
			virtual ~LevelEditor() = default;

			Gui::GuiManager* GuiManager();

			virtual void Update(UpdateEventArgs& e) override;
			virtual void Draw(DrawEventArgs& e) override;

			void ToggleGrid(bool grid_visible);

		protected:
			virtual void SetUp() override;
			virtual void Reset() override;
			virtual void Render(DrawEventArgs& e) override;
			virtual void _RenderGrid(DrawEventArgs& e);

		private:
			bool _grid_visible;
			Size _grid_space_size;

			Gui::GuiManager _gui_manager;

		};

	}
}