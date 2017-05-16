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
	void setValues(CEntity &other);
	void resetValues();
	void render(Point playerPos);
	int getDamage() { return attack_; }
private:
	Point startPosition{ 0,0 };
	float lifeDuration = 0;
};

