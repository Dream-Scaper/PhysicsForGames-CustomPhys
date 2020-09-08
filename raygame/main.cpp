#include "raylib.h"
#include "game.h"
#include "glm.hpp"

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	game gm;

	gm.init();
	gm.fixedTimeStep = 1.0f / 30.0f; // About 30 times 1 second.

	bool shouldRun = true;
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (shouldRun)
	{
		// Update
		//----------------------------------------------------------------------------------
		shouldRun = gm.tick();

		// Physics updates.
		while (gm.shouldTickPhysics())
		{
			gm.tickPhysics();
		}

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		gm.draw();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	gm.exit();
	//--------------------------------------------------------------------------------------

	return 0;
}