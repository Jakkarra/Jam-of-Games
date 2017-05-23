#pragma once
#include "EntityEnemy.h"
class CEntityBruteEnemy :
	public EntityEnemy
{
public:
	CEntityBruteEnemy(std::string textureLocation);
	~CEntityBruteEnemy();

	void update(World& world);
	void initialiseValues();
};

