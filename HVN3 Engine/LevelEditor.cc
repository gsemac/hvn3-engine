#include "LevelEditor.h"
#include "io/Keyboard.h"
#include "gui/Gui.h"
#include "GridDrawer.h"
#include "TilesetPanel.h"
#include <allegro5/allegro.h>
#define DEFAULT_GRID_CELL_SIZE 32

namespace hvn3 {
	namespace editor {

		LevelEditor::LevelEditor(const SizeF& size) :
			LevelEditor(size.Width(), size.Height()) {}
		LevelEditor::LevelEditor(unsigned int width, unsigned int height) :
			Room(0, SizeI(width, height)),
			_grid_space_size(DEFAULT_GRID_CELL_SIZE, DEFAULT_GRID_CELL_SIZE),
			_gui_manager(RectangleF(width, height)),
			_visible_region(RectangleF(width, height)),
			_tileset("data/test/tileset1.png", SizeI(32, 32)) {

			_grid_visible = true;


		}

		Gui::GuiManager& LevelEditor::GuiManager() {

			return _gui_manager;

		}

		void LevelEditor::OnUpdate(UpdateEventArgs& e) {

			Room::OnUpdate(e);

			_gui_manager.OnUpdate(e);

			if (Keyboard::KeyPressed(Key::G))
				ToggleGrid(!_grid_visible);

		}
		void LevelEditor::OnDraw(DrawEventArgs& e) {

			Room::OnDraw(e);

		}

		void LevelEditor::ToggleGrid(bool grid_visible) {

			_grid_visible = grid_visible;

		}

		// Inherited protected methods

		void LevelEditor::SetUp() {

			/*Gui::Window* window = new Gui::Window(300, 300, 300, 300, "test");
			GuiManager()->ControlManager()->AddControl(Gui::Control::Create(window));*/

			// Create menu strip.
			Gui::ToolStrip* menu_strip = new Gui::ToolStrip();

			Gui::ToolStripButton* btn_save = new Gui::ToolStripButton;
			btn_save->SetImage(Gui::BitmapResourceId::ExitButton);
			btn_save->SetDisplayStyle(Gui::ToolStripItemDisplayStyle::Image);
			menu_strip->AddItem(btn_save);

			menu_strip->AddItem(new Gui::ToolStripSeparator);
			menu_strip->AddItem(new Gui::ToolStripButton);
			menu_strip->AddItem(new Gui::ToolStripSeparator);
			menu_strip->AddItem(new Gui::ToolStripButton);
			menu_strip->AddItem(new Gui::ToolStripButton);
			menu_strip->AddItem(new Gui::ToolStripButton);
			menu_strip->AddItem(new Gui::ToolStripButton);
			menu_strip->AddItem(new Gui::ToolStripButton);
			menu_strip->AddItem(new Gui::ToolStripButton);
			menu_strip->AddItem(new Gui::ToolStripSeparator);
			menu_strip->AddItem(new Gui::ToolStripLabel("Snap X: "));

			GuiManager().ControlManager()->AddControl(Gui::Control::Create(menu_strip));


			Gui::Window* tileset_window = new Gui::Window(20, 20, 100, 100, "Tiles");
			Gui::TilesetPanel* tileset_panel = new Gui::TilesetPanel(PointF(0, 0), SizeF(tileset_window->Width(), tileset_window->Height() - tileset_window->TitlebarHeight()), _tileset);
			tileset_panel->SetAnchors(Gui::ANCHOR_ALL);
			tileset_window->Controls()->AddControl(Gui::Control::Create(tileset_panel));
			GuiManager().ControlManager()->AddControl(Gui::Control::Create(tileset_window));

			_visible_region.Translate(0, menu_strip->Height());

			// Define a view to control the visible region of the room.


		}
		void LevelEditor::Reset() {

			Room::Reset();

			_gui_manager.Clear();

		}
		void LevelEditor::Render(DrawEventArgs& e) {

			// Move drawing downward so that the room is visible despite the menu strip.
			Drawing::GraphicsState state = e.Graphics().Save();
			Drawing::Transform t;
			t.Translate(_visible_region.X(), _visible_region.Y());
			t.Compose(e.Graphics().GetTransform());
			e.Graphics().SetTransform(t);

			// Render the room.
			Room::Render(e);

			// Restore the previous graphics state.
			e.Graphics().Restore(state);

			e.Graphics().DrawLine(PointF(0, 0), PointF(600, 200), Color::Red, 30);
			e.Graphics().DrawLine(PointF(0, 0), PointF(Width() / 2, Height() / 2), Color::Blue, 30);

			if (_grid_visible)
				_RenderGrid(e);

			_gui_manager.OnDraw(e);

		}

		// Protected methods

		void LevelEditor::_RenderGrid(DrawEventArgs& e) {

			// Draw vertical grid lines.
			for (float i = 0; i < Width(); i += _grid_space_size.Width())
				e.Graphics().DrawLine(i, 0, i, Height(), Color(0, 0, 0, 128), 1);

			// Draw horizontal grid lines.
			for (float i = 0; i < Height(); i += _grid_space_size.Height())
				e.Graphics().DrawLine(0, i, Width(), i, Color(0, 0, 0, 128), 1);

		}

	}
}