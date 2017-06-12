#pragma once
#include "ObjectBase.h"
#include "SharedCreatableBase.h"
#include "ICollidable.h"

namespace hvn3 {

	class Object : public ObjectBase, public SharedCreateableBase<Object>, public ICollidable {

	public:
		Object(ObjectId id);
		Object(ObjectId id, const Point2d<float>& position);

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