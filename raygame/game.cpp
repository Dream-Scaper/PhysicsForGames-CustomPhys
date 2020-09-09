// HEADER
#include "game.h"

// SYSTEM LIBRAIES
#include <iostream> // cout
#include <ctime> // time
#include <cstdlib> // rand, srand

// PROJECT LIBRARIES
#include "raylib.h"

// PROJECT HEADERS
#include "enumUtils.h"

// Define a function that initializes our static variable.
collisionMap setupCollisionChecks()
{
	collisionMap map;
	map[static_cast<collisionPair>(shapeType::CIRCLE | shapeType::CIRCLE)] = checkCircleCircle;
	// TODO: checkCircleAABB
	// TODO: checkAABBAABB

	return map;
}

// Define the static variable in game.
collisionMap game::collisionCheckers = setupCollisionChecks();

game::game()
{
	accumulatedDeltaTime = 0.0f;
	fixedTimeStep = 1.0f / 30.0f;
}

void game::init(int screenWidth, int screenHeight, float timeStep)
{
	InitWindow(screenWidth, screenHeight, "Jay's Physics Sim");

	SetTargetFPS(60);

	fixedTimeStep = timeStep;

	// Seed RNG with unique value (in this case, system time).
	srand(time(0));
}

bool game::tick()
{
	// Add delta time to the accumulator.
	accumulatedDeltaTime += GetFrameTime();

	// If clicked, add a new physObject.
	if (IsMouseButtonPressed(0))
	{
		auto cursorPos = GetMousePosition();

		physObject baby({ cursorPos.x, cursorPos.y }, { 0,0 }, (rand() % 10) + 1, true);
		baby.coll.circleData.radius = baby.mass;
		baby.addImpulse({ rand() % 101 , rand() % 101 });

		physObjects.push_back(baby);
	}

	// TODO: hold right click pushes away particles within a radius.

	return !WindowShouldClose();
}

void game::tickPhysics()
{
	accumulatedDeltaTime -= fixedTimeStep;

	// TODO: gravity for all physics objects which have gravity enabled.

	// TODO optimize with spatial partitioning
	// Test for collision
	for (auto& lhs : physObjects)
	{
		for (auto& rhs : physObjects)
		{
			// Skip ourselves
			if (&lhs == &rhs) { continue; }

			auto *first = &lhs;
			auto *second = &rhs;

			// Swap our pointers around so they're in the right order.
			// static_cast<TARGET_TYPE>(VARIABLE) is effectively the same as (TARGET_TYPE)VARIABLE
			if (static_cast<uint8_t>(lhs.coll.colliderShape) > static_cast<uint8_t>(rhs.coll.colliderShape))
			{
				first = &rhs;
				second = &lhs;
			}

			// Get type of collision to test for.
			collisionPair pairType = static_cast<collisionPair>(lhs.coll.colliderShape | lhs.coll.colliderShape);

			// Get the collision check function to call, and then call it.
			bool collision = collisionCheckers[pairType](first->pos, first->coll, second->pos, second->coll);;

			// Do Something:tm: with that collision (if it happens).
			if (collision)
			{
				std::cout << "collision at (" << lhs.pos.x << "," << lhs.pos.y << ") and (" << rhs.pos.x << "," << rhs.pos.y << ")." << std::endl;
			}
		}
	}

	// Update all physObjects.
	for (auto& obj : physObjects)
	{
		obj.tickPhysics(fixedTimeStep);
	}
}

bool game::shouldTickPhysics() const
{
	return accumulatedDeltaTime >= fixedTimeStep;
}

void game::draw() const
{
	BeginDrawing();
	
	ClearBackground(SKYBLUE);

	// Draws for objects happen here after the background clear.
	// ----------------------------------------------------------
	for (auto& obj : physObjects)
	{
		obj.draw();
	}


	// ----------------------------------------------------------
	EndDrawing();
}

void game::exit()
{
	CloseWindow();
}
