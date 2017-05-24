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
#include "World.h"
// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

// GLM is included in the distributable and can be included if requeried
#include <glm/glm.hpp>




// Every HAPISprites program must have a HAPI_Sprites_Main as an entry pointx
// When this function exits the program will close down
void HAPI_Sprites_Main()
{
	
	unsigned int screenWidth{ 1920 };
	unsigned int screenHeight{ 1080 };

	HAPI_Sprites.Initialise(screenWidth, screenHeight, "DIY:Dungeon It Yourself");// , eFullscreen);
	World* world_ = new World;
	world_->Run();
}

