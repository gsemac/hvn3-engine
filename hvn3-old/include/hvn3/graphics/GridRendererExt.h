#pragma once
#include "hvn3/graphics/Graphics.h"
#include "hvn3/graphics/Pen.h"
#include "hvn3/utility/Grid.h"
#include <vector>

namespace hvn3 {
	namespace Graphics {

		class GridRendererExt {

		public:
			GridRendererExt();

			void Draw(Graphics& canvas, const PointF& position, const Grid& grid);

			void SetLineStyle(const Pen& value);
			void SetLinesVisible(bool value);
			void SetCellColor(const Color& value);
			void SetCellColors(std::initializer_list<Color> value);
			void SetCellMargin(float value);
		
		private:
			Bitmap _buffer;
			bool _buffer_invalidated;
			SizeF _last_cell_size;
			Pen _pen;
			std::vector<Color> _cell_colors;
			float _cell_margin;
			bool _lines_visible;

			void _refreshBuffer(const Grid& grid);
			SizeF _getActualCellSize(const Grid& grid);

		};

	}
}