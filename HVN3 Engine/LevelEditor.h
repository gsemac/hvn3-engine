#pragma once
#include "Room.h"

namespace hvn3 {
	namespace editor {

		class LevelEditor : public Room {

		public:
			LevelEditor(const Size& size);
			LevelEditor(unsigned int width, unsigned int height);
			virtual ~LevelEditor() = default;

			virtual void Update(UpdateEventArgs& e) override;

			void ToggleGrid(bool grid_visible);

		protected:
			virtual void Render(DrawEventArgs& e) override;
			virtual void _RenderGrid(DrawEventArgs& e);

		private:
			bool _grid_visible;
			Size _grid_space_size;

		};

	}
}