#pragma once
#include "SharedCreatableBase.h"
#include "ObjectBase.h"

#include "ICollidable.h"
#include "ISpriteable.h"

namespace hvn3 {

	class Object : public ObjectBase, public SharedCreateableBase<Object>, public ICollidable {

	public:
		Object(ObjectId id);
		Object(const Point& position, ObjectId id);

		ObjectId Id() const;

		int Depth() const;
		void SetDepth(int depth);
		
		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;
				
	private:
		ObjectId _id;
		int _depth;

	};

}