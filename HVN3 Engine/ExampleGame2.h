#pragma once
#include "HVN3.h"
#include "String.h"

namespace SuperMarioBros {

	class Player : public Object {

	};

	void RunGame(int argc, char *argv[]) {

		try {

			// Initialize the Framework.
			Framework::Initialize();

			// Set up game resources.
			std::string resource_directory = IO::Path::Combine(IO::Directory::GetCurrentDirectory(), "data", "ExampleGame2");

			// Set up Game Properties.
			GameProperties properties;
			properties.DisplayTitle = "HVN3 Engine";
			properties.OutsideColor = Color::Black;
			properties.DebugMode = true;
			properties.ScalingMode = ScalingMode::MaintainAspectRatio;

			// Set up the initial Scene.
			Scene scene(properties.DisplaySize.Width(), properties.DisplaySize.Height(), new CollisionGrid(16, 16));
			scene.SetBackgroundColor(Color::Silver);
			Player* player = new Player();
			std::shared_ptr<Sprite> spr = std::make_shared<Sprite>(Sprite::FromSpriteSheet(IO::Path::Combine(resource_directory, "mario_small_walk.png"), 16, 32, 0, 0, Color(157, 159, 159)));
			player->SetSprite(spr);
			player->SetImageSpeed(0.1f);
			scene.AddObject(player, 100, 100);

			// Create a new Runner instance to handle the game logic.
			Runner(properties, scene).Loop();

			// Shut down the Framework.
			Framework::Shutdown();

		}
		catch (std::exception& ex) {

			// Output the error message.
			std::cout << ex.what();

			// Rethrow the error, since we cannot handle it here.
			throw;

		}

	}

}