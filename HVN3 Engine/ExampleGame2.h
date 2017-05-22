#pragma once
#include "HVN3.h"
#include "String.h"
#include "DrawEventArgs.h"
#include "GUIButton.h"
#include "GUIWindow.h"
#include "GUIManager.h"
#include "GuiTextbox.h"
#include "Game.h"

namespace hvn3 {

	namespace SuperMarioBros {

		enum OBJECT_ID {
			OBJ_PLAYER,
			OBJ_SOLID
		};

		enum RES_ID {
			SPR_PLAYER,
			SPR_BLOCK,
			BG_HILLS
		};

		ResourceCollection<RES_ID, Sprite> sprites;
		ResourceCollection<RES_ID, Background> backgrounds;

		class Player : public Object {

		public:
			Player(float x, float y) : Object(x, y) {

				SetSprite(sprites[SPR_PLAYER]);
				SetImageSpeed(0.0f);

				Filter().SetCategoryBits(OBJ_PLAYER);
				Filter().SetMaskBits(OBJ_SOLID);
				SetCollisionMask(::CollisionMask(Rectangle(0, 16, 16, 16)));

			}
			void Update(UpdateEventArgs& e) override {


				//static float angle = 0;
				////Room()->View(0).SetAngle(angle);
				//angle += 0.5;

				//if (Keyboard::KeyDown(ALLEGRO_KEY_RIGHT) && PlaceFree(X() + 5, Y()))
				//	TranslateX(5);
				//else if (Keyboard::KeyDown(ALLEGRO_KEY_LEFT) && PlaceFree(X() - 5, Y()))
				//	TranslateX(-5);
				//else if (Keyboard::KeyDown(ALLEGRO_KEY_UP))
				//	TranslateY(-5);
				//else if (Keyboard::KeyDown(ALLEGRO_KEY_DOWN))
				//	TranslateY(5);

				//if (Keyboard::KeyDown(ALLEGRO_KEY_F5)) {
				//	Room()->Restart();
				//	angle = 0;
				//}

				///*if (PlaceFree(X(), Y() + 1))
				//	TranslateY(1);*/

				//MoveContact(270.0f, 1);

				//Object::Update(e);

			}
			void Draw(DrawEventArgs& e) override {

				Object::Draw(e);

				e.Graphics().DrawRectangle(AABB(), Color::Red, 1);
				//std::cout << "Mouse::Position(): " << Mouse::Position() << std::endl;
				e.Graphics().DrawCircle(Mouse::Position(), 4, Color::Red, 2);

			}

		};

		class Block : public Object {

		public:
			Block(float x, float y) : Object(x, y) {

				SetSprite(sprites[SPR_BLOCK]);

				Filter().SetCategoryBits(OBJ_SOLID);
				SetCollisionMask(::CollisionMask(Rectangle(0, 0, 16, 16)));

			}
			void BeginUpdate(UpdateEventArgs& e) override {



			}
			void Update(UpdateEventArgs& e) override {



			}
			void Draw(DrawEventArgs& e) override {

				Object::Draw(e);

				e.Graphics().DrawRectangle(AABB(), Color::Red, 1);

			}

		};

		class Stage001 : public RoomBase {

		public:
			Stage001() : RoomBase(640, 480) {

				SetUp();

			}
			void SetUp() override {

				SetBackgroundColor(Color::Silver);

				for (int i = 68; i <= 148; i += 16)
					ObjectManager().InstanceAdd(Object::Create<Block>(i, 148));
				for (int i = 148; i <= 404; i += 16)
					ObjectManager().InstanceAdd(Object::Create<Block>(i, 148));
				for (int i = 320; i <= 404; i += 16)
					ObjectManager().InstanceAdd(Object::Create<Block>(i, 148));

				std::shared_ptr<Object> player(new Player(100, 100));
				//AddView(::View(Point(0, 0), Size(640, 480), Point(0, 0), Size(640, 480), player, 0.0f, 0.0f));
				//AddView(::View(Point(0, 0), Size(160, 120), Point(0, 0), Size(320, 240), player, 0.0f, 0.0f));
				//AddView(::View(Point(0, 0), Size(320, 240), Point(0, 0), Size(320, 240), player, 0.0f, 0.0f));
				//AddView(::View(Point(0, 0), Size(320, 240), Point(320, 240), Size(320, 240), player, 0.0f, 0.0f));

				for (int i = 0; i < ViewCount(); ++i)
					View(i).Enable();

				ObjectManager().InstanceAdd(player);

				AddBackground(backgrounds[BG_HILLS]);
				Background(0).SetFixed(true);

				std::shared_ptr<Gui::GuiManager> manager(new Gui::GuiManager());
				Gui::Window* window = new Gui::Window(100, 100, 200, 100, "Window 1");
				window->AddControl(new Gui::Button(1, 0, 100, 29, "Button 1"));
				window->AddControl(new Gui::Button(1, 29, 100, 29, "Button 2"));
				manager->AddControl(new Gui::Textbox(1, 50, 100, 21));

				manager->AddControl(new Gui::Button(50, 50, 100, 29, "Button 1"));
				manager->AddControl(window);
				//manager->AddControl(new GUI::Button(50, 80, 100, 25, "Button 2"));
				//manager->AddControl(new GUI::Button(50, 100, 100, 25, "Button 3"));

				ObjectManager().InstanceAdd(manager);

			}

		private:

		};

		void RunGame(int argc, char *argv[]) {

			try {

				InitializeFramework();

				// Set up Game Resources.
				IO::Directory::SetCurrentDirectory(IO::Path::Combine(IO::Directory::GetCurrentDirectory(), "data"));
				sprites.Add(SPR_PLAYER, Sprite::FromSpriteSheet("ExampleGame2/mario_small_walk.png", 16, 32, 0, 0, Color(157, 159, 159)));
				sprites.Add(SPR_BLOCK, Sprite("ExampleGame2/block_001.png"));
				backgrounds.Add(BG_HILLS, Background("ExampleGame2/background_001.png"));

				// Set up Game Properties.
				Properties properties;
				properties.DisplayTitle = "HVN3 Engine";
				properties.OutsideColor = Color::Black;
				properties.DebugMode = true;
				properties.ScalingMode = ScalingMode::MaintainAspectRatio;

				// Set up the initial Scene.
				Stage001 scene;

				// Create a new Runner instance to handle the game logic.
				//Runner(properties, scene).Loop();

				// Clear resources.
				sprites.Clear();
				backgrounds.Clear();

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

}