#include "World.h"


using namespace HAPISPACE;
World::World()
{
}


World::~World()
{
	for (auto p : entityVector)
		delete p;

	for (auto p : bulletVector)
		delete p;

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
			
			if (mouse.rightButtonDown)
			{
				currentState = ePlayState;
			}
		}

		if (currentState == ePlayState)
		{
			Playing();
			//do playing function
			if (mouse.leftButtonDown)
			{
				currentState = eGameOver;
			}
		}

		if (currentState == eGameOver)
		{
			HAPI_Sprites.RenderText(660, 540, HAPI_TColour(255, 255, 0), "Game Over State", 70);
			//do gameover function
			if (mouse.middleButtonDown)
			{
				for (auto p : entityVector)
					p->initialiseValues();

				currentState = ePlayState;
			}
		}
	}
}

void World::Initialise()
{
	entityVector.push_back(player_);

	for (int i = 0; i < 500; i++)
	{
		CEntityBullet *bullet = new CEntityBullet;
		bulletVector.push_back(bullet);
		//max of 500 bullets
	}
	for (int i = 0; i < 20; i++)
	{
		EntityEnemy* enemy_ = new EntityEnemy("Data//rocketUp.png"); // we would need to make different types of enemies, or better yet opn room load randomly choose different types
	
		entityVector.push_back(enemy_);
	}
	//here we would add enemies to enemy vector to set a max number of enemies, all initally dead. then set however many we want to alive as you enter a room



	


	First_Room = new Room("Room_Floor_1.png", Position_To_Spawn, "Corners_And_Walls_Room_1.png", 32);


	First_Room->Create_Invidividual_Room();


	Second_Room = new Room("Room_Floor_1_.png", Position_To_Spawn_second, "Corners_And_Walls_Room_1.png", 32);


	auto test_texture = HAPI_Sprites.MakeSurface("Data\\Room_Floor_1.png");

	Second_Room->Create_Complex_Room(test_texture);




}

void World::Playing()
{
	if (player_->isAlive() == false)
		currentState = eGameOver;

	currTime = HAPI_Sprites.GetTime();

	if (currTime >= updateTime)
	{
		for (auto p : entityVector)
			p->update(*this);

		for (auto p : bulletVector) //seperate bullet vector so i can pass them through
			p->update(*this);



		for (auto p : entityVector)
			for (auto r : entityVector)
			{
				if (p->isAlive() == true && r->isAlive() == true && p->getSide() != r->getSide() && p->isInvunerable() == false && r->isInvunerable() == false)
					if (p->getPntrToSprite()->CheckCollision(p->getPos(), r->getSprite(), r->getPos()) == true)
					{
						p->hasCollided(r);
						r->hasCollided(p);
					}
			}

		for (auto p : entityVector)
			for (auto r : bulletVector)
			{
				if (p->isAlive() == true && r->isAlive() == true && p->getSide() != r->getSide() && p->isInvunerable() == false)
					if (p->getPntrToSprite()->CheckCollision(p->getPos(), r->getSprite(), r->getPos()) == true)
					{
						p->hasCollided(r);
						r->hasCollided(p);
					}
			}
		updateTime = HAPI_Sprites.GetTime() + 30.0f;
	}

	First_Room->Render_Floor();

	Second_Room->Render_Floor();


	for (auto p : entityVector) //might be better to have a single vector instead of two and have the offset for where the bullets start
		p->render();

	for (auto p : bulletVector) //also the render is seperate to the update as update is every tick, render may be slowed down
		p->render();


}

void World::mainMenu()
{

	HAPI_Sprites.RenderText(960, 540, HAPI_TColour(255, 0, 0), "Menu State");

}
