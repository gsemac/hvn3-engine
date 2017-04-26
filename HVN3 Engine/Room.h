#ifndef __SCENE_H
#define __SCENE_H
#include <list>
#include <memory>
#include <vector>
#include "Point.h"
#include "IUpdatable.h"
#include "Background.h"
#include "Size.h"
#include "View.h"
#include "CollisionGrid.h"
#include "CollisionManager.h"
#include "Graphics.h"
#include "UpdateEventArgs.h"
#include "ObjectManager.h"
#include "RoomEnterEventArgs.h"
#include "RoomExitEventArgs.h"

class Object;
class Runner;

typedef int RoomId;

class Room : public IUpdatable, public IDrawable, public ISizeable {
	friend class RoomController;

public:
	class BackgroundProperties {
		friend class Room;

	public:
		BackgroundProperties(std::shared_ptr<::Background> background);
		
		const std::shared_ptr<::Background>& Background() const;

		const Point& Offset() const;
		void SetOffset(float x_offset, float y_offset);
		const Point& Scale() const;
		void SetScale(float x_scale, float y_scale);
		const Vector2d& Velocity() const;
		void SetVelocity(const Vector2d& velocity);

		bool IsForeground() const;
		void SetForeground(bool is_foreground);
		bool IsTiledHorizontally() const;
		void SetTiledHorizontally(bool tile_horizontally);
		bool IsTiledVertically() const;
		void SetTiledVertically(bool tile_vertically);
		bool Visible() const;
		void SetVisible(bool visible);
		bool Fixed() const;
		void SetFixed(bool fixed);

		void ScaleToFit();

	private:
		std::shared_ptr<::Background> __background;
		Point __offset;
		Point __scale;
		Vector2d __velocity;
		bool __foreground;
		bool __tile_h, __tile_v;
		bool __visible;
		Room* __in_scene;
		bool __fixed;

	};

	Room(unsigned int width, unsigned int height);

	void Update(UpdateEventArgs& e) override;
	void Draw(DrawEventArgs& e) override;

	void SetBackgroundColor(int, int, int);
	void SetBackgroundColor(const Color& color);

	// Returns the View at the specified index.
	View& View(int index);
	// Adds a new View to the Scene and returns its index.
	int AddView(const ::View& view);
	// Returns the number of Views in the Scene.
	int ViewCount();
	// Returns the index of the View currently being rendered.
	const ::View& CurrentView() const;

	// Returns the BackgroundProperties object at the specified index.
	BackgroundProperties& Background(int index);
	// Adds a new Background to the Scene and returns its index.
	int AddBackground(std::shared_ptr<::Background> background);
	// Returns the number of Backgrounds in the Scene.
	int BackgroundCount();

	Size Size() const;

	ObjectManager& ObjectManager();

	virtual ::RoomId Id() const;
	bool Persistent() const;
	void SetPersistent(bool value);

	template<typename T, typename ... Args>
	static std::unique_ptr<Room> Create(Args &&... args) {

		return std::make_unique<T>(std::forward<Args>(args)...);

	}

protected:
	virtual void OnRoomEnter(RoomEnterEventArgs& e);
	virtual void OnRoomExit(RoomExitEventArgs& e);

	// Sets up the room by initializing all objects and resources.
	virtual void SetUp();

private:
	// Clears the object manager and collision manager, as well as all views and backgrounds.
	void Reset();

	::ObjectManager _obj_manager;

	std::vector<BackgroundProperties> __backgrounds;
	Color __background_color;

	std::vector<::View> __views;
	int __current_view;

	bool _set_up;
	bool _persistent;

	void DrawBackground(Drawing::Graphics& graphics, const BackgroundProperties& background, ::View* view);
	void UpdateViews();

};

#endif