#include "World.h"
#include "EntityPlayer.h"
#include "Entity.h"
#include "EntityEnemy.h"

using namespace HAPISPACE;
World::World()
{
}


World::~World()
{
}

void World::Run()
{
	while (HAPI_Sprites.Update())
	{
		//
		HAPI_TMouseData mouse = HAPI_Sprites.GetMouseData();
		SCREEN_SURFACE.Clear();
		if (currentState == eMainMenu) //Main Menu goes here
		{
			mainMenu();
		}

		if (currentState == ePlayState)
		{
			HAPI_Sprites.RenderText(960, 540, HAPI_TColour(255, 0, 0), "Game State");
			//do playing function
			if (mouse.rightButtonDown)
			{
				currentState = eGameOver;
			}
		}

		if (currentState == eGameOver)
		{
			HAPI_Sprites.RenderText(960, 540, HAPI_TColour(255, 0, 0), "Game Over State");
			//do gameover function
			if (mouse.middleButtonDown)
			{
				HAPI_Sprites.Close();
			}
		}
	}
}

void World::Playing()
{
	if (player.isAlive() == false) {
		currentState = eGameOver;
	}

}

void World::mainMenu()
{
	CEntityPlayer menuB("Data\\mainMenuBackground.png");
	menuB.render;
}
