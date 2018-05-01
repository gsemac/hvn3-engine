#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/graphics/GraphicsTypeDefs.h"
#include <initializer_list>
#include <vector>

namespace hvn3 {
	namespace Graphics {

		class Pen {

		public:
			Pen(const Color& color);
			Pen(const Color& color, float width);

			const Color& Color() const;
			void SetColor(const hvn3::Color& value);
			float Width() const;
			void SetWidth(float width);
			DashStyle DashStyle() const;
			void SetDashStyle(hvn3::Graphics::DashStyle value);
			const std::vector<float>& DashPattern() const;
			void SetDashPattern(std::initializer_list<float> pattern);

		private:
			hvn3::Color _color;
			float _width;
			hvn3::Graphics::DashStyle _dash_style;
			std::vector<float> _dash_pattern;


		};

	}
}