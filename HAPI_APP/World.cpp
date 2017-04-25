#include "World.h"
#include "EntityPlayer.h"
#include "Entity.h"
#include "Room.h"

using namespace HAPISPACE;

World::World()
{
	Point Position_To_Spawn{ 250,250 };

	Point Position_To_Spawn_second{ 450,450 };

	First_Room = new Room("Room_Floor_1.png", Position_To_Spawn, "Corners_And_Walls_Room_1.png", 32);

	First_Room->Create_Invidividual_Room();

	Second_Room = new Room("Room_Floor_1_.png", Position_To_Spawn_second, "Corners_And_Walls_Room_1.png", 32);

	auto test_texture = HAPI_Sprites.MakeSurface("Data\\Room_Floor_1.png");

	Second_Room->Create_Complex_Room(test_texture);

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

		First_Room->Render_Floor();
		Second_Room->Render_Floor();
	}
}

void World::Playing()
{

}

void World::mainMenu()
{
	//CEntityPlayer menuB("Data\\mainMenuBackground.png");
	//menuB.render();

}
