#pragma once
#include "ResourceCollection.h"
#include "Sprite.h"
#include "Background.h"

class Game {

public:
	static void Init();
	static void Run();
	static void End();

private:
	//static ResourceCollection<int, Sprite> __sprites;
	//static ResourceCollection<int, Background> __backgrounds;

};