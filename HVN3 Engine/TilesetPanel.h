#pragma once
#include "Tileset.h"
#include "gui/ScrollableControl.h"

namespace hvn3 {
	namespace Gui {

		class TilesetPanel : public ScrollableControl {

		public:
			TilesetPanel(const PointF& position, const SizeF& size, const Tileset& tileset);

			void OnPaint(PaintEventArgs& e) override;

		private:
			void UpdateScrollableRegion();
			
			const Tileset* _tileset;


		};

	}
}