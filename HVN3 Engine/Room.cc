#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <cassert>
#include "Room.h"
#include "Background.h"
#include "Object.h"
#include "Graphics.h"
#include "Mouse.h"
#include "DrawEventArgs.h"
#include "Exception.h"
#define CELL_DIMENSIONS 32

// Virtual members

void Room::Build() {}
void Room::Restart() {

	__restart_pending = true;



}

// Protected members

void Room::Reset() {

	// Clear the list of Objects.
	__objects.clear();

	// Clear the list of Backgrounds.
	__backgrounds.clear();

	// Clear the list of Views.
	__views.clear();

	// Reset members to default values.
	__background_color = Color::Black;
	__current_view = 0;

	// Todo: Reset the collision management members.
	delete __broadphase_handler;
	__broadphase_handler = nullptr;
	__broadphase_handler = new CollisionGrid(16, 16);
	__collision_manager = ::CollisionManager(__broadphase_handler);

}
void Room::Rebuild() {

	Reset();
	Build();

}

Room::Room(unsigned int width, unsigned int height) :
	Room(width, height, new CollisionGrid(Size(CELL_DIMENSIONS, CELL_DIMENSIONS))) {}
Room::Room(unsigned int width, unsigned int height, IBroadphase* broadphase_handler) :
	ISizeable(width, height),
	__collision_manager(broadphase_handler) {

	// Set the default background color to black.
	__background_color = Color::Black;

	// The current View is 0 because no views have been added yet. When drawing occurs, this value corresponds to the view currently being drawn.
	__current_view = 0;
	__restart_pending = false;

	// Set the broadphase handler, which handles broadphase collision detection. The Scene stores this (even though it doesn't use it directly) to delete it when the Scene is destructed.
	__broadphase_handler = broadphase_handler;

}
Room::~Room() {

	// Free the broadphase handler object.
	if (__broadphase_handler)
		delete __broadphase_handler;
	__broadphase_handler = nullptr;

}
void Room::Update(UpdateEventArgs& e) {

	// Run the pre-update procedure for all Objects.
	for (auto it = __objects.begin(); it != __objects.end(); ++it)
		(*it)->BeginUpdate(e);

	// Run the primary update procedure for all Objects.
	for (auto it = __objects.begin(); it != __objects.end(); ++it)
		(*it)->Update(e);

	// Update the Collision Manager.
	__collision_manager.Update();

	// Run the post update procedure for all Objects.
	for (auto it = __objects.begin(); it != __objects.end(); ++it)
		(*it)->EndUpdate(e);

	// If a restart is pending, perform the restart now.
	if (__restart_pending) {
		Rebuild();
		__restart_pending = false;
	}

	// Update Backgrounds.
	for (size_t i = 0; i < __backgrounds.size(); ++i)
		if (__backgrounds[i].Velocity().Magnitude() != 0.0f)
			__backgrounds[i].SetOffset(__backgrounds[i].Offset().X() + __backgrounds[i].Velocity().X(), __backgrounds[i].Offset().Y() + __backgrounds[i].Velocity().Y());

	// Update views.
	UpdateViews();

}
void Room::Draw(DrawEventArgs& e) {

	//Stopwatch watch(true);

	// Each View needs to be drawn separately to improve the appearance of scaled Bitmaps.
	if (__views.size() > 0) {

		// Save the original state.
		Drawing::Transform original_tranform(e.Graphics().GetTransform());
		Rectangle original_clip(e.Graphics().Clip());

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
			e.Graphics().SetClip(Rectangle(p1, p2));

			// Clear to background color.
			e.Graphics().Clear(__background_color);

			// Set transform according to view state.		
			Point offset(view.ViewX(), view.ViewY());
			original_tranform.TransformPoint(offset);
			Drawing::Transform transform(original_tranform);
			transform.Translate(-offset.X() + p1.X(), -offset.Y() + p1.Y());
			e.Graphics().SetTransform(transform);

			// Draw all Backgrounds (foregrounds are skipped for now).
			e.Graphics().HoldBitmapDrawing(true);
			for (size_t i = 0; i < __backgrounds.size(); ++i)
				if (!__backgrounds[i].IsForeground() && __backgrounds[i].Visible())
					DrawBackground(e.Graphics(), __backgrounds[i], Point(view.ViewX(), view.ViewY()));
			e.Graphics().HoldBitmapDrawing(false);

			// Draw all Objects.
			for (auto it = __objects.begin(); it != __objects.end(); ++it)
				(*it)->Draw(e);

			// Draw all Foregrounds.
			e.Graphics().HoldBitmapDrawing(true);
			for (size_t i = 0; i < __backgrounds.size(); ++i)
				if (__backgrounds[i].IsForeground() && __backgrounds[i].Visible())
					DrawBackground(e.Graphics(), __backgrounds[i], Point(view.ViewX(), view.ViewY()));
			e.Graphics().HoldBitmapDrawing(false);

		}

		// Restore the previous transform.
		e.Graphics().SetTransform(original_tranform);

		// Reset the clipping region.
		e.Graphics().SetClip(original_clip);

		// Reset current view to 0.
		__current_view = 0;

	}
	else {

		// Clear to background color.
		e.Graphics().Clear(__background_color);

		// Draw all Backgrounds (foregrounds are skipped for now).
		for (size_t i = 0; i < __backgrounds.size(); ++i)
			if (!__backgrounds[i].IsForeground() && __backgrounds[i].Visible())
				DrawBackground(e.Graphics(), __backgrounds[i], Point(0.0f, 0.0f));

		// Save the state of the Graphics object in case any of the Draw calls modify it.
		Drawing::GraphicsState orig = e.Graphics().Save();

		// If no Views are used, simply draw all of the Objects with normal scaling.
		for (auto it = __objects.begin(); it != __objects.end(); ++it)
			(*it)->Draw(e);

		// Draw all Foregrounds.
		for (size_t i = 0; i < __backgrounds.size(); ++i)
			if (__backgrounds[i].IsForeground() && __backgrounds[i].Visible())
				DrawBackground(e.Graphics(), __backgrounds[i], Point(0.0f, 0.0f));

	}

}
void Room::SetBackgroundColor(int r, int g, int b) {

	__background_color = Color(r, g, b);

}
void Room::SetBackgroundColor(const Color& color) {

	__background_color = color;

}
void Room::AddInstance(Object* object) {

	AddInstance(object, object->X(), object->Y());

}
void Room::AddInstance(Object* object, float x, float y) {

	std::shared_ptr<Object> ptr(object);
	AddInstance(ptr, x, y);

}
void Room::AddInstance(std::shared_ptr<Object> object) {

	AddInstance(object, object->X(), object->Y());

}
void Room::AddInstance(std::shared_ptr<Object> object, float x, float y) {

	// Set the Object's parent Scene to this Scene.
	object->__room = this;

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
void Room::AddInstance(ObjectBase* object) {

	throw NotImplementedException();

}
View& Room::View(int index) {

	// Return the View at the specified index.
	return __views[index];

}
int Room::AddView(const ::View& view) {

	__views.push_back(view);
	return (int)__views.size() - 1;

}
int Room::ViewCount() {

	return (int)__views.size();

}
int Room::CurrentView() {

	return __current_view;

}
Room::BackgroundProperties& Room::Background(int index) {
	assert(index >= 0 && index < (int)__backgrounds.size() && "Background index is out of bounds.");

	return __backgrounds[index];

}
int Room::AddBackground(std::shared_ptr<::Background> background) {

	BackgroundProperties bgp(background);
	bgp.__in_scene = this;

	__backgrounds.push_back(bgp);

	return (int)__backgrounds.size() - 1;

}
int Room::BackgroundCount() {

	return (int)__backgrounds.size();

}
CollisionManager& Room::CollisionManager() {

	return __collision_manager;

}

void Room::DrawBackground(Drawing::Graphics& graphics, const BackgroundProperties& background, const Point& view_offset) {

	// Get a reference to the pointer to the background we're going to be drawing.
	const std::shared_ptr<::Background>& bg = background.Background();

	// Calculate scaled dimensions of the background.
	float scale_x = background.Scale().X();
	float scale_y = background.Scale().Y();
	float width = bg->Width() * (std::abs)(scale_x);
	float height = bg->Height() * (std::abs)(scale_y);

	// If either dimension is 0, don't draw.
	if (IsZero(width, 0.1f) || IsZero(height, 0.1f))
		return;

	// Calculate the offset of the background. If the background is tiled, this is the starting offset.
	Point offset = background.Offset();

	// If the background is fixed, counteract this by the view offset.
	if (background.Fixed())
		offset += view_offset;

	if (background.IsTiledHorizontally())
		// Subtract the width of the background from the offset until it is "0" or negative.
		while (offset.X() > 0.1f) offset.TranslateX(-width);

	if (background.IsTiledVertically())
		// Subtract the height of the background from the offset until it is "0" or negative.
		while (offset.Y() > 0.1f) offset.TranslateY(-height);

	// If the scale is negative, adjust the offset so that the tiles are drawn in the proper location (it will be flipped over axis).
	if (scale_x < 0.0f) offset.TranslateX(width);
	if (scale_y < 0.0f) offset.TranslateY(height);

	if (background.IsTiledHorizontally() && background.IsTiledVertically())
		// Draw background tiled horizontally and vertically.
		for (; offset.X() < (scale_x < 0.0f ? Width() + width : Width()); offset.TranslateX(width))
			for (float j = offset.Y(); j < ((scale_y < 0.0f) ? (Height() + height) : Height()); j += height)
				graphics.DrawBitmap(bg->Bitmap(), offset.X(), j, background.Scale().X(), background.Scale().Y());
	else if (background.IsTiledHorizontally())
		// Draw background tiled horizontally only.
		for (; offset.X() < (scale_x < 0.0f ? Width() + width : Width()); offset.TranslateX(width))
			graphics.DrawBitmap(bg->Bitmap(), offset.X(), offset.Y(), background.Scale().X(), background.Scale().Y());
	else if (background.IsTiledVertically())
		// Draw background tiled vertically only.
		for (; offset.Y() < (scale_y < 0.0f ? Height() + height : Height()); offset.TranslateY(height))
			graphics.DrawBitmap(bg->Bitmap(), offset.X(), offset.Y(), background.Scale().X(), background.Scale().Y());
	else
		// Draw background without tiling.
		graphics.DrawBitmap(bg->Bitmap(), offset.X(), offset.Y(), background.Scale().X(), background.Scale().Y());

}
void Room::UpdateViews() {

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

Room::BackgroundProperties::BackgroundProperties(std::shared_ptr<::Background> background) :
	__background(background),
	__offset(0.0f, 0.0f),
	__scale(1.0f, 1.0f),
	__velocity(0.0f, 0.0f) {

	__foreground = false;
	__tile_h = false;
	__tile_v = false;
	__visible = true;
	__fixed = false;
	__in_scene = nullptr;

}
const std::shared_ptr<::Background>& Room::BackgroundProperties::Background() const {

	return __background;

}
const Point& Room::BackgroundProperties::Offset() const {

	return __offset;

}
void Room::BackgroundProperties::SetOffset(float x_offset, float y_offset) {

	__offset.SetXY(x_offset, y_offset);

}
const Point& Room::BackgroundProperties::Scale() const {

	return __scale;

}
void Room::BackgroundProperties::SetScale(float x_scale, float y_scale) {

	__scale.SetXY(x_scale, y_scale);

}
const Vector2d& Room::BackgroundProperties::Velocity() const {

	return __velocity;

}
void Room::BackgroundProperties::SetVelocity(const Vector2d& velocity) {

	__velocity = velocity;

}
bool Room::BackgroundProperties::IsForeground() const {

	return __foreground;

}
void Room::BackgroundProperties::SetForeground(bool is_foreground) {

	__foreground = is_foreground;

}
bool Room::BackgroundProperties::IsTiledHorizontally() const {

	return __tile_h;

}
void Room::BackgroundProperties::SetTiledHorizontally(bool tile_horizontally) {

	__tile_h = tile_horizontally;

}
bool Room::BackgroundProperties::IsTiledVertically() const {

	return __tile_v;

}
void Room::BackgroundProperties::SetTiledVertically(bool tile_vertically) {

	__tile_v = tile_vertically;

}
bool Room::BackgroundProperties::Visible() const {

	return __visible;

}
void Room::BackgroundProperties::SetVisible(bool visible) {

	__visible = visible;

}
bool Room::BackgroundProperties::Fixed() const {

	return __fixed;

}
void Room::BackgroundProperties::SetFixed(bool fixed) {

	__fixed = fixed;

}
void Room::BackgroundProperties::ScaleToFit() {

	__scale.SetXY(__in_scene->Width() / __background->Width(), __in_scene->Height() / __background->Height());


}