#pragma once
#include "HAPISprites_lib.h"
#include "EntityBullet.h"
#include "EntityPlayer.h"
#include "EntityMenu.h"
#include "EntityEnemy.h"

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
	Point getPlayerPos() { return player_->getPos(); }
	
private:
	menuStates currentState = eMainMenu;

	std::vector<CEntity*> entityVector;
	std::vector<CEntityBullet*> bulletVector;
	CEntityPlayer* player_ = new CEntityPlayer;
	float currTime = 0;
	float updateTime = 0;
};

