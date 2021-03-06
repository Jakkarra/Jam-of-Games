#pragma once
#include "Entity.h"

#include <HAPISprites_lib.h>

// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

class CEntityBullet :
	public CEntity
{
public:
	CEntityBullet();
	~CEntityBullet();
	void initialiseValues();
	void update(World& world);
	void setValues(CEntity &other, int weapon, Point offset);
	void resetValues();
	void render(Point playerPos);
	int getDamage() { return attack_; }
	void hasCollided(CEntity &other);
	void setangle(float angle);
	
private:
	Point startPosition{ 0,0 };
	float lifeDuration = 0;
	Point newPos;
};

