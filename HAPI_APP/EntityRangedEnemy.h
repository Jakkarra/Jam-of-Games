#pragma once
#include "EntityEnemy.h"
class CEntityRangedEnemy :
	public EntityEnemy
{
public:
	CEntityRangedEnemy(std::string textureLocation);
	~CEntityRangedEnemy();

	void update(World& world);
	void initialiseValues();

};

