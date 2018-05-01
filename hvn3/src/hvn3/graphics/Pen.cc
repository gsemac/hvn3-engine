#include "hvn3/graphics/Pen.h"

namespace hvn3 {
	namespace Graphics {

		Pen::Pen(const hvn3::Color& color) :
			Pen(color, 1.0f) {
		}
		Pen::Pen(const hvn3::Color& color, float width) :
			_color(color),
			_width(width) {
			_dash_style = DashStyle::Solid;
		}

		const Color& Pen::Color() const {
			return _color;
		}
		void Pen::SetColor(const hvn3::Color& value) {
			_color = value;
		}
		float Pen::Width() const {
			return _width;
		}
		void Pen::SetWidth(float width) {
			_width = width;
		}
		DashStyle Pen::DashStyle() const {
			return _dash_style;
		}
		void Pen::SetDashStyle(Graphics::DashStyle value) {
			_dash_style = value;
		}
		const std::vector<float>& Pen::DashPattern() const {
			return _dash_pattern;
		}
		void Pen::SetDashPattern(std::initializer_list<float> pattern) {

			_dash_pattern.clear();
			_dash_pattern.reserve(pattern.size());

			for (auto i = pattern.begin(); i != pattern.end(); ++i)
				_dash_pattern.push_back(*i);

			SetDashStyle(DashStyle::Dash);

		}

	}
}