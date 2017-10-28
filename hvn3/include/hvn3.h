#define _CRT_SECURE_NO_WARNINGS
#pragma once

// This file includes all required engine components.

#include "graphics/Color.h"
#include "core/Framework.h"
#include "rooms/Room.h"
#include "tilesets/Tileset.h"

#include "core/GameManager.h"
#include "resources/ResourceManager.h"
#include "physics/BasicPhysicsManager.h"

#include "io/IO.h"

#include "utility/Environment.h"
#include "math/Vector2d.h"
#include "utility/Direction8.h"
#include "utility/Random.h"
#include "io/Console.h"
#include "utility/Thread.h"

#include "rooms/RoomTransitionFade.h"
#include "collision/RectangleHitMask.h"
#include "collision/CircleHitMask.h"
#include "sprites/SpriteRenderer.h"