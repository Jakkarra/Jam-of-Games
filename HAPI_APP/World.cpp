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
		SCREEN_SURFACE.Clear();
		if (currentState == eMainMenu)
		{
			mainMenu();
		}

		if (currentState == ePlay)
		{
			Playing();
		}

		if (currentState == eGameOver)
		{			
			endGame();					
		}

		if (currentState == eCharacter)
		{
			charCreation();
		}

		if (currentState == ePaused)
		{
			Pause();
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
	EntityHealth* health = new EntityHealth();
	entityVector.push_back(health);


	CEntityPickup *pickup1 = new CEntityPickup(1, 2, 4, 2);//get sprites for the pickups just define them as we wish
	CEntityPickup *pickup3 = new CEntityPickup(0, 0, 4, 0);
	CEntityPickup *pickup4 = new CEntityPickup(0, 3, 0, 0);
	CEntityPickup *pickup2 = new CEntityPickup(2, 0, 0, 0);

	entityVector.push_back(pickup1);
	entityVector.push_back(pickup2);
	entityVector.push_back(pickup3);
	entityVector.push_back(pickup4);

	First_Room = new Room("testFloor.png", Position_To_Spawn, "Corners_And_Walls_Room_1.png", 32);


	First_Room->Create_Invidividual_Room();


	Second_Room = new Room("Room_Floor_1_.png", Position_To_Spawn_second, "Corners_And_Walls_Room_1.png", 32);


	auto test_texture = HAPI_Sprites.MakeSurface("Data\\Room_Floor_1.png");

	Second_Room->Create_Complex_Room(test_texture);




}

void World::Playing()
{
	if (player_->isAlive() == false)
		currentState = eGameOver;
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0); //maybe find a way to make this a public thing so we dont ned to constantly create it

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER])
	{
		currentState = ePaused;
	}

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
				if (p->isAlive() && r->isAlive() && p->getSide() != r->getSide())
					if (p->getPntrToSprite()->CheckCollision(p->getPos(), r->getSprite(), r->getPos()) == true)
					{
						p->hasCollided(*r);
						r->hasCollided(*p);
					}
			}

		for (auto p : entityVector)
			for (auto r : bulletVector)
			{
				if (p->isAlive() == true && r->isAlive() == true && p->getSide() != r->getSide() && p->isInvunerable() == false)
					if (p->getPntrToSprite()->CheckCollision(p->getPos(), r->getSprite(), r->getPos()) == true)
					{
						p->hasCollided(*r);
						r->hasCollided(*p);
					}
			}

		//for auto player and reference to room vector
		//if player collides with and hasnt entered before
		//player cant leave room and spawn enemies

		updateTime = HAPI_Sprites.GetTime() + 30.0f;
	}

	First_Room->Render_Floor(getPlayerPos());

	Second_Room->Render_Floor(getPlayerPos());


	for (auto p : entityVector) //might be better to have a single vector instead of two and have the offset for where the bullets start
		p->render(getPlayerPos());

	for (auto p : bulletVector) //also the render is seperate to the update as update is every tick, render may be slowed down
		p->render(getPlayerPos());
	
	//need check collision between the rooms and player/enemies also then corridor checks 


}

void World::mainMenu()
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);
	const HAPI_TMouseData &mouse = HAPI_Sprites.GetMouseData();
	static int trans1 = 255;
	static int trans2 = 70;
	static float timelimit = 0;
	static bool canExit = false;
	HAPI_Sprites.ChangeFont("Copperplate Gothic Light");

	menuStates selectedState = eCharacter;
	
	if (!conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER])
	{
		canExit = true;
	}

	sp->setPosition(Point{ 1725,980 });
	HAPI_Sprites.RenderText(1650, 990, HAPI_TColour(255, 255, 255, 255), "Press		 to select", 24);

	HAPI_Sprites.RenderText(0, 750, HAPI_TColour(255, 255, 255, trans1), "Play", 84);
	HAPI_Sprites.RenderText(0, 870, HAPI_TColour(255, 255, 255, trans2), "Controls", 84);
	bg->render(getPlayerPos());
	sp->render(getPlayerPos());

	if (mouse.leftButtonDown)
	{
		currentState = eGameOver;
	}

	if (canExit)
	{
		if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER]) //selection
		{
			currentState = selectedState;
			canExit = false;
		}
	}
	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{
		timelimit = HAPI_Sprites.GetTime() + 200;
		optionSelected += 1;
		if (optionSelected >= 2)
			optionSelected = 0;

		if (optionSelected == 0)
		{
			trans1 = 255;
			trans2 = 70;
			selectedState = eCharacter;

		}
		else
		{
			trans1 = 70;
			trans2 = 255;
			selectedState = eControls;

		}
	}
	else if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{

		timelimit = HAPI_Sprites.GetTime() + 200;
		optionSelected += 1;
		if (optionSelected >= 2)
			optionSelected = 0;

		if (optionSelected == 0)
		{
			trans1 = 255;
			trans2 = 70;
			selectedState = eCharacter;

		}
		else
		{
			trans1 = 70;
			trans2 = 255;
			selectedState = eControls;

		}
	}
}
void World::charCreation()
{
	HAPI_Sprites.ChangeFont("Copperplate Gothic Light");
	//Menu
	totalStats = healthPoints + speedPoints + ratePoints + damagePoints;
	totalPoints - totalStats;
	static float timelimit = 0;
	static int trans1 = 255;
	static int trans2 = 120;
	static int trans3 = 120;
	static int trans4 = 120;

	HAPI_Sprites.RenderText(550, 200, HAPI_TColour(255, 255, 255, 255), 0, 0, "Choose Your Stats!", 60);
	HAPI_Sprites.RenderText(600, 350, HAPI_TColour(255, 255, 255, trans1), 0, 0, "Health:", 34);
	HAPI_Sprites.RenderText(600, 450, HAPI_TColour(255, 255, 255, trans2), 0, 0, "Speed:", 34);
	HAPI_Sprites.RenderText(600, 550, HAPI_TColour(255, 255, 255, trans3), 0, 0, "Fire Rate:", 34);
	HAPI_Sprites.RenderText(600, 650, HAPI_TColour(255, 255, 255, trans4), 0, 0, "Damage:", 34);

	HAPI_Sprites.RenderText(1100, 350, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(healthPoints), 38);
	HAPI_Sprites.RenderText(1100, 450, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(speedPoints), 38);
	HAPI_Sprites.RenderText(1100, 550, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(ratePoints), 38);
	HAPI_Sprites.RenderText(1100, 650, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(damagePoints), 38, 1);

	HAPI_Sprites.RenderText(1000, 270, HAPI_TColour(255, 255, 255, 255), 0, 0, "Points Remaining:", 34);
	HAPI_Sprites.RenderText(1400, 272, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(totalPoints), 34);

	sp->setPosition(Point{ 1660,980 });
	HAPI_Sprites.RenderText(1550, 990, HAPI_TColour(255, 255, 255, 255), "Press		 to select", 32);

	bg->render(getPlayerPos());
	sp->render(getPlayerPos());
	

	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		std::cout << "Down Press" << std::endl;
		optionSelected += 1;
	}
	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		std::cout << "Down Press" << std::endl;
		optionSelected -= 1;
	}



		if (optionSelected >= 4)
			optionSelected = 0;

		if (optionSelected < 0)
			optionSelected = 3;

		isHealth = false;
		isSpeed = false;
		isRate = false;
		isDamage = false;

		if (optionSelected == 0)
		{
			trans1 = 255;
			trans2 = 120;
			trans3 = 120; //health
			trans4 = 120;
			isHealth = true;
		}
		if (optionSelected == 1)
		{
			trans1 = 120;
			trans2 = 255;
			trans3 = 120; //speed
			trans4 = 120;
			isSpeed = true;
		}
		if (optionSelected == 2)
		{
			trans1 = 120;
			trans2 = 120; //firerate
			trans3 = 255;
			trans4 = 120;
			isRate = true;
		}
		if (optionSelected == 3)
		{
			trans1 = 120;
			trans2 = 120; //damage
			trans3 = 120;
			trans4 = 255;
			isDamage = true;
		}
		

#pragma region Stats Selection

	if (totalPoints > 0)
	{
		if (healthPoints < 1 || healthPoints > 8)
			healthPoints = 1;

		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzoneLeft && isHealth == true && HAPI_Sprites.GetTime() > timelimit)
		{
			timelimit = HAPI_Sprites.GetTime() + 300;
			healthPoints++;
		}

		if (ratePoints < 1 || ratePoints > 8)
			ratePoints = 1;

		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzoneLeft && isRate == true && HAPI_Sprites.GetTime() > timelimit)
		{
			timelimit = HAPI_Sprites.GetTime() + 300;
			ratePoints++;
		}
		
		if (speedPoints < 1 || speedPoints > 8)
			speedPoints = 1;

		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzoneLeft && isSpeed == true && HAPI_Sprites.GetTime() > timelimit)
		{
			timelimit = HAPI_Sprites.GetTime() + 300;
			speedPoints++;
		}

		if (damagePoints < 1 || damagePoints > 8)
			damagePoints = 1;

		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzoneLeft && isDamage == true && HAPI_Sprites.GetTime() > timelimit)
		{
			timelimit = HAPI_Sprites.GetTime() + 300;
			damagePoints++;
		}
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzoneLeft && isHealth == true && HAPI_Sprites.GetTime() > timelimit)
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		if (healthPoints == 1)
		{
			healthPoints = 1;
		}
		healthPoints--;
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzoneLeft && isRate == true && HAPI_Sprites.GetTime() > timelimit)
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		if (ratePoints == 1)
		{
			ratePoints = 1;
		}
		ratePoints--;
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzoneLeft && isSpeed == true && HAPI_Sprites.GetTime() > timelimit)
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		if (speedPoints == 1)
		{
			speedPoints = 1;
		}
		speedPoints--;
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzoneLeft && isDamage == true && HAPI_Sprites.GetTime() > timelimit)
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		if (damagePoints == 1)
		{
			damagePoints = 1;
		}
		damagePoints--;
	}
#pragma endregion

	totalPoints = 12 - (healthPoints + ratePoints + damagePoints + speedPoints);

	if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER] && totalPoints == 0)
	{
		player_->initialiseValues(healthPoints, speedPoints, ratePoints, damagePoints);
		healthPoints = ratePoints = damagePoints = speedPoints = 1;
		currentState = ePlay;
	}


}
void World::endGame()
{

	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);
	lt->setPosition(Point{ 1725,980 });
	gbg->render(getPlayerPos());
	lt->setPosition(Point{ 650, 500 });
	lt->render(getPlayerPos());
	HAPI_Sprites.ChangeFont("Copperplate Gothic Light");
	HAPI_Sprites.RenderText(600, 300, HAPI_TColour(255, 255, 255, 255), "You Died!", 150);
	HAPI_Sprites.RenderText(400, 500, HAPI_TColour(255, 255, 255, 255), "Press		to Return to Menu", 70);

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER])
	{
		for (auto p : entityVector)
			p->initialiseValues();

		currentState = eCharacter;
	}



}
void World::Pause()
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);
	static bool canExit = false;

	if (!conData.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER])
		canExit = true;
	
	pbg->render(getPlayerPos());

	//Stats ----------------------------
	HAPI_Sprites.RenderText(800, 200, HAPI_TColour(255, 255, 255, 255), "PAUSED", 70);

	HAPI_Sprites.RenderText(10, 10, HAPI_TColour(255, 255, 255, 255), "Current Stats", 32, 4);
	HAPI_Sprites.RenderText(10, 50, HAPI_TColour(255, 255, 255, 255), "Health:", 28);
	HAPI_Sprites.RenderText(10, 80, HAPI_TColour(255, 255, 255, 255), "Speed:", 28);
	HAPI_Sprites.RenderText(10, 110, HAPI_TColour(255, 255, 255, 255), "Fire Rate:", 28);
	HAPI_Sprites.RenderText(10, 140, HAPI_TColour(255, 255, 255, 255), "Damage:", 28);

	HAPI_Sprites.RenderText(150, 50, HAPI_TColour(255, 255, 255, 255), std::to_string(player_->getHealth()), 28);
	HAPI_Sprites.RenderText(150, 80, HAPI_TColour(255, 255, 255, 255), std::to_string(player_->getSpeed()), 28);
	HAPI_Sprites.RenderText(200, 110, HAPI_TColour(255, 255, 255, 255), std::to_string(player_->getROF()), 28);
	HAPI_Sprites.RenderText(170, 140, HAPI_TColour(255, 255, 255, 255), std::to_string(player_->getAttack()), 28);

	//Pause Menu -----------------------------
	static float timelimit = 0;
	static int trans1 = 255;
	static int trans2 = 120;
	static int trans3 = 120;
	static int trans4 = 120;

	isContinue = false;
	isControls = false;
	isExit = false;

	HAPI_Sprites.RenderText(830, 350, HAPI_TColour(255, 255, 255, trans1), 0, 0, "Continue", 45);
	HAPI_Sprites.RenderText(820, 450, HAPI_TColour(255, 255, 255, trans2), 0, 0, "Controls", 45);
	HAPI_Sprites.RenderText(900, 550, HAPI_TColour(255, 255, 255, trans3), 0, 0, "Exit", 45);

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		std::cout << "Down Press" << std::endl;
		optionSelected += 1;
	}
	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		std::cout << "Down Press" << std::endl;
		optionSelected -= 1;
	}



	if (optionSelected >= 3)
		optionSelected = 0;

	if (optionSelected < 0)
		optionSelected = 2;

	if (optionSelected == 0)
	{
		trans1 = 255;
		trans2 = 120;
		trans3 = 120; //Continue
		trans4 = 120;
		isContinue = true;
	}
	if (optionSelected == 1)
	{
		trans1 = 120;
		trans2 = 255;
		trans3 = 120; //Controls
		trans4 = 120;
		isControls = true;
	}
	if (optionSelected == 2)
	{
		trans1 = 120;
		trans2 = 120; //Exit
		trans3 = 255;
		trans4 = 120;
		isExit = true;
	}
	if (canExit)
	{
		if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER] && isContinue == true)
		{
			currentState = ePlay;
			canExit = false;
		}
		if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER] && isControls == true)
		{
			currentState = eControls;
			canExit = false;
		}
		if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER] && isExit == true)
		{
			currentState = eMainMenu;
			canExit = false;
		}
	}

}
