#pragma once
#include "EntityEnemy.h"
class CEntityEnemyBOSS :
	public EntityEnemy
{
public:
	CEntityEnemyBOSS(std::string textureLocation);
	~CEntityEnemyBOSS();

	void update(World& world);
	void initialiseValues();

private:
	float shooting_timer_{ 60 };
};

