#include "World.h"


using namespace HAPISPACE;
World::World()
{
	Point Position_To_Spawn{ 100, 150 };

	Point Position_To_Spawn_second{ 1500, 750 };

	Point Position_To_Spawn_third{ 1300, 675 };

	First_Room = new Room("Floor.png", Position_To_Spawn, "Corners_And_Walls_Room_1.png", 32);

	First_Room->Create_Invidividual_Room();

	Second_Room = new Room("Room_Floor_1_.png", Position_To_Spawn_second, "Corners_And_Walls_Room_1.png", 32);

	Second_Room->Create_Invidividual_Room();

	//Third_Room = new Room("Room_Floor_1_.png", Position_To_Spawn_third, "Corners_And_Walls_Room_1.png", 32);

	//Third_Room->Create_Invidividual_Room();

	First_Room->Pathfind_Corridor(*Second_Room);

	//Second_Room->Pathfind_Corridor(*Third_Room);

	First_Room->Link_Rooms(*Second_Room);

	//Second_Room->Link_Rooms(*Third_Room);
}


World::~World()
{
	for (auto p : entityVector)
		delete p;

	for (auto p : bulletVector)
		delete p;

	delete player_;
}

void World::Run()
{
	Initialise();


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

		First_Room->Render_Path("Seamless_Texture.png");
		//Second_Room->Render_Path("Corridor_Floor.png");

		First_Room->Render_Floor();
		Second_Room->Render_Floor();
		//Third_Room->Render_Floor();
		First_Room->Spawn_Points();
		//Second_Room->Spawn_Points();

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

void World::Initialise()
{

	for (int i = 0; i < 500; i++)
	{
		CEntityBullet *bullet = new CEntityBullet;
		bulletVector.push_back(bullet);
		//max of 500 bullets
	}

	entityVector.push_back(player_);
	//here we would add enemies to enemy vector to set a max number of enemies, all initally dead. then set however many we want to alive as you enter a room
}

void World::Playing()
{
	

}

void World::mainMenu()
{



}
