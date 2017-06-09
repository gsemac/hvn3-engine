#ifndef __OBJECT_H
#define __OBJECT_H
#include <initializer_list>
//#include "IDrawable.h"
#include "IUpdatable.h"
#include "ICollidable.h"
#include "ISpriteable.h"
#include "Vector2d.h"
#define noone 0

namespace hvn3 {

	class DrawEventArgs;

	typedef int ObjectId;

	class ObjectBase : public IUpdatable, public IDrawable {

	public:
		ObjectBase();
		virtual void OnDraw(DrawEventArgs& e) override;
		virtual void OnUpdate(UpdateEventArgs& e) override;
		void Destroy();
		bool IsDestroyed();
		void Deactivate();
		bool IsActive();

	private:
		bool __is_destroyed;
		bool __active;

	};

	class Object : public ObjectBase, public ICollidable, public ISpriteable {
		friend class CollisionGrid;
		friend class RoomBase;

	public:
		Object();
		Object(float x, float y);
		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;

		virtual ObjectId Id() const;
		int Depth() const;
		void SetDepth(int depth);
		Vector2d& Velocity();

		template<typename T, typename ... Args>
		static std::shared_ptr<Object> Create(Args &&... args) {

			return std::make_shared<T>(std::forward<Args>(args)...);

		}

		float DistanceTo(const Object* other);

	private:
		float __image_index_timer;
		Vector2d __velocity;
		int __depth;

		void IncrementImageIndex();

	};

}

#endif