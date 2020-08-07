#include "graphics/graphics.h"

namespace hvn3::graphics {

	ALLEGRO_COLOR to_allegro_color(const Color& color) {

		return al_map_rgba_f(color.Rf() * color.Af(), color.Gf() * color.Af(), color.Bf() * color.Af(), color.Af());

	}

	// Public members

	Graphics::Graphics(const Bitmap& canvas) {

		this->canvas = canvas;

	}

	//void Graphics::DrawPixel(float x, float y, const Color& color) {
	//}

	//void Graphics::DrawLine(const math::LineF& line, const IPen& pen) {
	//}

	//void Graphics::DrawRectangle(const math::RectangleF& rectangle, const IPen& pen) {
	//}
	//void Graphics::FillRectangle(const math::RectangleF& rectangle, const Color& color) {
	//}

	//void Graphics::DrawRoundedRectangle(const math::RectangleF& rectangle, float radius, const IPen& pen) {
	//}
	//void Graphics::FillRoundedRectangle(const math::RectangleF& rectangle, float radius, const Color& color) {
	//}

	//void Graphics::DrawCircle(float x, float y, float radius, const Color& color, float thickness) {
	//}
	//void Graphics::FillCircle(float x, float y, float radius, const Color& color) {
	//}

	//void Graphics::DrawText(float x, float y, const char* text, const Font& font, const Color& color, Alignment alignment) {
	//}

	//void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, float scaleX, float scaleY, float originX, float originY, float angle, const Color& tint) {
	//}

	void Graphics::Clear(const Color& color) {

		PrepareCanvas(true);

		al_clear_to_color(to_allegro_color(color));

	}
	//void Graphics::Fill(const Color& color) {
	//}

	//math::RectangleI Graphics::Clip() const {

	//	return math::RectangleI(0, 0, 0, 0);

	//}
	//void Graphics::SetClip(int x, int y, int width, int height) {
	//}
	//void Graphics::ResetClip() {
	//}

	//class Transform Graphics::Transform() const {

	//	return class Transform();

	//}
	//void Graphics::SetTransform(const class  Transform& transform) {
	//}
	//void Graphics::ResetTransform() {
	//}

	// Private members

	Graphics* Graphics::lastToDraw = nullptr;

	void Graphics::PrepareCanvas(bool isWriting) {

		if (!IsCanvasReady()) {

			al_set_target_bitmap(canvas.GetUnderlyingData(isWriting));

			/*		_applyClip();
					_applyTransform();*/

			lastToDraw = this;

		}

	}
	bool Graphics::IsCanvasReady() {

		return lastToDraw == this;

	}

}