#pragma once
class World
{
public:
	World();
	~World();

	enum menuStates
	{
		mainMenu, playState, gameOver, paused, creation
	};

	void Run();

private:
	menuStates currentState = mainMenu;
};

