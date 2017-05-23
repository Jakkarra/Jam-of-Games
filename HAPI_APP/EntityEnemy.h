#pragma once
#include "Entity.h"
#include <HAPISprites_lib.h>

// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class EntityEnemy :
	public CEntity
{
public:

	EntityEnemy(std::string textureLocation);
	~EntityEnemy();
	void initialiseValues();
	void update(World& world);
	void hasCollided(CEntity &other);
	void render(Point playerPos);


private:
	Point playerpos_;
	float eframeTime = 0;
	unsigned int eframeOffset = 0;
	unsigned int enumberOfFramesForAnimation = 1;

};

