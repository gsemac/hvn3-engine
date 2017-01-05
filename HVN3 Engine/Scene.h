#ifndef __SCENE_H
#define __SCENE_H
#include <list>
#include <memory>
#include <vector>
#include "Point.h"
#include "IDrawable.h"
#include "IUpdatable.h"
#include "Background.h"
#include "Size.h"
#include "View.h"
#include "CollisionGrid.h"
#include "CollisionManager.h"

class Object;

class Scene : public IDrawable, public IUpdatable, public ISizeable {

public:
	class BackgroundProperties {
		friend class Scene;

	public:
		BackgroundProperties(std::shared_ptr<::Background> background);
		
		const std::shared_ptr<::Background>& Ptr() const;

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

		void ScaleToFit();

	private:
		std::shared_ptr<::Background> __background;
		Point __offset;
		Point __scale;
		Vector2d __velocity;
		bool __foreground;
		bool __tile_h, __tile_v;
		bool __visible;
		Scene* __in_scene;
	};
	
	Scene(unsigned int width, unsigned int height);
	Scene(unsigned int width, unsigned int height, IBroadphase* broadphase_handler);
	~Scene();

	void Update(float dt) override;
	void Draw() override;

	void SetBackgroundColor(int, int, int);
	void SetBackgroundColor(const Color& color);

	void AddObject(Object* object);
	void AddObject(Object* object, float x, float y);
	void AddObject(std::shared_ptr<Object> object);
	void AddObject(std::shared_ptr<Object> object, float x, float y);

	// Returns the View at the specified index.
	View& View(int index);
	// Adds a new View to the Scene and returns its index.
	int AddView(const ::View& view);
	// Returns the number of Views in the Scene.
	int ViewCount();
	// Returns the index of the View currently being rendered.
	int CurrentView();

	// Returns the BackgroundProperties object at the specified index.
	BackgroundProperties& Background(int index);
	// Adds a new Background to the Scene and returns its index.
	int AddBackground(std::shared_ptr<::Background> background);
	// Returns the number of Backgrounds in the Scene.
	int BackgroundCount();

	CollisionManager& CollisionManager();

private:
	std::list<std::shared_ptr<Object>> __objects;

	std::vector<BackgroundProperties> __backgrounds;
	Color __background_color;

	std::vector<::View> __views;
	int __current_view;

	::CollisionManager __collision_manager;
	IBroadphase* __broadphase_handler;
	//CollisionGrid __collision_grid;

	void DrawBackground(const BackgroundProperties& background);
	void UpdateViews();

};

#endif