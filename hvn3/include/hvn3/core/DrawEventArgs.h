#pragma once
#include "hvn3/core/GameStateEventArgs.h"
#include "hvn3/graphics/Graphics.h"

namespace hvn3 {

	class DrawEventArgs : public System::GameStateEventArgs {

	public:
		DrawEventArgs(Graphics::Graphics& graphics);
		DrawEventArgs(Graphics::Graphics& graphics, IGameManager* manager);
		Graphics::Graphics& Graphics();

	private:
		Graphics::Graphics& _graphics;

	};

}