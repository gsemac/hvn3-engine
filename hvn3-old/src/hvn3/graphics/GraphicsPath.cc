#include "hvn3/graphics/GraphicsPath.h"

namespace hvn3 {

	void GraphicsPath::AddCircle(float x, float y, float radius) {

		// #todo Provide all points for generating a proper circle with smooth curves.

		_vertices.emplace_back(vertex_t{ x, y - radius, VertexTag::Curve });
		_vertices.emplace_back(vertex_t{ x + radius, y, VertexTag::Curve });
		_vertices.emplace_back(vertex_t{ x, y + radius, VertexTag::Curve });
		_vertices.emplace_back(vertex_t{ x - radius, y, VertexTag::Curve });

	}
	void GraphicsPath::AddLine(float x1, float y1, float x2, float y2) {

		_vertices.emplace_back(vertex_t{ x1, y1, VertexTag::LineSegment });
		_vertices.emplace_back(vertex_t{ x2, y2, VertexTag::LineSegment });

	}
	void GraphicsPath::AddPoint(float x, float y, VertexTag tag) {

		_vertices.emplace_back(vertex_t{ x, y, tag });

	}
	void GraphicsPath::AddRectangle(float x, float y, float width, float height) {

		_vertices.emplace_back(vertex_t{ x, y, VertexTag::Polygon });
		_vertices.emplace_back(vertex_t{ x + width, y, VertexTag::Polygon });
		_vertices.emplace_back(vertex_t{ x + width, y + height, VertexTag::Polygon });
		_vertices.emplace_back(vertex_t{ x, y + height, VertexTag::Polygon });

	}
	RectangleF GraphicsPath::BoundingBox() const {
	
		if (_vertices.size() <= 0)
			return RectangleF(0.0f, 0.0f, 0.0f, 0.0f);

		float min_x = _vertices[0].x;
		float max_x = _vertices[0].x;
		float min_y = _vertices[0].y;
		float max_y = _vertices[0].y;

		for (auto i = ++_vertices.begin(); i != _vertices.end(); ++i) {

			if (i->x < min_x)
				min_x = i->x;
			else if (i->x > max_x)
				max_x = i->x;

			if (i->y < min_y)
				min_y = i->y;
			else if (i->y > max_y)
				max_y = i->y;

		}

		return RectangleF(PointF(min_x, min_y), PointF(max_x, max_y));

	}

}