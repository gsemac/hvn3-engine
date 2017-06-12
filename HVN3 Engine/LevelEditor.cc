#include "LevelEditor.h"
#include "io/Keyboard.h"
#include "gui/Gui.h"
#include "GridDrawer.h"
#include <allegro5/allegro.h>
#define DEFAULT_GRID_CELL_SIZE 32

namespace hvn3 {
	namespace editor {

		LevelEditor::LevelEditor(const SizeF& size) :
			LevelEditor(size.Width(), size.Height()) {}
		LevelEditor::LevelEditor(unsigned int width, unsigned int height) :
			Room(width, height),
			_grid_space_size(DEFAULT_GRID_CELL_SIZE, DEFAULT_GRID_CELL_SIZE),
			_gui_manager(RectangleF(width, height)) {

			_grid_visible = true;

		}

		Gui::GuiManager* LevelEditor::GuiManager() {

			return &_gui_manager;

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

			std::cout << "Grid toggled!\n";

			_grid_visible = grid_visible;

		}

		// Protected methods

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

			GuiManager()->ControlManager()->AddControl(Gui::Control::Create(menu_strip));

		}
		void LevelEditor::Reset() {

			Room::Reset();

			_gui_manager.Clear();

		}
		void LevelEditor::Render(DrawEventArgs& e) {

			Room::Render(e);

			e.Graphics().DrawLine(Point2F(0, 0), Point2F(600, 200), Color::Red, 30);
			e.Graphics().DrawLine(Point2F(0, 0), Point2F(Width() / 2, Height() / 2), Color::Blue, 30);

			Drawing::GridDrawer gv(e.Graphics());

			gv.DrawGrid(Point2F(30, 30), Grid(10, 10, 32, 32), Color::Silver, Color::White);

			/*if (_grid_visible)
				_RenderGrid(e);*/

			_gui_manager.OnDraw(e);

		}
		void LevelEditor::_RenderGrid(DrawEventArgs& e) {

			// Draw vertical grid lines.
			for (float i = 0; i < Width(); i += _grid_space_size.Width())
				e.Graphics().DrawLine(i, 0, i, Height(), Color(0, 0, 0, 0.4), 1);

			// Draw horizontal grid lines.
			for (float i = 0; i < Height(); i += _grid_space_size.Height())
				e.Graphics().DrawLine(0, i, Width(), i, Color(0, 0, 0, 0.4), 1);

		}

	}
}