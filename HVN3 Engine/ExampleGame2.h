#pragma once
#include "HVN3.h"
#include "String.h"

namespace SuperMarioBros {

	class Player : public Object {

	public:
		Player(float x, float y) : Object(x, y) {

			SetImageSpeed(0.1f);
			Velocity().SetX(1);

		}
		void Update(float dt) override {

			if (Keyboard::KeyDown(ALLEGRO_KEY_RIGHT))
				TranslateX(5);
			else if (Keyboard::KeyDown(KEY_ANY)) {

			}


		}

	};

	class Block : public Object {

	public:
		Block(float x, float y) : Object(x, y) {}
		void Update(float dt) override {



		}
		void Draw() override {

			Graphics::DrawFilledRectangle(Rectangle(X(), Y(), 16, 16), Color::Black);

		}

	};

	void RunGame(int argc, char *argv[]) {

		try {

			// Initialize the Framework.
			InitializeFramework();

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
			Player* player = new Player(100, 100);
			std::shared_ptr<Sprite> spr = std::make_shared<Sprite>(Sprite::FromSpriteSheet(IO::Path::Combine(resource_directory, "mario_small_walk.png"), 16, 32, 0, 0, Color(157, 159, 159)));
			player->SetSprite(spr);
			scene.AddObject(player);

			// Create a new Runner instance to handle the game logic.
			Runner(properties, scene).Loop();

			// Shut down the Framework.
			ShutdownFramework();

		}
		catch (std::exception& ex) {

			// Output the error message.
			std::cout << ex.what();

			// Rethrow the error, since we cannot handle it here.
			throw;

		}

	}

}