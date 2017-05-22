#pragma once
#include "EntityEnemy.h"
class CEntityEnemyMelee :
	public EntityEnemy
{
public:
	CEntityEnemyMelee(std::string textureLocation);
	~CEntityEnemyMelee();

	void update(World& world);
	void initialiseValues();
};

