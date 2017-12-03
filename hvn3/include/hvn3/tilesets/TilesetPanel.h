#pragma once
#include "hvn3/tilesets/Tileset.h"
#include "hvn3/gui/ScrollableControl.h"

namespace hvn3 {
	namespace Gui {

		class TilesetPanel : public ScrollableControl {

		public:
			TilesetPanel(const PointF& position, const SizeF& size, const Tileset& tileset);

			void OnPaint(PaintEventArgs& e) override;
			void OnMouseMove(MouseMoveEventArgs& e) override;
			void OnMouseDown(MouseEventArgs& e) override;
			void OnMouseUp(MouseEventArgs& e) override;
			void OnUpdate(UpdateEventArgs& e) override;

			PointI SelectedTilePosition() const;

		private:
			void UpdateScrollableRegion();

			// Takes a position and snaps it to the tileset grid.
			PointI SnapToTilesetGrid(const PointF& position);
			// Returns true if the point was on the tileset bitmap, returns false otherwise.
			bool PositionIsOnBitmap(const PointF& position);
			
			const Tileset* _tileset;
			PointF _hovered_tile_position;
			PointI _selected_tile;
			RectangleI _selected_region;
			bool _mouse_held;

		};

	}
}