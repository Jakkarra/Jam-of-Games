#pragma once
#include "Entity.h"

#include <HAPISprites_lib.h>

// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class CEntityBullet :
	public CEntity
{
public:
	CEntityBullet(std::string textureLocation);
	~CEntityBullet();
	void initialiseValues();
	void update();
	void setValues(std::string bulletType, std::string facing, Point startPos, int side);
	void resetValues();

private:
	Point startPosition{ 0,0 };
};

