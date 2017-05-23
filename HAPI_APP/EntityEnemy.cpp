#include "EntityEnemy.h"
#include "World.h"





EntityEnemy::EntityEnemy()
{
}

EntityEnemy::~EntityEnemy()
{
}


void EntityEnemy::NPCshoot(CEntityBullet * bullet)
{
	if (HAPI_Sprites.GetTime() > reloadTime)
	{
		bullet->setValues(*this, 1);
		reloadTime = HAPI_Sprites.GetTime() + 1250;
	}
}

void EntityEnemy::BOSSshoot(CEntityBullet * bullet,float angle)
{
	if (HAPI_Sprites.GetTime() > bossreloadtime)
	{
		bullet->setValues(*this, 1);
		bossreloadtime = HAPI_Sprites.GetTime() + 75;
	}
}

void EntityEnemy::BOSSALTshoot(CEntityBullet * bullet, float angle)
{
	if (HAPI_Sprites.GetTime() > bossreloadrearreload)
	{
		bullet->setValues(*this, 1);
		bossreloadrearreload = HAPI_Sprites.GetTime() + 75;
	}
}

void EntityEnemy::setpos(Point pos)
{
	pos_ = pos;
}


