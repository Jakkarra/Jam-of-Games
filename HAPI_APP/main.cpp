/*
	HAPI Sprites Start
	----------
	This solution contains an already set up HAPI Sprites project and this main file with brief example code
	Release and Debug configurations for both 64 bit and 32 bit builds are configured for you.
	
	The directory structure and main files are:

	HAPISprites_Start - contains the Visual Studio solution file (.sln)
		HAPI_APP - contains the Visual Studio HAPI_APP project file (.vcxproj) and source code
			HAPI_SPRITES - the directory with all the HAPI library files including GLM
			Data - a place to put all your data files
		MakeDistributable32.bat - a batch file for populating the Demo folder with a 32 bit distributable
		MakeDistributable64.bat - a batch file for populating the Demo folder with a 64 bit distributable
		Demo - a folder filled via the above batch files

	Last updated: Keith Ditchburn 11/01/17
*/

// Include the HAPI Sprites header to get access to all of the HAPI Sprites interface
#include <HAPISprites_lib.h>
#include "EntityPlayer.h"
// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

// GLM is included in the distributable and can be included if requeried
#include <glm/glm.hpp>

constexpr DWORD kUpdateTime{ 10 };

// Every HAPISprites program must have a HAPI_Sprites_Main as an entry point
// When this function exits the program will close down
void HAPI_Sprites_Main()
{
	unsigned int width{ 1024 };
	unsigned int height{ 768 };

	// We must initialise HAPI Sprites before doing anything else
	// This call allows us to specify the screen size and optionally the window title text and flags
	if (!HAPI_Sprites.Initialise(width, height, "HAPI Sprites Start Project"))
		return;

	// HApi Sprites has a rectangle type that takes left, right, top, bottom values
	Rectangle screenRect(0, width, 0, height);

	// Turn on the FPS counter
	HAPI_Sprites.SetShowFPS(true);

	CEntityPlayer player("Data\\thing.png");
	CEntityPlayer player2("Data\\thing.png");


	float logoRotationAngle = 10;

	DWORD lastTimeUpdated{ 0 };

	// Enter the HAPI Sprites update loop. This continues until HAPI Sprites wants to close
	while (HAPI_Sprites.Update())
	{
		// Clear the screen surface
		SCREEN_SURFACE.Clear(0);

		
		// Draw the logo sprite to the centre of the screen with 'logoRotationAngle' rotation
		player.render();


		// Get the current keyboard state
		const HAPI_TKeyboardData &keyData = HAPI_Sprites.GetKeyboardData();

		// Allow the user to change the rotation angle using left and right keys
		if (keyData.scanCode[HK_LEFT])
			logoRotationAngle -= 0.001f;

		// Uncomment to control sprite rotation manually
			if (keyData.scanCode[HK_RIGHT])			
			logoRotationAngle += 0.001f;
	}
}

