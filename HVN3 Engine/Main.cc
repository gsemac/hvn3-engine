#include "HVN3.h"
//#include "ExampleGame.h"
#include "ExampleGame2.h"
#include "Display.h"
#include "Graphics.h"
#include "Tileset.h"
#include "RoomManager.h"
#include "ObjectManager.h"

class TestRoom : public Room {

public:
	TestRoom() : Room(640, 480) {
	
		SetUp();

	}

	void SetUp() override {
		
		SetBackgroundColor(Color::BlanchedAlmond);

	}

private:


};

int main(int argc, char *argv[]) {

	Properties properties;
	properties.DisplayTitle = "Hello, world!";
	properties.OutsideColor = Color::Black;

	RoomManager room_manager;
	room_manager.AddRoom(std::make_shared<TestRoom>());

	CollisionGrid broadphase_collision_manager(32, 32);
	CollisionManager collision_manager(&broadphase_collision_manager);
	ObjectManager obj_manager(collision_manager);
	
	// Run the main game loop.
	Framework(argc, argv).Loop(Runner(properties, room_manager));

	return 0;

}