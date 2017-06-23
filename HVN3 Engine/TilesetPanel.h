#pragma once
#include "Tileset.h"
#include "gui/ScrollableControl.h"

namespace hvn3 {
	namespace Gui {

		class TilesetPanel : public ScrollableControl {

		public:
			TilesetPanel(const PointF& position, const SizeF& size, const Tileset& tileset);

			void OnPaint(PaintEventArgs& e) override;
			void OnMouseMove(MouseMoveEventArgs& e) override;
			void OnMouseDown(MouseEventArgs& e) override;

		private:
			void UpdateScrollableRegion();
			PointF SnapToTilesetGrid(const PointF& position);

			const Tileset* _tileset;
			PointF _hovered_tile_position;
			RectangleI _selected_tile_region;

		};

	}
}