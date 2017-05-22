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
			pause();
		}
		int x = Generate_random_vector(-500, 500);
		int v = Generate_random_vector(-500, 500);
		int y = Generate_random_vector(-250, 250);
		std::cout <<"First Random:" <<  x << std::endl;
		std::cout <<"Second Random" << x*y << std::endl;
	}
}

void World::Initialise()
{

	Create_Rooms(8, 32);

	Connect_Rooms();

	entityVector.push_back(player_);

	for (int i = 0; i < 500; i++)
	{
		CEntityBullet *bullet = new CEntityBullet;
		bulletVector.push_back(bullet);
		//max of 500 bullets
	}

	EntityEnemy* enemy_;

	for (int i = 0; i < 20; i++)
	{
		int select = (rand() % 2);

		EnemyType type_select;

		switch (select)
		{
		case 0:
			type_select = eMelee;
			break;
		case 1:
			type_select = eRanged;
			break;
		case 2:
			type_select = eBrute;
			break;
		}

		spawnenemy(enemy_, First_Room->Get_Room_Position(), First_Room->getsize(), "", type_select);
	}

	spawnenemy(enemy_, First_Room->Get_Room_Position(), First_Room->getsize(), "", eBoss);


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

}

void World::Playing()
{
	if (player_->isAlive() == false)
		currentState = eGameOver;
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0); //maybe find a way to make this a public thing so we dont ned to constantly create it

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER] && conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER])
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

	for (auto room : Rooms)
	{
		room.Render_Floor(getPlayerPos());
		
		if (room.Check_Path_Exists() == true)
		{
			room.Render_Path("Seamless_Texture.png", getPlayerPos());

			room.Spawn_Points(getPlayerPos());
		}
	}


	for (auto p : entityVector) //might be better to have a single vector instead of two and have the offset for where the bullets start
		p->render(getPlayerPos());

	for (auto p : bulletVector) //also the render is seperate to the update as update is every tick, render may be slowed down
		p->render(getPlayerPos());
	
	//need check collision between the rooms and player/enemies also then corridor checks 


}

void World::mainMenu()
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);
	static int trans1 = 255;
	static int trans2 = 70;
	static float timelimit = 0;

	menuStates selectedState = eCharacter;
	

	sp->setPosition(Point{ 1725,980 });
	HAPI_Sprites.RenderText(1650, 990, HAPI_TColour(255, 255, 255, 255), "Press		 to select", 24);

	HAPI_Sprites.RenderText(0, 750, HAPI_TColour(255, 255, 255, trans1), "Play", 84);
	HAPI_Sprites.RenderText(0, 870, HAPI_TColour(255, 255, 255, trans2), "Controls", 84);
	bg->render(getPlayerPos());
	sp->render(getPlayerPos());
	
	if (conData.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER]) //selection
	{
		currentState = selectedState;
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
	//Menu
	totalStats = healthPoints + speedPoints + ratePoints + damagePoints;
	totalPoints - totalStats;
	static float timelimit = 0;
	static int trans1 = 255;
	static int trans2 = 70;
	static int trans3 = 70;
	static int trans4 = 70;

	HAPI_Sprites.RenderText(650, 200, HAPI_TColour(255, 255, 255, 255), 0, 0, "Choose Your Stats!", 60);
	HAPI_Sprites.RenderText(500, 350, HAPI_TColour(255, 255, 255, trans1), 0, 0, "Health:", 34);
	HAPI_Sprites.RenderText(500, 450, HAPI_TColour(255, 255, 255, trans2), 0, 0, "Speed:", 34);
	HAPI_Sprites.RenderText(500, 550, HAPI_TColour(255, 255, 255, trans3), 0, 0, "Fire Rate:", 34);
	HAPI_Sprites.RenderText(500, 650, HAPI_TColour(255, 255, 255, trans4), 0, 0, "Damage:", 34);

	HAPI_Sprites.RenderText(900, 350, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(healthPoints), 34);
	HAPI_Sprites.RenderText(900, 450, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(speedPoints), 34);
	HAPI_Sprites.RenderText(900, 550, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(ratePoints), 34);
	HAPI_Sprites.RenderText(900, 650, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(damagePoints), 34);

	HAPI_Sprites.RenderText(1000, 270, HAPI_TColour(255, 255, 255, 255), 0, 0, "Points Remaining:", 34);
	HAPI_Sprites.RenderText(1400, 272, HAPI_TColour(255, 255, 255, 255), 0, 0, std::to_string(totalPoints), 34);

	sp->setPosition(Point{ 1725,980 });
	HAPI_Sprites.RenderText(1650, 990, HAPI_TColour(255, 255, 255, 255), "Press		 to select", 24);

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
			trans2 = 70;
			trans3 = 70; //health
			trans4 = 70;
			isHealth = true;
		}
		if (optionSelected == 1)
		{
			trans1 = 70;
			trans2 = 255;
			trans3 = 70; //speed
			trans4 = 70;
			isSpeed = true;
		}
		if (optionSelected == 2)
		{
			trans1 = 70;
			trans2 = 70; //firerate
			trans3 = 255;
			trans4 = 70;
			isRate = true;
		}
		if (optionSelected == 3)
		{
			trans1 = 70;
			trans2 = 70; //damage
			trans3 = 70;
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
		player_->initialiseValues(healthPoints, speedPoints, ratePoints, damagePoints, 0);
		healthPoints = ratePoints = damagePoints = speedPoints = 1;
		currentState = ePlay;
	}


}
void World::endGame()
{

	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);
	HAPI_Sprites.RenderText(660, 540, HAPI_TColour(255, 255, 0), "Game Over State, LT to retry", 70);

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER])
	{
		for (auto p : entityVector)
			p->initialiseValues();

		currentState = eCharacter;
	}



}
void World::pause()
{
	const HAPI_TControllerData &conData = HAPI_Sprites.GetControllerData(0);
	static bool canExit = false;

	if (!conData.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER])
		canExit = true;
	
	HAPI_Sprites.RenderText(660, 540, HAPI_TColour(255, 255, 0), "PAUSED", 70);

	if (conData.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER] && canExit == true)
	{
		currentState = ePlay;
		canExit = false;
	}


}

void World::Create_Rooms(int Number_of_Rooms, int Walls_Texture_Size)
{
	Point First_Room_Position{ 960,  540 };

	First_Room = new Room("Floor_1.png", First_Room_Position, "Corners_And_Walls_Room_1.png", Walls_Texture_Size);

	First_Room->Create_Invidividual_Room();

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
					Position_To_Spawn_Room = Point{Last_Room_Position.x - (int)Floor_Sprite->Width() - Generate_random_vector(600, 800),  Last_Room_Position.y };
					last_direction_to_spawn = direction_to_spawn;
					try_position = true;
				}

				if (direction_to_spawn == 2 && last_direction_to_spawn != 1)
				{
					Position_To_Spawn_Room = Point{ Last_Room_Position.x + (int)Last_Floor_Rect.Width() + Generate_random_vector(600, 800),  Last_Room_Position.y };
					last_direction_to_spawn = direction_to_spawn;
					try_position = true;
				}

				if (direction_to_spawn == 3 && last_direction_to_spawn != 4)
				{
					Position_To_Spawn_Room = Point{ Last_Room_Position.x, Last_Room_Position.y - (int)Floor_Sprite->Height() - Generate_random_vector(600, 800) };
					last_direction_to_spawn = direction_to_spawn;
					try_position = true;
				}

				if (direction_to_spawn == 4 && last_direction_to_spawn != 3)
				{
					Position_To_Spawn_Room = Point{ Last_Room_Position.x, Last_Room_Position.y + (int)Last_Floor_Rect.Height() + Generate_random_vector(600, 800) };
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

int World::Generate_random_vector(int minimum_value, int maximum_value)
{
	rand_engine.seed(HAPI_Sprites.GetTime());

	std::uniform_int_distribution<int> int_rand(minimum_value, maximum_value);

	int random_scalar = int_rand(rand_engine);

	return random_scalar;
}

void World::spawnenemy(EntityEnemy* enemy_, Point tl, Rectangle room_size, std::string sprite, EnemyType type)
{
	int width = room_size.Width();
	int height = room_size.Height();

	int posX = rand() %  width+ tl.x;
	int posY = rand() % height + tl.y;

	switch (type)
	{
	case eMelee:
		enemy_ = new CEntityEnemyMelee("Data//fireBall.png");
		break;
	case eRanged:
		enemy_ = new CEntityRangedEnemy("Data//rocketUp.png");
		break;
	case eBrute:
		enemy_ = new CEntityBruteEnemy("Data//HPHeartEmpty.png");
		break;
	case eBoss:
		enemy_ = new CEntityEnemyBOSS("Data//HAPI Sprites Logo.png");
		break;
	}


	Point pos = { posX,posY };

	enemy_->setpos(pos);

	entityVector.push_back(enemy_);
}

void World::activatenemy(Point tl, Rectangle roomsize, EnemyType type)
{

	int width = roomsize.Width();
	int height = roomsize.Height();

	int posX = rand() % width + tl.x;
	int posY = rand() % height + tl.y;
	Point pos = { posX, posY };
	for (int i = 0; i < entityVector.size(); i++)
	{
		if (entityVector[i]->getclass() != eplayer)
		{
			if (entityVector[i]->getclass() == type)
			{
				entityVector[i]->setPosition(pos);
				entityVector[i]->isAlive();
			}
		}
	}
}
