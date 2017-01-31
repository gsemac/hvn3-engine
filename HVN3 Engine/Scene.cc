#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <cassert>
#include "Scene.h"
#include "Background.h"
#include "Object.h"
#include "Graphics.h"
#include "Mouse.h"

#define CELL_DIMENSIONS 32

Scene::Scene(unsigned int width, unsigned int height) :
	Scene(width, height, new CollisionGrid(Size(CELL_DIMENSIONS, CELL_DIMENSIONS))) {}
Scene::Scene(unsigned int width, unsigned int height, IBroadphase* broadphase_handler) :
	ISizeable(width, height),
	__collision_manager(broadphase_handler) {

	// Set the default background color to black.
	__background_color = Color::Black;

	// The current View is 0 because no views have been added yet.
	// When drawing occurs, this value corresponds to the view currently being drawn.
	__current_view = 0;

	// Set the broadphase handler, which handles broadphase collision detection.
	// The Scene stores this (even though it doesn't use it directly) to delete it when the Scene is destructed.
	__broadphase_handler = broadphase_handler;

}
Scene::~Scene() {

	// Free the broadphase handler object.
	if (__broadphase_handler)
		delete __broadphase_handler;
	__broadphase_handler = nullptr;

}
void Scene::Update(float dt) {

	// Update all Objects in the Scene.
	for (auto it = __objects.begin(); it != __objects.end(); ++it)
		(*it)->Update(dt);

	// Update the Collision Manager.
	__collision_manager.Update();

	// Update Backgrounds.
	for (size_t i = 0; i < __backgrounds.size(); ++i)
		if (__backgrounds[i].Velocity().Magnitude() != 0.0f)
			__backgrounds[i].SetOffset(__backgrounds[i].Offset().X() + __backgrounds[i].Velocity().X(), __backgrounds[i].Offset().Y() + __backgrounds[i].Velocity().Y());

	// Update views.
	UpdateViews();

}
void Scene::Draw(Drawing::Graphics& graphics) {

	// Each View needs to be drawn separately to improve the appearance of scaled Bitmaps.
	if (__views.size() > 0) {

		// Save the original transform.
		Drawing::Transform original_tranform(graphics.GetTransform());
		Rectangle original_clip(graphics.Clip());

		for (size_t i = 0; i < __views.size(); ++i) {

			// Set current view index.
			__current_view = static_cast<int>(i);

			// Get a reference to the View.
			::View& view = __views[i];

			// If the View isn't enabled, do nothing.
			if (!view.Enabled())
				continue;
			
			// Set the clipping region according to the view port.
			Point p1 = view.Port().TopLeft();
			Point p2 = view.Port().BottomRight();
			original_tranform.TransformPoint(p1);
			original_tranform.TransformPoint(p2);
			graphics.SetClip(Rectangle(p1, p2));

			// Clear to background color.
			graphics.Clear(__background_color);

			// Set transform according to view state.
			Drawing::Transform transform(original_tranform);
			transform.Translate(-view.ViewX(), -view.ViewY());
			transform.Scale(view.ScaleX(), view.ScaleY());
			transform.Rotate(view.Port().Midpoint(), view.Angle());
			graphics.SetTransform(transform);

			// Draw all Backgrounds (foregrounds are skipped for now).
			for (size_t i = 0; i < __backgrounds.size(); ++i)
				if (!__backgrounds[i].IsForeground() && __backgrounds[i].Visible())
					DrawBackground(graphics, __backgrounds[i]);

			// Draw all Objects.
			for (auto it = __objects.begin(); it != __objects.end(); ++it)
				(*it)->Draw(graphics);

			// Draw all Foregrounds.
			for (size_t i = 0; i < __backgrounds.size(); ++i)
				if (__backgrounds[i].IsForeground() && __backgrounds[i].Visible())
					DrawBackground(graphics, __backgrounds[i]);

		}

		// Restore the previous transform.
		graphics.SetTransform(original_tranform);

		// Reset the clipping region.
		graphics.SetClip(original_clip);

		// Reset current view to 0.
		__current_view = 0;

	}
	else {

		// Clear to background color.
		graphics.Clear(__background_color);

		// Draw all Backgrounds (foregrounds are skipped for now).
		for (size_t i = 0; i < __backgrounds.size(); ++i)
			if (!__backgrounds[i].IsForeground() && __backgrounds[i].Visible())
				DrawBackground(graphics, __backgrounds[i]);

		// If no Views are used, simply draw all of the Objects with normal scaling.
		for (auto it = __objects.begin(); it != __objects.end(); ++it)
			(*it)->Draw(graphics);

		// Draw all Foregrounds.
		for (size_t i = 0; i < __backgrounds.size(); ++i)
			if (__backgrounds[i].IsForeground() && __backgrounds[i].Visible())
				DrawBackground(graphics, __backgrounds[i]);

	}

}
void Scene::SetBackgroundColor(int r, int g, int b) {

	__background_color = Color(r, g, b);

}
void Scene::SetBackgroundColor(const Color& color) {

	__background_color = color;

}
void Scene::AddObject(Object* object) {

	AddObject(object, object->X(), object->Y());

}
void Scene::AddObject(Object* object, float x, float y) {

	std::shared_ptr<Object> ptr(object);
	AddObject(ptr, x, y);

}
void Scene::AddObject(std::shared_ptr<Object> object) {

	AddObject(object, object->X(), object->Y());

}
void Scene::AddObject(std::shared_ptr<Object> object, float x, float y) {

	// Set the Object's parent Scene to this Scene.
	object->__scene = this;

	// Set the Object's position.
	object->SetXY(x, y);

	// Add the object to the collision manager.
	CollisionManager().Broadphase().Add(object.get());

	// If there are no objects in the list, just insert the new object.
	if (__objects.size() == 0) {
		__objects.push_back(object);
		return;
	}

	// Get the highest and lowest depths in the list.
	int lowest_depth = __objects.back()->Depth();
	int highest_depth = __objects.front()->Depth();

	if (object->Depth() <= lowest_depth) {
		// If the object's depth is <= than the lowest depth, insert last.
		__objects.push_back(object);
	}
	else if (object->Depth() >= highest_depth) {
		// If the objects depth is >= the highest depth, insert first.
		__objects.push_front(object);
	}
	else {
		// Find a proper position for the object according to its depth.
		for (auto it = __objects.begin(); it != __objects.end(); ++it) {
			if ((*it)->Depth() < object->Depth()) {
				__objects.insert(it, object);
				return;
			}
		}
		__objects.push_back(object);
	}

}
View& Scene::View(int index) {

	// Return the View at the specified index.
	return __views[index];

}
int Scene::AddView(const ::View& view) {

	__views.push_back(view);
	return (int)__views.size() - 1;

}
int Scene::ViewCount() {

	return (int)__views.size();

}
int Scene::CurrentView() {

	return __current_view;

}
Scene::BackgroundProperties& Scene::Background(int index) {
	assert(index >= 0 && index < (int)__backgrounds.size() && "Background index is out of bounds.");

	return __backgrounds[index];

}
int Scene::AddBackground(std::shared_ptr<::Background> background) {

	BackgroundProperties bgp(background);
	bgp.__in_scene = this;

	__backgrounds.push_back(bgp);

	return (int)__backgrounds.size() - 1;

}
int Scene::BackgroundCount() {

	return (int)__backgrounds.size();

}
CollisionManager& Scene::CollisionManager() {

	return __collision_manager;

}

void Scene::DrawBackground(Drawing::Graphics& graphics, const BackgroundProperties& background) {

	// Get a referen ce to the pointer to the background we're going to be drawing.
	const std::shared_ptr<::Background>& bg = background.Ptr();

	// Calculate scaled dimensions of the background.
	float scale_x = background.Scale().X();
	float scale_y = background.Scale().Y();
	float width = bg->Width() * (std::abs)(scale_x);
	float height = bg->Height() * (std::abs)(scale_y);

	// If either dimension is 0, don't draw.
	if (IsZero(width, 0.1f) || IsZero(height, 0.1f))
		return;

	// Calculate the offset of the background. If the background is tiled, this is the starting offset.
	float offset_x = background.Offset().X();
	float offset_y = background.Offset().Y();

	if (background.IsTiledHorizontally())
		// Subtract the width of the background from the offset until it is "0" or negative.
		while (offset_x > 0.1f) offset_x -= width;

	if (background.IsTiledVertically())
		// Subtract the height of the background from the offset until it is "0" or negative.
		while (offset_y > 0.1f) offset_y -= height;

	// If the scale is negative, adjust the offset so that the tiles are drawn in the proper location (it will be flipped over axis).
	if (scale_x < 0.0f) offset_x += width;
	if (scale_y < 0.0f) offset_y += height;

	graphics.HoldBitmapDrawing(true);
	if (background.IsTiledHorizontally() && background.IsTiledVertically())
		// Draw background tiled horizontally and vertically.
		for (; offset_x < (scale_x < 0.0f ? Width() + width : Width()); offset_x += width)
			for (float j = offset_y; j < ((scale_y < 0.0f) ? (Height() + height) : Height()); j += height)
				graphics.DrawBitmap(bg->Bitmap(), offset_x, j, background.Scale().X(), background.Scale().Y());
	else if (background.IsTiledHorizontally())
		// Draw background tiled horizontally only.
		for (; offset_x < (scale_x < 0.0f ? Width() + width : Width()); offset_x += width)
			graphics.DrawBitmap(bg->Bitmap(), offset_x, offset_y, background.Scale().X(), background.Scale().Y());
	else if (background.IsTiledVertically())
		// Draw background tiled vertically only.
		for (; offset_y < (scale_y < 0.0f ? Height() + height : Height()); offset_y += height)
			graphics.DrawBitmap(bg->Bitmap(), offset_x, offset_y, background.Scale().X(), background.Scale().Y());
	else
		// Draw background without tiling.
		graphics.DrawBitmap(bg->Bitmap(), offset_x, offset_y, background.Scale().X(), background.Scale().Y());
	graphics.HoldBitmapDrawing(false);

}
void Scene::UpdateViews() {

	// Keep track of whether a view has already taken control of the mouse (to prevent multiple views from doing so).
	bool mouse_taken = false;

	for (int i = ViewCount() - 1; i >= 0; --i) {

		// Initialize variables.	
		::View& view = View(i);
		Object* obj = view.GetFollowing();
		bool has_mouse = !mouse_taken && view.HasMouse();
		if (has_mouse) mouse_taken = true;

		// If the View isn't following an Object, or is disabled, there's nothing to do.
		if (!obj || !view.Enabled()) continue;

		// Calculate the distance of the Object from the center of the view (to compare with borders).
		float diff_x = (view.ViewX() + view.Region().Width() / 2.0f) - obj->X();
		float diff_y = (view.ViewY() + view.Region().Height() / 2.0f) - obj->Y();

		// Check for overlap in view horizonal view border.
		if ((std::abs)(diff_x) > (view.Region().Width() / 2.0f - view.HorizontalBorder())) {

			// Calculate the amount that the view has to shift by.
			float diff = (view.HorizontalBorder() - ((view.Region().Width() / 2.0f) - (std::abs)(diff_x))) * Sign(diff_x);

			// Make sure the View doesn't shift outside of the room boundaries.
			diff = Clamp(diff, -(Dimensions().Width() - view.Region().Width() - view.ViewPosition().X()), view.ViewPosition().X());

			// Adjust View position.
			view.ViewPosition().TranslateX(-diff);

		}

		// Check for overlap in view horizonal view border.
		if ((std::abs)(diff_y) > (view.Region().Height() / 2.0f - view.VerticalBorder())) {

			// Calculate the amount that the view has to shift by.
			float diff = (view.VerticalBorder() - ((view.Region().Height() / 2.0f) - (std::abs)(diff_y))) * Sign(diff_y);

			// Make sure the View doesn't shift outside of the room boundaries.
			diff = Clamp(diff, -(Dimensions().Height() - view.Region().Height() - view.ViewPosition().Y()), view.ViewPosition().Y());

			// Adjust View/mouse position.
			view.ViewPosition().TranslateY(-diff);

		}

		// Adjust mouse position (if applicable).
		if (has_mouse) {
			Point pos = view.MousePosition();
			Mouse::X = pos.X();
			Mouse::Y = pos.Y();
		}

	}

}

Scene::BackgroundProperties::BackgroundProperties(std::shared_ptr<::Background> background) :
	__background(background),
	__offset(0.0f, 0.0f),
	__scale(1.0f, 1.0f),
	__velocity(0.0f, 0.0f) {

	__foreground = false;
	__tile_h = false;
	__tile_v = false;
	__visible = true;
	__in_scene = nullptr;

}
const std::shared_ptr<::Background>& Scene::BackgroundProperties::Ptr() const {

	return __background;

}
const Point& Scene::BackgroundProperties::Offset() const {

	return __offset;

}
void Scene::BackgroundProperties::SetOffset(float x_offset, float y_offset) {

	__offset.SetXY(x_offset, y_offset);

}
const Point& Scene::BackgroundProperties::Scale() const {

	return __scale;

}
void Scene::BackgroundProperties::SetScale(float x_scale, float y_scale) {

	__scale.SetXY(x_scale, y_scale);

}
const Vector2d& Scene::BackgroundProperties::Velocity() const {

	return __velocity;

}
void Scene::BackgroundProperties::SetVelocity(const Vector2d& velocity) {

	__velocity = velocity;

}
bool Scene::BackgroundProperties::IsForeground() const {

	return __foreground;

}
void Scene::BackgroundProperties::SetForeground(bool is_foreground) {

	__foreground = is_foreground;

}
bool Scene::BackgroundProperties::IsTiledHorizontally() const {

	return __tile_h;

}
void Scene::BackgroundProperties::SetTiledHorizontally(bool tile_horizontally) {

	__tile_h = tile_horizontally;

}
bool Scene::BackgroundProperties::IsTiledVertically() const {

	return __tile_v;

}
void Scene::BackgroundProperties::SetTiledVertically(bool tile_vertically) {

	__tile_v = tile_vertically;

}
bool Scene::BackgroundProperties::Visible() const {

	return __visible;

}
void Scene::BackgroundProperties::SetVisible(bool visible) {

	__visible = visible;

}
void Scene::BackgroundProperties::ScaleToFit() {

	__scale.SetXY(__in_scene->Width() / __background->Width(), __in_scene->Height() / __background->Height());


}