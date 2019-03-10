#pragma once

#include "hvn3/rooms/ISceneManager.h"

namespace hvn3 {

	class SceneManagerBase :
		public ISceneManager {

	public:
		SceneManagerBase();

		// Inherited from IDrawable

		depth_type Depth() const override;
		void SetDepth(depth_type value) override;

		// Inherited from IManager

		bool IsSuspendable() const override;

	protected:

		// Inherited from IContextReciever

		void ReceiveContext(const ApplicationContext& context) override;

		// Inherited from IDrawable

		void OnDraw(DrawEventArgs& e) override;

		// Inherited from IUpdatable

		void OnUpdate(UpdateEventArgs& e) override;

		// Inherited from IManager

		ApplicationContext Context() override;

	private:
		ApplicationContext _context;
		depth_type _depth;

	};

}