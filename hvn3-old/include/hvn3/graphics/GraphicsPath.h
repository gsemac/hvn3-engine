#pragma once

#include "hvn3/fonts/Font.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/utility/Utf8String.h"

#include <string>
#include <vector>

namespace hvn3 {

	class GraphicsPath {

	public:
		enum class VertexTag {
			Point,
			LineSegment,
			Polygon,
			Curve
		};

		void AddCircle(float x, float y, float radius);
		void AddLine(float x1, float y1, float x2, float y2);
		void AddPoint(float x, float y, VertexTag tag);
		template<typename IteratorT>
		void AddPoints(IteratorT begin, IteratorT end, VertexTag tag);
		void AddRectangle(float x, float y, float width, float height);

		RectangleF BoundingBox() const;

	private:
		struct vertex_t {
			float x;
			float y;
			VertexTag tag;
		};

		std::vector<vertex_t> _vertices;

	};

	template<typename IteratorT>
	void GraphicsPath::AddPoints(IteratorT begin, IteratorT end, VertexTag tag) {

		for (auto i = begin; i != end; ++i)
			AddPoint(i->x, i->y, tag);

	}

}