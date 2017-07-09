#pragma once
#include "IUpdatable.h"
#include "IDrawable.h"
#include "ObjectEventArgs.h"
#include "BitFlags.h"

namespace hvn3 {

	typedef int ObjectId;

	enum : ObjectId {
		Self = -1,
		Other = -2,
		All = -3,
		NoOne = -4
	};

	enum class ObjectFlags {
		Solid = 1,
		NoCollision = 2,
		NoPhysics = 4
	};
	ENABLE_BITFLAG_OPERATORS(ObjectFlags);

	class ObjectBase : public virtual IUpdatable, public virtual IDrawable {

	public:
		ObjectBase();

		virtual void OnCreate(CreateEventArgs& e);
		virtual void OnDestroy(DestroyEventArgs& e);
		virtual void OnDraw(DrawEventArgs& e) override;
		virtual void OnUpdate(UpdateEventArgs& e) override;
		
		void Destroy();
		bool IsDestroyed() const;
		void Activate();
		void Deactivate();
		bool IsActive() const;

		ObjectFlags Flags() const;
		void SetFlags(ObjectFlags flags);

	private:
		bool _is_destroyed;
		bool _active;
		ObjectFlags _flags;

	};

}