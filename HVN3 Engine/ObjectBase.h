#pragma once
#include "IUpdatable.h"
#include "IDrawable.h"

namespace hvn3 {

	typedef int ObjectId;

	enum : ObjectId {
		self = -1,
		other = -2,
		all = -3,
		noone = -4
	};

	class ObjectBase : public virtual IUpdatable, public virtual IDrawable {

	public:
		ObjectBase();
		virtual void OnDraw(DrawEventArgs& e) override;
		virtual void OnUpdate(UpdateEventArgs& e) override;
		void Destroy();
		bool IsDestroyed();
		void Deactivate();
		bool IsActive();

	private:
		bool _is_destroyed;
		bool _active;

	};

}