#pragma once
#include "HVN3.h"
#include "String.h"
#include "DrawEventArgs.h"

namespace SuperMarioBros {

	enum OBJECT_ID {
		OBJ_PLAYER,
		OBJ_SOLID
	};

	enum SPRITE_ID {
		SPR_PLAYER,
		SPR_BLOCK
	};

	ResourceCollection<SPRITE_ID, Sprite> sprites;

	class Player : public Object {

	public:
		Player(float x, float y) : Object(x, y) {

			SetSprite(sprites[SPR_PLAYER]);
			SetImageSpeed(0.0f);

			Filter().SetCategoryBits(OBJ_PLAYER);
			Filter().SetMaskBits(OBJ_SOLID);
			SetCollisionMask(::CollisionMask(Rectangle(0, 16, 16, 16)));

		}
		void Update(UpdateEventArgs e) override {

			if (Keyboard::KeyDown(ALLEGRO_KEY_RIGHT) && PlaceFree(X() + 5, Y()))
				TranslateX(5);
			else if (Keyboard::KeyDown(ALLEGRO_KEY_LEFT) && PlaceFree(X() - 5, Y()))
				TranslateX(-5);
			else if (Keyboard::KeyDown(ALLEGRO_KEY_UP))
				TranslateY(-5);
			else if (Keyboard::KeyDown(ALLEGRO_KEY_DOWN))
				TranslateY(5);

			if (Keyboard::KeyDown(ALLEGRO_KEY_F5)) {
				Scene().Restart();
			}

			/*if (PlaceFree(X(), Y() + 1))
				TranslateY(1);*/

			MoveContact(270.0f, 1);

			Object::Update(e);

		}

		void Draw(DrawEventArgs e) override {

			Object::Draw(e);

			e.Graphics().DrawRectangle(AABB(), Color::Red, 1);

		}

	};

	class Block : public Object {

	public:
		Block(float x, float y) : Object(x, y) {

			SetSprite(sprites[SPR_BLOCK]);

			Filter().SetCategoryBits(OBJ_SOLID);
			SetCollisionMask(::CollisionMask(Rectangle(0, 0, 16, 16)));

		}
		void Update(UpdateEventArgs e) override {



		}
		void Draw(DrawEventArgs e) override {

			Object::Draw(e);

			e.Graphics().DrawRectangle(AABB(), Color::Red, 1);

		}

	};

	class Stage001 : public Room {

	public:
		Stage001() : Room(640, 480, new CollisionGrid(16, 16)) {

			Build();

		}
		void Build() override {

			SetBackgroundColor(Color::Silver);

			Player* player = new Player(100, 100);
			AddView(::View(Point(0, 0), Size(320, 240), Point(0, 0), Size(320, 240), player, 0.0f, 0.0f));
			AddView(::View(Point(0, 0), Size(320, 240), Point(320, 240), Size(320, 240), player, 0.0f, 0.0f));

			for (int i = 0; i < ViewCount(); ++i)
				View(i).Enable();

			AddObject(player);
			for (int i = 68; i <= 148; i += 16)
				AddObject(new Block(i, 148));
			for (int i = 148; i <= 404; i += 16)
				AddObject(new Block(i, 180));
			for (int i = 320; i <= 404; i += 16)
				AddObject(new Block(i, 300));

		}

	private:


	};

	void RunGame(int argc, char *argv[]) {

		try {

			// todo next: Make it possible to reset the game state/current scene.

			// Initialize the Framework.
			InitializeFramework();

			// Set up Game Resources.
			IO::Directory::SetCurrentDirectory(IO::Path::Combine(IO::Directory::GetCurrentDirectory(), "data", "ExampleGame2"));
			sprites.Add(SPR_PLAYER, new Sprite(Sprite::FromSpriteSheet(IO::Path::Combine(IO::Directory::GetCurrentDirectory(), "mario_small_walk.png"), 16, 32, 0, 0, Color(157, 159, 159))));
			sprites.Add(SPR_BLOCK, new Sprite(IO::Path::Combine(IO::Directory::GetCurrentDirectory(), "block_001.png")));

			// Set up Game Properties.
			GameProperties properties;
			properties.DisplayTitle = "HVN3 Engine";
			properties.OutsideColor = Color::Black;
			properties.DebugMode = true;
			properties.ScalingMode = ScalingMode::MaintainAspectRatio;

			// Set up the initial Scene.
			Stage001 scene;

			// Create a new Runner instance to handle the game logic.
			Runner(properties, scene).Loop();

			// Dispose of resources.
			sprites.Clear();

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