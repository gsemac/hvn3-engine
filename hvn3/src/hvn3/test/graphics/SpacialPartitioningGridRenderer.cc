#pragma once
#include "hvn3/test/graphics/SpacialPartitioningGridRenderer.h"
#include "hvn3/exceptions/Exception.h"

namespace hvn3 {

	SpacialPartitioningGridRenderer::SpacialPartitioningGridRenderer(Graphics::Graphics& graphics) :
		_graphics(graphics) {
	}

	void SpacialPartitioningGridRenderer::Draw(SpacialPartitioningGridRenderer& grid) {

		throw System::NotImplementedException();

		/*for (auto i = _grid.begin(); i != _grid.end(); ++i) {

			PointI cell = (*i).first;

			e.Graphics().DrawRectangle(
				static_cast<float>(cell.X() * _cell_size.Width()),
				static_cast<float>(cell.Y() * _cell_size.Width()),
				static_cast<float>(_cell_size.Width()),
				static_cast<float>(_cell_size.Height()),
				Color::Fuchsia, 1.0f);

		}*/

	}

}