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
	void update();
	void render(Point pos, float angle);

private:


};

