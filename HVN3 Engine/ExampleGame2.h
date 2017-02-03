#pragma once
#include "HVN3.h"
#include "String.h"
#include "DrawEventArgs.h"

namespace SuperMarioBros {

	typedef std::shared_ptr<Sprite> SpritePtr;
	SpritePtr spr_player, spr_block;

	enum OBJECT_ID {
		OBJ_PLAYER,
		OBJ_SOLID
	};

	class Player : public Object {

	public:
		Player(float x, float y) : Object(x, y) {

			SetSprite(spr_player);
			SetImageSpeed(0.0f);

			SetCollisionId(OBJ_PLAYER);
			SetCollisionFilter(OBJ_SOLID);
			SetCollisionMask(CollisionMask(Rectangle(0, 16, 16, 16)));

		}
		void Update(float dt) override {

			if (Keyboard::KeyDown(ALLEGRO_KEY_RIGHT) && PlaceFree(X() + 5, Y()))
				TranslateX(5);
			else if (Keyboard::KeyDown(ALLEGRO_KEY_LEFT) && PlaceFree(X() - 5, Y()))
				TranslateX(-5);

			/*if (PlaceFree(X(), Y() + 1))
				TranslateY(1);*/

			MoveContact(270.0f, 1);

			Object::Update(dt);

		}

		void Draw(DrawEventArgs e) override {

			Object::Draw(e);

			e.Graphics().DrawRectangle(AABB(), Color::Red, 1);

		}

	};

	class Block : public Object {

	public:
		Block(float x, float y) : Object(x, y) {

			SetSprite(spr_block);

			SetCollisionId(OBJ_SOLID);
			SetCollisionMask(CollisionMask(Rectangle(0, 0, 16, 16)));

		}
		void Update(float dt) override {



		}
		void Draw(DrawEventArgs e) override {

			Object::Draw(e);

			e.Graphics().DrawRectangle(AABB(), Color::Red, 1);

		}

	};

	void RunGame(int argc, char *argv[]) {

		try {

			// todo next: Make it possible to reset the game state/current scene.

			// Initialize the Framework.
			InitializeFramework();

			/*	Display first_display(200, 200, "Window #1");
				Display second_display(200, 200, "Window #2");

				do {
					Graphics::DrawCircle(100, 100, 10, Color::White, 2);
					first_display.Refresh();
					second_display.Refresh();
				} while (1);
	*/
	// Set up Game Resources.
			IO::Directory::SetCurrentDirectory(IO::Path::Combine(IO::Directory::GetCurrentDirectory(), "data", "ExampleGame2"));
			spr_player = std::make_shared<Sprite>(Sprite::FromSpriteSheet(IO::Path::Combine(IO::Directory::GetCurrentDirectory(), "mario_small_walk.png"), 16, 32, 0, 0, Color(157, 159, 159)));
			spr_block = std::make_shared<Sprite>(IO::Path::Combine(IO::Directory::GetCurrentDirectory(), "block_001.png"));

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
			//scene.AddView(View(Point(0, 0), Size(640, 480), Point(0, 0), Size(640, 480), player, 0.0f, 0.0f));
			scene.AddView(View(Point(0, 0), Size(320, 240), Point(0, 0), Size(320, 240), player, 0.0f, 0.0f));
			scene.AddView(View(Point(0, 0), Size(320, 240), Point(320, 240), Size(320, 240), player, 0.0f, 0.0f));

			for (int i = 0; i < scene.ViewCount(); ++i)
				scene.View(i).Enable();

			scene.AddObject(player);
			for (int i = 68; i <= 148; i += 16)
				scene.AddObject(new Block(i, 148));
			for (int i = 148; i <= 404; i += 16)
				scene.AddObject(new Block(i, 180));
			for (int i = 320; i <= 404; i += 16)
				scene.AddObject(new Block(i, 300));

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