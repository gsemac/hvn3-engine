#pragma once

#include "core/type_list.h"
#include "events/event_base.h"
#include "graphics/igraphics.h"

namespace hvn3::events {

	class DrawFrameEvent :
		EventBase<> {
	};

	class DrawEvent :
		EventBase<> {

	public:
		DrawEvent(graphics::IGraphics& graphics);

		graphics::IGraphics& Graphics();
		const graphics::IGraphics& Graphics() const;

	private:
		graphics::IGraphics& graphics;

	};

	using DrawEvents = core::TypeList<DrawFrameEvent, DrawEvent>;

}