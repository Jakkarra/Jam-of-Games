#pragma once
#include "Entity.h"
#include "EntityBullet.h"
#include <HAPISprites_lib.h>

// HAPI Sprites itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

struct Distance
{
	int X, Y;
};

class EntityEnemy :
	public CEntity
{
public:

	EntityEnemy();
	virtual ~EntityEnemy();
	
	void NPCshoot(CEntityBullet* bullet);
	void BOSSshoot(CEntityBullet* bullet, float angle);
	void BOSSALTshoot(CEntityBullet* bullet, float angle);
	void setpos(Point pos);
	bool setalive() { alive_ = true; }

protected:

	Point playerpos_;
	Distance distance_;
	int totaldistance{ 0 };
	float timeToShoot = 0.f;
	unsigned int reloadTime = 250.f;
	unsigned int bulletNum = 0;
	unsigned int bossreloadtime = 50.f;
	unsigned int bossreloadrearreload = 50.f;
};

