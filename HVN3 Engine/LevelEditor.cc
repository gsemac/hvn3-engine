#include "LevelEditor.h"
#include "io/Keyboard.h"
#include <allegro5/allegro.h>
#define DEFAULT_GRID_CELL_SIZE 32

namespace hvn3 {
	namespace editor {

		LevelEditor::LevelEditor(const Size& size) :
			LevelEditor(size.Width(), size.Height()) {}
		LevelEditor::LevelEditor(unsigned int width, unsigned int height) :
			Room(width, height),
			_grid_space_size(DEFAULT_GRID_CELL_SIZE, DEFAULT_GRID_CELL_SIZE) {

			_grid_visible = true;

		}

		void LevelEditor::Update(UpdateEventArgs& e) {

			Room::Update(e);

			if (Keyboard::KeyPressed(Key::G))
				ToggleGrid(!_grid_visible);

		}

		void LevelEditor::ToggleGrid(bool grid_visible) {

			std::cout << "Grid toggled!\n";

			_grid_visible = grid_visible;

		}

		// Protected methods

		void LevelEditor::Render(DrawEventArgs& e) {

			Room::Render(e);

			e.Graphics().DrawLine(Point(0, 0), Point(600, 200), Color::Red, 30);
			e.Graphics().DrawLine(Point(0, 0), Point(Width() / 2, Height() / 2), Color::Blue, 30);

			if (_grid_visible)
				_RenderGrid(e);

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