#pragma once
#include "HAPISprites_lib.h"

class Room;

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


private:
	menuStates currentState = eMainMenu;

	Room *First_Room;
	Room *Second_Room;
};

