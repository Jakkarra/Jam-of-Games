#pragma once
#include "HAPISprites_lib.h"
#include "EntityBullet.h"
#include "EntityPlayer.h"
#include "EntityMenu.h"
#include "Room.h"

class CEntity;

class World
{
public:
	World();
	~World();

	enum menuStates
	{
		eMainMenu, ePlayState, eGameOver, ePaused, eCreation, eWin
	};

	void Run();
	void Initialise();
	void Playing();
	void mainMenu();
	std::vector<CEntityBullet*> getBullets() { return bulletVector; }
	
private:
	menuStates currentState = eMainMenu;

	Room *First_Room;

	Room *Second_Room;

	Room *Third_Room;

	std::vector<CEntity*> entityVector;
	std::vector<CEntityBullet*> bulletVector;
	CEntityPlayer* player_ = new CEntityPlayer;
};

