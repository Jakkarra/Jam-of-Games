#include "World.h"
#include <random>

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
		else if (currentState == eControls)
		{
			Controls(cameFrom);
		}
		else if (currentState == eLoading)
		{
			Loading();
		}
		
	}
}
void World::Reset()
{
	Rooms.clear();
	Create_Rooms(8, 32);
	Connect_Rooms();
	player_->setPosition(Point(960, 540));

	for (auto &e : entityVector)
	{
		if (e->getSide() != player)
			e->setAlive(false);
	}
	for (auto &b : bulletVector)
		b->setAlive(false);

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
		EntityEnemy *enemy_;
		int type = (rand() % 2);
		switch (type)
		{
		case eMelee:
			enemy_ = new CEntityBruteEnemy("Data//fireBall.png");
			break;
		case eRanged:
			enemy_ = new CEntityRangedEnemy("Data//rocketUp.png");
			break;
		
		}
		entityVector.push_back(enemy_);	
	}




	//here we would add enemies to enemy vector to set a max number of enemies, all initally dead. then set however many we want to alive as you enter a room
	EntityHealth* health = new EntityHealth();
	entityVector.push_back(health);

	for (int i = 0; i < 5; i++)
	{
		CEntityPickup *powerUp = new CEntityPickup();
		entityVector.push_back(powerUp);
	}
	

	HAPI_Sprites.LoadSound("Data\\Grunt.wav");
	HAPI_Sprites.LoadSound("Data\\Power_Up.wav");
	HAPI_Sprites.LoadSound("Data\\Arrow_Shot.wav");
	HAPI_Sprites.LoadSound("Data\\Fireball.wav");

	Play_background_music(0);
}

void World::Playing()
{
	if (player_->isAlive() == false)
		currentState = eGameOver;


	Play_background_music(1);
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0); //maybe find a way to make this a public thing so we dont ned to constantly create it

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER])
	{
		currentState = ePaused;
	}

	currTime = HAPI_Sprites.GetTime();

	if (currTime >= updateTime)
	{
		player_->setOutOfBounds(true);
		int roomNumber = 0;

		for (auto p : entityVector)
			p->update(*this);

		for (auto p : bulletVector) //seperate bullet vector so i can pass them through
			p->update(*this);


		for (auto &room : Rooms)
		{
			roomNumber++;

			std::vector<std::vector<HAPISPACE::Line>> allPaths = room.getAllPaths();

			if (room.Get_Collision_Rectangle().Contains(getPlayerPos() + Point(30,80)))
			{
				player_->setOutOfBounds(false);
				if (!room.getHasPlayerEntered())
				{
					if (roomNumber == number_of_rooms)
					{
						spawnBoss(room.Get_Room_Position(), room.getsize());
						room.setHasPlayerEntered(true);
					}
					else
					{
						spawnenemy(room.Get_Room_Position(), room.getsize());
						room.setHasPlayerEntered(true);
					}
				}

				break;
			}

			if (player_->getOutOfBounds() && checkEnemiesDead())
			{
				for (auto path : allPaths)
				{
					for (auto p2 : path)
					{
						if (p2.p1.DistanceBetween(getPlayerPos() + Point(15, 50)) <= 50)
						{
							player_->setOutOfBounds(false);
							break;
						}
					}

				}
			}

			
		}
		updateTime = HAPI_Sprites.GetTime() + 30.0f;
	}

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

	

	
	for (auto &room : Rooms)
	{
		room.Render_Floor(getPlayerPos());

		if (room.Check_Path_Exists() == true && checkEnemiesDead())
			room.Render_Path("Seamless_Texture.png", getPlayerPos());
	}

	


	for (auto p : entityVector) //might be better to have a single vector instead of two and have the offset for where the bullets start
		p->render(getPlayerPos());

	for (auto p : bulletVector) //also the render is seperate to the update as update is every tick, render may be slowed down
		p->render(getPlayerPos());
	


}

void World::mainMenu()
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);
	const HAPI_TMouseData &mouse = HAPI_Sprites.GetMouseData();
	static int trans1 = 255;
	static int trans2 = 120;
	static int trans3 = 120;
	static float timelimit = 0;
	static bool canExit = false;
	HAPI_Sprites.ChangeFont("Copperplate Gothic Light");

	static menuStates selectedState = eCharacter;

	if (!conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER])
	{
		canExit = true;
	}

	sp->setPosition(Point{ 1725,980 });
	HAPI_Sprites.RenderText(1650, 990, HAPI_TColour(255, 255, 255, 255), "Press		 to select", 24);

	HAPI_Sprites.RenderText(0, 750, HAPI_TColour(255, 255, 255, trans1), "Play", 84);
	HAPI_Sprites.RenderText(0, 870, HAPI_TColour(255, 255, 255, trans2), "Controls", 84);
	HAPI_Sprites.RenderText(0, 990, HAPI_TColour(255, 255, 255, trans3), "Exit Game", 84);
	bg->render(getPlayerPos());
	sp->render(getPlayerPos());

	if (canExit)
	{
		if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER]) //selection
		{
			if (isQuit == true)
				HAPI_Sprites.Close();

			if (selectedState == eControls)
				cameFrom = 0;

			canExit = false;
			currentState = selectedState;
		}


	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
		optionSelected += 1;
	}
	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > deadzoneLeft && HAPI_Sprites.GetTime() > timelimit) //changing selection
	{
		timelimit = HAPI_Sprites.GetTime() + 300;
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
		trans3 = 120;
		selectedState = eCharacter;
		isQuit = false;

	}
	if (optionSelected == 1)
	{
		trans1 = 120;
		trans2 = 255;
		trans3 = 120;
		selectedState = eControls;
		isQuit = false;
	}
	if (optionSelected == 2)
	{
		trans1 = 120;
		trans2 = 120;
		trans3 = 255;
		isQuit = true;
	}
}
void World::charCreation()
{
	HAPI_Sprites.ChangeFont("Copperplate Gothic Light");
	//Menu
	totalStats = healthPoints + speedPoints + ratePoints + damagePoints;
	static float timelimit = 0;
	static int trans1 = 255;
	static int trans2 = 120;
	static int trans3 = 120;
	static int trans4 = 120;
	static int trans5 = 120;

	static std::string wep = "Bow";

	HAPI_Sprites.RenderText(550, 200, HAPI_TColour(255, 255, 255, 255), 0, 0, "Choose Your Stats!", 60);
	HAPI_Sprites.RenderText(600, 350, HAPI_TColour(255, 255, 255, trans1), 0, 0, "Health:", 34);
	HAPI_Sprites.RenderText(600, 450, HAPI_TColour(255, 255, 255, trans2), 0, 0, "Speed:", 34);
	HAPI_Sprites.RenderText(600, 550, HAPI_TColour(255, 255, 255, trans3), 0, 0, "Fire Rate:", 34);
	HAPI_Sprites.RenderText(600, 650, HAPI_TColour(255, 255, 255, trans4), 0, 0, "Damage:", 34);
	HAPI_Sprites.RenderText(600, 750, HAPI_TColour(255, 255, 255, trans5), 0, 0, "Weapon:", 34);

	HAPI_Sprites.RenderText(1100, 350, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(healthPoints), 38);
	HAPI_Sprites.RenderText(1100, 450, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(speedPoints), 38);
	HAPI_Sprites.RenderText(1100, 550, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(ratePoints), 38);
	HAPI_Sprites.RenderText(1100, 650, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(damagePoints), 38, 1);
	HAPI_Sprites.RenderText(1100, 750, HAPI_TColour(255, 255, 255, 255), 0, 0, wep, 38, 1);

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



	if (optionSelected >= 5)
		optionSelected = 0;

	if (optionSelected < 0)
		optionSelected = 4;

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
		trans5 = 120;
		isSpeed = true;
	}
	if (optionSelected == 2)
	{
		trans1 = 120;
		trans2 = 120; //firerate
		trans3 = 255;
		trans4 = 120;
		trans5 = 120;
		isRate = true;
	}
	if (optionSelected == 3)
	{
		trans1 = 120;
		trans2 = 120; //damage
		trans3 = 120;
		trans4 = 255;
		trans5 = 120;
		isDamage = true;
	}
	if (optionSelected == 4)
	{
		trans1 = 120;
		trans2 = 120; //Weapon
		trans3 = 120;
		trans4 = 120;
		trans5 = 255;
		isWeapon = true;
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

		if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > deadzoneLeft && isWeapon == true && HAPI_Sprites.GetTime() > timelimit)
		{
			timelimit = HAPI_Sprites.GetTime() + 300;
			if (wep == "Bow")
			{
				wep = "Staff";
			}
			else 
			{
				wep = "Bow";
			}
			weaponValue++;
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

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -deadzoneLeft && isWeapon == true && HAPI_Sprites.GetTime() > timelimit)
	{
		timelimit = HAPI_Sprites.GetTime() + 300;

		if (wep == "Bow")
		{
			wep = "Staff";
		}
		else
		{
			wep = "Bow";
		}
		weaponValue--;
	}

#pragma endregion

	totalPoints = 8 - (healthPoints + ratePoints + damagePoints + speedPoints);

	if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER] && totalPoints == 0)
	{
		player_->initialiseValues(healthPoints, speedPoints, ratePoints, damagePoints, weaponValue);
		healthPoints = ratePoints = damagePoints = speedPoints = 1;
		currentState = eLoading;
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
			cameFrom = 1;
			canExit = false;
		}
		if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER] && isExit == true)
		{
			currentState = eMainMenu;
			canExit = false;
		}
	}

}

void World::Create_Rooms(int Number_of_Rooms, int Walls_Texture_Size)
{
	Point First_Room_Position{ 760,  340 };

	Room *First_Room = new Room("Floor_1.png", First_Room_Position, "Corners_And_Walls_Room_1.png", Walls_Texture_Size);

	First_Room->Create_Invidividual_Room();
	First_Room->setHasPlayerEntered(true);

	Rooms.push_back(*First_Room);

	int offset_for_corridor = Walls_Texture_Size * 4;

	Point Last_Room_Position = First_Room_Position;

	Rectangle Last_Floor_Rect = First_Room->Get_Collision_Rectangle();

	int last_direction_to_spawn = 0;

	for (int i = 0; i < (Number_of_Rooms + 1); i++)
	{
		bool safe_to_spawn = false;

		while (safe_to_spawn == false)
		{
			int floor_number = Generate_random_vector(1, 5);

			std::string file_name;

			std::shared_ptr<Surface> Floor_Sprite;

			if (i == Number_of_Rooms)
			{
				file_name = "Boss_Room.png";

				Floor_Sprite = HAPI_Sprites.MakeSurface("Data\\" + file_name);
			}
			else
			{
				file_name = "Floor_" + std::to_string(floor_number) + ".png";

				Floor_Sprite = HAPI_Sprites.MakeSurface("Data\\" + file_name);
			}


			if (!Floor_Sprite->HasData())
			{
				return;
			}

			Point Position_To_Spawn_Room;

			bool try_position = false;

			while (try_position == false)
			{
				int direction_to_spawn = Generate_random_vector(1, 4);

				if (direction_to_spawn == 1 && last_direction_to_spawn != 2)
				{
					Position_To_Spawn_Room = Point{Last_Room_Position.x - (int)Floor_Sprite->Width() - Generate_random_vector(600, 800),  Last_Room_Position.y + Generate_random_vector(-100, 100) };
					last_direction_to_spawn = direction_to_spawn;
					try_position = true;
				}

				if (direction_to_spawn == 2 && last_direction_to_spawn != 1)
				{
					Position_To_Spawn_Room = Point{ Last_Room_Position.x + (int)Last_Floor_Rect.Width() + Generate_random_vector(600, 800),  Last_Room_Position.y + Generate_random_vector(-100, 100) };
					last_direction_to_spawn = direction_to_spawn;
					try_position = true;
				}

				if (direction_to_spawn == 3 && last_direction_to_spawn != 4)
				{
					Position_To_Spawn_Room = Point{ Last_Room_Position.x + Generate_random_vector(-100, 100), Last_Room_Position.y - (int)Floor_Sprite->Height() - Generate_random_vector(600, 800) };
					last_direction_to_spawn = direction_to_spawn;
					try_position = true;
				}

				if (direction_to_spawn == 4 && last_direction_to_spawn != 3)
				{
					Position_To_Spawn_Room = Point{ Last_Room_Position.x + Generate_random_vector(-100, 100), Last_Room_Position.y + (int)Last_Floor_Rect.Height() + Generate_random_vector(600, 800) };
					last_direction_to_spawn = direction_to_spawn;
					try_position = true;
				}
			}

			Rectangle FloorRect = Rectangle(Position_To_Spawn_Room.x, Position_To_Spawn_Room.x+ Floor_Sprite->Width(), Position_To_Spawn_Room.y , Position_To_Spawn_Room.y +Floor_Sprite->Height());

			safe_to_spawn = true;

			for (auto & room : Rooms)
			{
				if (room.Get_Collision_Rectangle().Intersects(FloorRect))
				{
					safe_to_spawn = false;
				}
			}

			if (safe_to_spawn == true && i == Number_of_Rooms)
			{
				Room * Another_Room = new Room("Boss_Room.png", Position_To_Spawn_Room, "Corners_And_Walls_Room_1.png", Walls_Texture_Size);

				Another_Room->Create_Invidividual_Room();

				Rooms.push_back(*Another_Room);
			}
			else
			{
				if (safe_to_spawn == true && i != Number_of_Rooms)
				{
					Room * Another_Room = new Room("Floor_" + std::to_string(floor_number) + ".png", Position_To_Spawn_Room, "Corners_And_Walls_Room_1.png", Walls_Texture_Size);

					Another_Room->Create_Invidividual_Room();

					Rooms.push_back(*Another_Room);

					Last_Room_Position = Position_To_Spawn_Room;

					Last_Floor_Rect = Rectangle(Position_To_Spawn_Room.x, Position_To_Spawn_Room.x + Floor_Sprite->Width(), Position_To_Spawn_Room.y, Position_To_Spawn_Room.y + Floor_Sprite->Height());
				}
				else
				{
					safe_to_spawn = false;
				}
			}
		}
	}

	number_of_rooms = Number_of_Rooms + 2;

}

void World::Connect_Rooms()
{
	for (int i = 0; i < (number_of_rooms - 1); i++)
	{
		Rooms[i].Pathfind_Corridor(Rooms[i + 1]);
		Rooms[i].Link_Rooms(Rooms[i + 1]);
	}
}

void World::Play_background_music(int musicID)
{
	HAPI_TStreamedMediaOptions sound_options;
	static int lastCall = 2;
	
	sound_options.loop = true;
	sound_options.volume = 0.5f;

	unsigned int backMusicID = 0;
	unsigned int playMusicID = 1;
	if (lastCall != musicID)
	{
		if (musicID == 0)
		{
			HAPI_Sprites.StopStreamedMedia(playMusicID);

			if (!HAPI_Sprites.PlayStreamedMedia("Data\\Background_Music.wav", sound_options, backMusicID)) {

			}

		}
		else
		{
			HAPI_Sprites.StopStreamedMedia(backMusicID);

			if (!HAPI_Sprites.PlayStreamedMedia("Data\\play_music.wav", sound_options, playMusicID)) {

			}
		}
		lastCall = musicID;
	}
		

		

}

int World::Generate_random_vector(int minimum_value, int maximum_value)
{
	rand_engine.seed(HAPI_Sprites.GetTime());

	std::uniform_int_distribution<int> int_rand(minimum_value, maximum_value);

	int random_scalar = int_rand(rand_engine);

	return random_scalar;
}

void World::spawnenemy(Point tl, Rectangle room_size)
{
	int width = (6*room_size.Width())/7;
	int height = (6*room_size.Height())/7;

	int posX;
	int posY;
	int numberOfEnemies;
	if(room_size.Width() == 768)
		numberOfEnemies = rand() % 5;
	numberOfEnemies = rand() % 5 + 5;
	int enemyCounter = 0;
	
	while (enemyCounter < numberOfEnemies)
	{
		for (auto& entity : entityVector)
		{
			int randomCheck = rand() % 2;
			if (entity->getSide() == enemy  && enemyCounter < numberOfEnemies && randomCheck == 1)
			{
				posX = rand() % width + tl.x + 50;
				posY = rand() % height + tl.y + 50;
				enemyCounter++;
				entity->initialiseValues();
				entity->setPosition(Point(posX, posY));
				entity->setAlive(true);
			}
		}
	}
	

}

void World::spawnBoss(Point tl, Rectangle room_size)
{
	int x = tl.x + room_size.Width() /2;
	int y = tl.y +room_size.Height() /2;

	EntityEnemy *boss = new CEntityEnemyBOSS("irrelevent");

	entityVector.push_back(boss);

	boss->initialiseValues();
	boss->setPosition(Point(x, y));
	boss->setAlive(true);
	


}



void World::bossDeath()
{
	if (restart == false)
	{
		restart = true;
		Reset();
	}
}

void World::PowerUp(Point pos)
{
	//int randomChance = rand() % 3;//to 30 at least
								  //if (randomChance == 1)

	for (auto& entity : entityVector)
	{
		if (entity->getSide() == pickup)
		{
			if (entity->isAlive() == false)
			{
				entity->initialiseValues();
				entity->setPosition(pos);
				entity->setAlive(true);
				break;
			}
		}
	}

	

}


void World::Loading()
{
	Reset();
	currentState = ePlay;
}

void World::Controls(int cameFrom)
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);
	cimg->render(getPlayerPos());

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER] && cameFrom == 0)
	{
		currentState = eMainMenu;
	}

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER] && cameFrom == 1)
	{
		currentState = ePaused;
	}
}
